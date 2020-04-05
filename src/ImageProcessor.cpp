#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(ViewerWidget* viewer)
{
	_view_w = viewer;
}

ImageProcessor::~ImageProcessor()
{
}

void ImageProcessor::mirrorExtendAccepted()
{
	MirrorExtendDialog* mirrorExtendDialog = static_cast<MirrorExtendDialog*>(sender());

	int nPixels = mirrorExtendDialog->getPixels();
	mirrorExtendImageBy(nPixels);
}

void ImageProcessor::blurAccepted()
{
	BlurDialog* blurDialog = static_cast<BlurDialog*>(sender());

	int radius = blurDialog->getRadius();
	bool printToConsole = blurDialog->printKernelToConsole();
	int kerType = blurDialog->getKernelType();
	// 0 - Gaussian, 1 - Averaging
	if (kerType == 0) {
		getGaussianKernel(radius, printToConsole);
	}
	else {
		getAveragingKernel(radius, printToConsole);
	}

	QImage* img = _view_w->getImage();
	QImage extended = QImage(*img);
	_view_w->setImage(extended);
	mirrorExtendImageBy(radius);
	blurImage(radius);
	clearMasks();
}

void ImageProcessor::bernsenThresholdAccepted()
{
	BernsenThresholdDialog* bernsenThresholdDialog = static_cast<BernsenThresholdDialog*>(sender());

	int bgType = bernsenThresholdDialog->getBackgroundType();
	int radius = bernsenThresholdDialog->getRadius();
	int minContrast = bernsenThresholdDialog->getMinContrast();
	bool printMaskToConsole = bernsenThresholdDialog->printMaskToConsole();
	bool convertToGrayscale = bernsenThresholdDialog->convertToGrayscale();

	getCircularKernel(radius, printMaskToConsole);

	if (convertToGrayscale) grayscale();
	QImage* img = _view_w->getImage();
	QImage extended = QImage(*img);
	_view_w->setImage(extended);
	mirrorExtendImageBy(radius);
	bernsenThreshold(radius, minContrast, bgType);
	clearMasks();
}

void ImageProcessor::multiBlurAccepted()
{
	MultiBlurDialog* multiBlur = static_cast<MultiBlurDialog*>(sender());

	int nSteps = multiBlur->getSteps();

	printf("multiBlur with %d steps\n", nSteps);
}

// update viewer image after histogram stretch
void ImageProcessor::on_stretch()
{
	_view_w->setDataPtr();
	_view_w->update();
}

void ImageProcessor::on_histogramGrayscale()
{
	grayscale();
	emit sigGrayscaled();
}

void ImageProcessor::on_singleThreshold()
{
	HistogramWindow* hw = static_cast<HistogramWindow*>(sender());
	int threshold = hw->threshold();

	uchar* data = _view_w->getData();
	int height = _view_w->getImgHeight();
	int width = _view_w->getImgWidth();
	int row = _view_w->getImage()->bytesPerLine();

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			uchar val = (data[i * row + j] > threshold ? 255 : 0);
			_view_w->setPixel(_view_w->getImage(), j, i, static_cast<uchar>(val));
		}
	}
	_view_w->update();
	hw->setImage(_view_w->getImage());
	hw->updateHistogram();
}

bool ImageProcessor::clearImage()
{
	_view_w->clear();
	_view_w->update();
	return true;
}

bool ImageProcessor::invertColors()
{
	int height = _view_w->getImgHeight();
	int width = _view_w->getImgWidth();
	uchar* data = _view_w->getData();

	int row = _view_w->getImage()->bytesPerLine();
	int depth = _view_w->getImage()->depth();

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (depth == 8) {
				_view_w->setPixel(_view_w->getImage(), j, i, static_cast<uchar>(255 - data[i * row + j]));
			}
			else {
				uchar r = static_cast<uchar>(255 - data[i * row + j * 4]);
				uchar g = static_cast<uchar>(255 - data[i * row + j * 4 + 1]);
				uchar b = static_cast<uchar>(255 - data[i * row + j * 4 + 2]);
				_view_w->setPixel(_view_w->getImage(), j, i, r, g, b);
			}
		}
	}
	_view_w->update();
	return true;
}

bool ImageProcessor::grayscale(float wRed, float wGreen, float wBlue, float gamma)
{
	if (_view_w->getImage()->depth() == 8) {
		return true;
	}

	int height = _view_w->getImgHeight();
	int width = _view_w->getImgWidth();
	uchar* data = _view_w->getData();

	int row = _view_w->getImage()->bytesPerLine();

	QImage bwImage = QImage(QSize(width, height), QImage::Format_Grayscale8);
	bwImage.fill(QColor(0, 0, 0));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			uchar r = static_cast<uchar>(data[i * row + j * 4]);
			uchar g = static_cast<uchar>(data[i * row + j * 4 + 1]);
			uchar b = static_cast<uchar>(data[i * row + j * 4 + 2]);
			// gamma correction:
			float rLin = pow((float)r / 256.0f, gamma);
			float gLin = pow((float)g / 256.0f, gamma);
			float bLin = pow((float)b / 256.0f, gamma);
			int intensity = std::round(wRed * rLin * 256 + wGreen * gLin * 256 + wBlue * bLin * 256);
			intensity = std::max(0, std::min(intensity, 255));

			_view_w->setPixel(&bwImage, j, i, static_cast<uchar>(intensity));
		}
	}

	_view_w->setImage(bwImage);
	_view_w->update();
	return true;
}

bool ImageProcessor::mirrorExtendImageBy(int nPixels)
{
	uchar* data = _view_w->getData();

	int row = _view_w->getImage()->bytesPerLine();
	int depth = _view_w->getImage()->depth();

	const int oldHeight = _view_w->getImgHeight();
	const int oldWidth = _view_w->getImgWidth();
	const int newHeight = oldHeight + 2 * nPixels;
	const int newWidth = oldWidth + 2 * nPixels;

	QImage::Format format = _view_w->getImage()->format();
	QImage newImg = QImage(QSize(newWidth, newHeight), format);
	newImg.fill(QColor(0, 0, 0));

	for (int i = 0; i < newHeight; i++) {
		for (int j = 0; j < newWidth; j++) {

			int iPos; int jPos;
			// upper left corner
			if (i < nPixels && j < nPixels) {
				iPos = nPixels - 1 - i;
				jPos = nPixels - 1 - j;
			}
			// top strip
			else if (i < nPixels && j >= nPixels && j < newWidth - nPixels) {
				iPos = nPixels - 1 - i;
				jPos = j - nPixels;
			}
			// top right corner
			else if (i < nPixels && newWidth - j <= nPixels) {
				iPos = nPixels - 1 - i;
				jPos = nPixels + 2 * oldWidth - 1 - j;
			}
			// left strip
			else if (i >= nPixels && i < nPixels + oldHeight && j < nPixels) {
				iPos = i - nPixels;
				jPos = nPixels - 1 - j;
			}
			// right strip
			else if (i >= nPixels && i < nPixels + oldHeight && j >= newWidth - nPixels) {
				iPos = i - nPixels;
				jPos = nPixels + 2 * oldWidth - 1 - j;
			}
			// bottom left corner
			else if (i >= nPixels + oldHeight && j < nPixels) {
				iPos = nPixels + 2 * oldHeight - 1 - i;
				jPos = nPixels - 1 - j;
			}
			// bottom strip
			else if (i >= nPixels + oldHeight && j >= nPixels && j < newWidth - nPixels) {
				iPos = nPixels + 2 * oldHeight - 1 - i;
				jPos = j - nPixels;
			}
			// bottom right corner
			else if (i >= nPixels + oldHeight && j >= newWidth - nPixels) {
				iPos = nPixels + 2 * oldHeight - 1 - i;
				jPos = nPixels + 2 * oldWidth - 1 - j;
			}
			// inside old image
			else {
				iPos = i - nPixels;
				jPos = j - nPixels;
			}

			if (depth == 8) {
				_view_w->setPixel(&newImg, j, i, static_cast<uchar>(data[iPos * row + jPos]));
			}
			else {
				uchar r = static_cast<uchar>(data[iPos * row + jPos * 4]);
				uchar g = static_cast<uchar>(data[iPos * row + jPos * 4 + 1]);
				uchar b = static_cast<uchar>(data[iPos * row + jPos * 4 + 2]);
				_view_w->setPixel(&newImg, j, i, r, g, b);
			}
		}
	}

	_view_w->setImage(newImg);
	_view_w->update();
	return true;
}

uchar ImageProcessor::kernelSumGS(uchar* img, int row, int x, int y, int r)
{
	float sum = 0.0f;
	for (int i = 0; i < 2 * r + 1; i++) {
		for (int j = 0; j < 2 * r + 1; j++) {
			int xPos = x - r + j, yPos = y - r + i;
			sum += ((float)img[yPos * row + xPos]) * W->at((size_t)i * (2 * r + 1) + j);
		}
	}

	return (uchar)std::fminf(sum + 0.5, 255.f);
}

QRgb ImageProcessor::kernelSumRGB(uchar* img, int row, int x, int y, int r)
{
	float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
	for (int i = 0; i < 2 * r + 1; i++) {
		for (int j = 0; j < 2 * r + 1; j++) {
			int xPos = x - r + j, yPos = y - r + i;
			sumR += ((float)img[yPos * row + xPos * 4]) * W->at((size_t)i * (2 * r + 1) + j);
			sumG += ((float)img[yPos * row + xPos * 4 + 1]) * W->at((size_t)i * (2 * r + 1) + j);
			sumB += ((float)img[yPos * row + xPos * 4 + 2]) * W->at((size_t)i * (2 * r + 1) + j);
		}
	}

	return qRgb(
		(int)std::fminf(sumR + 0.5, 255.f),
		(int)std::fminf(sumG + 0.5, 255.f),
		(int)std::fminf(sumB + 0.5, 255.f)
	);
}

void ImageProcessor::findGrayscaleExtremesWithinCircle(uchar* img, int row, int x, int y, uchar* minVal, uchar* maxVal)
{
	for (int i = 0; i < iMask->size(); i += 2) {
		int iPos = y + iMask->at(i); int jPos = x + iMask->at((size_t)i + 1);
		*minVal = (img[iPos * row + jPos] < *minVal ? img[iPos * row + jPos] : *minVal);
		*maxVal = (img[iPos * row + jPos] > *maxVal ? img[iPos * row + jPos] : *maxVal);
	}
}

void ImageProcessor::findRGBExtremesWithinCircle(uchar* img, int row, int x, int y, QColor* minVal, QColor* maxVal)
{
	for (int i = 0; i < iMask->size(); i += 2) {
		int iPos = y + iMask->at(i); int jPos = x + iMask->at((size_t)i + 1);
		minVal->setRed(img[iPos * row + jPos * 4] < minVal->red() ? img[iPos * row + jPos * 4] : minVal->red());
		minVal->setGreen(img[iPos * row + jPos * 4 + 1] < minVal->green() ? img[iPos * row + jPos * 4 + 1] : minVal->green());
		minVal->setBlue(img[iPos * row + jPos * 4 + 2] < minVal->blue() ? img[iPos * row + jPos * 4 + 2] : minVal->blue());

		maxVal->setRed(img[iPos * row + jPos * 4] > maxVal->red() ? img[iPos * row + jPos * 4] : maxVal->red());
		maxVal->setGreen(img[iPos * row + jPos * 4 + 1] > maxVal->green() ? img[iPos * row + jPos * 4 + 1] : maxVal->green());
		maxVal->setBlue(img[iPos * row + jPos * 4 + 2] > maxVal->blue() ? img[iPos * row + jPos * 4 + 2] : maxVal->blue());
	}
}

bool ImageProcessor::blurImage(int radius)
{
	int height = _view_w->getImgHeight();
	int width = _view_w->getImgWidth();
	uchar* data = _view_w->getData();

	int row = _view_w->getImage()->bytesPerLine();
	int depth = _view_w->getImage()->depth();

	QImage blurredImg = QImage(QSize(width - 2 * radius, height - 2 * radius), _view_w->getImage()->format());
	blurredImg.fill(QColor(0, 0, 0));

	for (int x = 0; x < blurredImg.width(); x++) {
		for (int y = 0; y < blurredImg.height(); y++) {
			int xPos = x + radius, yPos = y + radius;
			if (depth == 8) {
				_view_w->setPixel(&blurredImg, x, y, static_cast<uchar>(kernelSumGS(data, row, xPos, yPos, radius)));
			}
			else {
				QColor col = kernelSumRGB(data, row, xPos, yPos, radius);
				_view_w->setPixel(&blurredImg, x, y,
					static_cast<uchar>(col.red()), static_cast<uchar>(col.green()), static_cast<uchar>(col.blue())
				);
			}
		}
	}

	_view_w->setImage(blurredImg);
	_view_w->update();
	return true;
}

bool ImageProcessor::bernsenThreshold(int radius, int minContrast, int bgType)
{
	uchar subThreshold = (bgType ? 0 : 255);

	int height = _view_w->getImgHeight();
	int width = _view_w->getImgWidth();
	uchar* data = _view_w->getData();

	int row = _view_w->getImage()->bytesPerLine();
	int depth = _view_w->getImage()->depth();

	QImage binaryImg = QImage(QSize(width - 2 * radius, height - 2 * radius), _view_w->getImage()->format());
	binaryImg.fill(QColor(0, 0, 0));

	for (int x = 0; x < binaryImg.width(); x++) {
		for (int y = 0; y < binaryImg.height(); y++) {
			int xPos = x + radius, yPos = y + radius;
			if (depth == 8) {
				uchar Imin = 255; uchar Imax = 0;
				findGrayscaleExtremesWithinCircle(data, row, xPos, yPos, &Imin, &Imax);
				uchar contrast = Imax - Imin;
				uchar threshold = (contrast >= minContrast ? std::round(0.5f * ((float)Imax + (float)Imin)) : subThreshold);
				uchar pxVal = (data[yPos * row + xPos] > threshold ? 255 : 0);
				
				_view_w->setPixel(&binaryImg, x, y, static_cast<uchar>(pxVal));
			}
			else {
				QColor Imin = QColor(255, 255, 255);
				QColor Imax = QColor(0, 0, 0);
				findRGBExtremesWithinCircle(data, row, xPos, yPos, &Imin, &Imax);
				QColor contrast = QColor(Imax.red() - Imin.red(), Imax.green() - Imin.green(), Imax.blue() - Imin.blue());
				QColor threshold = QColor(
					(contrast.red() >= minContrast ? std::round(0.5f * ((float)Imax.red() + (float)Imin.red())) : subThreshold),
					(contrast.green() >= minContrast ? std::round(0.5f * ((float)Imax.green() + (float)Imin.green())) : subThreshold),
					(contrast.blue() >= minContrast ? std::round(0.5f * ((float)Imax.blue() + (float)Imin.blue())) : subThreshold)
				);
				uchar r = static_cast<uchar>(data[yPos * row + xPos * 4]);
				uchar g = static_cast<uchar>(data[yPos * row + xPos * 4 + 1]);
				uchar b = static_cast<uchar>(data[yPos * row + xPos * 4 + 2]);
				QColor col = QColor(
					(r > threshold.red() ? 255 : 0),
					(g > threshold.green() ? 255 : 0),
					(b > threshold.blue() ? 255 : 0)
				);

				_view_w->setPixel(&binaryImg, x, y,
					static_cast<uchar>(col.red()), static_cast<uchar>(col.green()), static_cast<uchar>(col.blue())
				);
			}
		}
	}

	_view_w->setImage(binaryImg);
	_view_w->update();
	return true;
}


void ImageProcessor::getGaussianKernel(int radius, bool print)
{
	if (W) delete W;
	const int size = (2 * radius + 1) * (2 * radius + 1);
	W = new std::vector<float>(size, 0.0f);

	float sigma = radius / 3.0f, h = 1.0f;
	float x0, x1, y0, y1, wSum = 0.0f;

	// direct integration of the kernel using error function (erf)
	for (int i = 0; i <= 2 * radius; i++) {
		for (int j = 0; j <= 2 * radius; j++) {
			x0 = (2 * i - 1) * h / 2 - radius;	x1 = (2 * i + 1) * h / 2 - radius;
			y0 = (2 * j - 1) * h / 2 - radius;	y1 = (2 * j + 1) * h / 2 - radius;

			// 2D gaussians are independent G(x,y) = G(x) G(y) and so are their integrals
			float pxIntegral = 0.25f * (erf(x1 / (M_SQRT2 * sigma)) - erf(x0 / (M_SQRT2 * sigma))) * (erf(y1 / (M_SQRT2 * sigma)) - erf(y0 / (M_SQRT2 * sigma)));

			int id = i * (2 * radius + 1) + j;
			W->at(id) = pxIntegral;
			wSum += pxIntegral;
		}
	}

	// normalize
	for (int i = 0; i <= 2 * radius; i++) {
		for (int j = 0; j <= 2 * radius; j++) {
			int id = i * (2 * radius + 1) + j;
			W->at(id) /= wSum;
		}
	}

	if (print) {
		printf("K = \n");
		for (int i = 0; i <= 2 * radius; i++) {
			for (int j = 0; j <= 2 * radius; j++) {
				int id = i * (2 * radius + 1) + j;
				printf("%f ", W->at(id));
			}
			printf("\n");
		}
	}
}

void ImageProcessor::getAveragingKernel(int radius, bool print)
{
	if (W) delete W;
	int size = (2 * radius + 1) * (2 * radius + 1);
	float pxWeight = 1.0f / size;
	W = new std::vector<float>(size, pxWeight);

	if (print) {
		printf("K = \n");
		for (int i = 0; i <= 2 * radius; i++) {
			for (int j = 0; j <= 2 * radius; j++) {
				int id = i * (2 * radius + 1) + j;
				printf("%f ", W->at(id));
			}
			printf("\n");
		}
	}
}

void ImageProcessor::getCircularKernel(int radius, bool print)
{
	if (iMask) delete iMask;
	int radiusSq = radius * radius;
	iMask = new std::vector<int>();

	for (int i = 0; i <= 2 * radius; i++) {
		for (int j = 0; j <= 2 * radius; j++) {
			int iPos = (i - radius); int jPos = (j - radius);
			if (iPos * iPos + jPos * jPos <= radiusSq) {
				iMask->push_back(iPos); iMask->push_back(jPos);
			}
		}
	}

	if (print) {
		int iId = 0;
		printf("K = \n");
		for (int i = 0; i <= 2 * radius; i++) {
			for (int j = 0; j <= 2 * radius; j++) {
				int iPos = (i - radius); int jPos = (j - radius);
				if (iPos * iPos + jPos * jPos <= radiusSq) {
					printf("(%s%d, %s%d) ", (iPos < 0 ? "" : " "), iMask->at(iId++), (jPos < 0 ? "" : " "), iMask->at(iId++));
				}
				else {
					printf("(  ,   ) ");
				}
			}
			printf("\n");
		}
	}
}

void ImageProcessor::clearMasks()
{
	if (W) {
		W->clear();
		delete W;
		W = nullptr;
	}
	if (iMask) {
		iMask->clear();
		delete iMask;
		iMask = nullptr;
	}
}
