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

	nSteps = multiBlur->getSteps();
	int radius = multiBlur->getRadius();

	printf("multiBlur with %d steps\n", nSteps);
	
	_view_w->clearImages();
	_view_w->allocateImages(nSteps + 1);

	getGaussianKernel(radius);

	for (int i = 0; i < nSteps; i++) {
		QImage img = QImage(*_view_w->getImage());
		_view_w->setImageAt(img, i);
		QImage extended = QImage(img);
		_view_w->setImage(extended);
		mirrorExtendImageBy(radius);
		blurImage(radius);
		printf("step %d blurred\n", i + 1);
	}
	_view_w->setImageAt(QImage(*_view_w->getImage()), nSteps);
	_view_w->imgId = nSteps;
	_view_w->setImage(_view_w->imgArray[nSteps]);
	clearMasks();

	emit multiImageComplete();
}

void ImageProcessor::heatEquationAccepted()
{
	HeatEquationDialog* heqDialog = static_cast<HeatEquationDialog*>(sender());
	float timeStep = heqDialog->getTimeStep();
	int scheme = heqDialog->getScheme();
	nSteps = heqDialog->getNSteps();

	// time step array prep:
	_view_w->clearImages();
	_view_w->allocateImages(nSteps + 1);

	// mirror extend first img
	QImage img = QImage(*_view_w->getImage());
	_view_w->setImageAt(img, 0);
	QImage extended = QImage(img);
	_view_w->setImage(extended);
	mirrorExtendImageBy(1);

	double* rhsData;
	if (scheme > 0) {
		// save double px values from previous time step
		int dataSize = _view_w->getImage()->sizeInBytes();
		uchar* data = _view_w->getData();
		rhsData = new double[dataSize];
		for (int i = 0; i < dataSize; i++) rhsData[i] = (double)data[i];
	}

	for (int i = 1; i <= nSteps; i++) {
		printf("<=== heatEquation step %d...\n", i);
		if (scheme == 0) {
			explicitHeatEquation(timeStep);
		}
		else {
			implicitHeatEquation(timeStep, rhsData);
		}
		printf("=== heatEquation step %d complete ===>\n", i);

		QImage img = QImage(*_view_w->getImage());
		_view_w->setImageAt(img, i);
		QImage extended = QImage(img);
		_view_w->setImage(extended);
		mirrorExtendImageBy(1); // extend by 1 for zero Neumann B.C.

		if (scheme > 0 && i < nSteps) {
			// save double px values
			int dataSize = _view_w->getImage()->sizeInBytes();
			uchar* data = _view_w->getData();
			for (int i = 0; i < dataSize; i++) rhsData[i] = (double)data[i];
		}
	}

	if (scheme > 0)	delete[] rhsData;

	emit multiImageComplete();
}

void ImageProcessor::peronaMalikAccepted()
{
	PeronaMalikDialog* pmDialog = static_cast<PeronaMalikDialog*>(sender());
	float timeStep = pmDialog->getTimeStep();
	int scheme = pmDialog->getScheme();
	float K_coeff = pmDialog->getKCoeff();
	nSteps = pmDialog->getNSteps();

	// time step array prep:
	_view_w->clearImages();
	_view_w->allocateImages(nSteps + 1);

	// mirror extend first img
	QImage img = QImage(*_view_w->getImage());
	_view_w->setImageAt(img, 0);
	QImage extended = QImage(img);
	_view_w->setImage(extended);
	mirrorExtendImageBy(1);


	const int dataSize = _view_w->getImage()->sizeInBytes();
	uchar* data = _view_w->getData();
	double* uData = new double[dataSize];
	for (int i = 0; i < dataSize; i++) uData[i] = (double)data[i];
	double* rhsData;
	if (scheme > 0) {	// save double px values from previous time step
		rhsData = new double[dataSize];
		for (int i = 0; i < dataSize; i++) rhsData[i] = (double)data[i];
	}

	for (int i = 1; i <= nSteps; i++) {
		printf("<=== Perona-Malik Equation step %d...\n", i);
		if (scheme == 0) {
			explicitPeronaMalik(timeStep, K_coeff, uData);
		}
		else {
			implicitPeronaMalik(timeStep, K_coeff, uData, rhsData);
		}
		printf("=== Perona-Malik Equation step %d complete ===>\n", i);

		QImage img = QImage(*_view_w->getImage());
		_view_w->setImageAt(img, i);
		QImage extended = QImage(img);
		_view_w->setImage(extended);
		mirrorExtendImageBy(1); // extend by 1 for zero Neumann B.C.

		data = _view_w->getData();
		if (scheme > 0 && i < nSteps) {
			// save double px values
			for (int i = 0; i < dataSize; i++) rhsData[i] = (double)data[i];
		}
		if (i < nSteps) for (int i = 0; i < dataSize; i++) uData[i] = (double)data[i];
	}

	if (scheme > 0)	delete[] rhsData;
	delete[] uData;

	emit multiImageComplete();
}

// update viewer image after histogram stretch
void ImageProcessor::on_stretch()
{
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

bool ImageProcessor::explicitHeatEquation(float timeStep)
{
	uchar* data = _view_w->getData();
	int height = _view_w->getImgHeight();
	int width = _view_w->getImgWidth();
	int row = _view_w->getImage()->bytesPerLine();
	int depth = _view_w->getImage()->depth();

	float diagCoeff = 1.0f - 4.0f * timeStep;
	float coeff = timeStep;

	QImage diffusedImg = QImage(QSize(width - 2, height - 2), _view_w->getImage()->format());
	diffusedImg.fill(QColor(0, 0, 0));

	float mean = 0.0f, mean_red = 0.0f, mean_green = 0.0f, mean_blue = 0.0f;
	for (int x = 0; x < diffusedImg.width(); x++) {
		for (int y = 0; y < diffusedImg.height(); y++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			// neighboring pixels:
			int xNorth = xPos, yNorth = y;
			int xSouth = xPos, ySouth = yPos + 1;
			int xWest = x, yWest = yPos;
			int xEast = xPos + 1, yEast = yPos;
			// ------
			if (depth == 8) {
				float val = diagCoeff * data[yPos * row + xPos] +
					coeff * (data[yNorth * row + xNorth] + data[ySouth * row + xSouth] + data[yWest * row + xWest] + data[yEast * row + xEast]);
				mean += val;
				_view_w->setPixel(&diffusedImg, x, y, static_cast<uchar>(std::max(std::min(255, (int)std::round(val)), 0)));
			}
			else {
				uchar r = static_cast<uchar>(data[yPos * row + xPos * 4]);
				uchar rN = static_cast<uchar>(data[yNorth * row + xNorth * 4]);
				uchar rS = static_cast<uchar>(data[ySouth * row + xSouth * 4]);
				uchar rW = static_cast<uchar>(data[yWest * row + xWest * 4]);
				uchar rE = static_cast<uchar>(data[yEast * row + xEast * 4]);

				float valRed = diagCoeff * r + coeff * (rN + rS + rW + rE);
				mean_red += valRed;

				uchar g = static_cast<uchar>(data[yPos * row + xPos * 4 + 1]);
				uchar gN = static_cast<uchar>(data[yNorth * row + xNorth * 4 + 1]);
				uchar gS = static_cast<uchar>(data[ySouth * row + xSouth * 4 + 1]);
				uchar gW = static_cast<uchar>(data[yWest * row + xWest * 4 + 1]);
				uchar gE = static_cast<uchar>(data[yEast * row + xEast * 4 + 1]);

				float valGreen = diagCoeff * g + coeff * (gN + gS + gW + gE);
				mean_green += valGreen;

				uchar b = static_cast<uchar>(data[yPos * row + xPos * 4 + 2]);
				uchar bN = static_cast<uchar>(data[yNorth * row + xNorth * 4 + 2]);
				uchar bS = static_cast<uchar>(data[ySouth * row + xSouth * 4 + 2]);
				uchar bW = static_cast<uchar>(data[yWest * row + xWest * 4 + 2]);
				uchar bE = static_cast<uchar>(data[yEast * row + xEast * 4 + 2]);

				float valBlue = diagCoeff * b + coeff * (bN + bS + bW + bE);
				mean_blue += valBlue;

				_view_w->setPixel(&diffusedImg, x, y, 
					static_cast<uchar>(std::max(std::min(255, (int)std::round(valRed)), 0)),
					static_cast<uchar>(std::max(std::min(255, (int)std::round(valGreen)), 0)),
					static_cast<uchar>(std::max(std::min(255, (int)std::round(valBlue)), 0))
				);
			}
		}
	}
	if (depth == 8) {
		mean /= (width * height);
		printf("u_mean = %f\n", mean);
	}
	else {
		mean_red /= (width * height);
		mean_green /= (width * height);
		mean_blue /= (width * height);
		printf("u_mean(RGB) = (%f, %f, %f)\n", mean_red, mean_green, mean_blue);
	}

	_view_w->setImage(diffusedImg);
	_view_w->update();
	return true;
}

bool ImageProcessor::implicitHeatEquation(float timeStep, double* rhsData, bool printMean, bool printIter)
{
	uchar* data = _view_w->getData();

	int heightTotal = _view_w->getImgHeight();
	int widthTotal = _view_w->getImgWidth();

	int height = heightTotal - 2;
	int width = widthTotal - 2;

	int row = _view_w->getImage()->bytesPerLine();
	int depth = _view_w->getImage()->depth();

	int dataSize = _view_w->getImage()->sizeInBytes();

	// old and new img data (old and new iter)
	double* uData = new double[dataSize];
	for (int i = 0; i < dataSize; i++) uData[i] = 0.0;

	// SOR params:
	double omega = 1.5, tol = 1e-4;
	// SPD matrix coeffs:
	double diagCoeff = 1.0 + 4.0 * (double)timeStep;
	double coeff = -(double)timeStep;

	QImage::Format format = _view_w->getImage()->format();
	QImage diffusedImg = QImage(QSize(width, height), format); // solution
	diffusedImg.fill(QColor(0, 0, 0));

	int iter = 0, maxIter = 100;

	ImageParams imPar(height, width, depth, row);
	IterParams itPar(timeStep, diagCoeff, coeff, omega, printMean, printIter);

	do {
		iter++;
		if (printIter) printf("SOR iter %d: ", iter);		
		doSORIter(&imPar, &itPar, uData, rhsData);

	} while ((iter < maxIter) && (itPar.res > tol));

	// write result
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int xPos = x + 1, yPos = y + 1;

			if (depth == 8) {
				int pos = yPos * row + xPos;
				_view_w->setPixel(&diffusedImg, x, y, static_cast<uchar>(std::max(std::min(255, (int)std::round(uData[pos])), 0)));
			}
			else {
				uchar r = static_cast<uchar>(std::max(std::min(255, (int)std::round(uData[yPos * row + xPos * 4])), 0));
				uchar g = static_cast<uchar>(std::max(std::min(255, (int)std::round(uData[yPos * row + xPos * 4 + 1])), 0));
				uchar b = static_cast<uchar>(std::max(std::min(255, (int)std::round(uData[yPos * row + xPos * 4 + 2])), 0));

				_view_w->setPixel(&diffusedImg, x, y, r, g, b);
			}			
		}
	}

	// rhsOld = dataNew
	for (int i = 0; i < dataSize; i++) rhsData[i] = uData[i];

	delete[] uData;

	_view_w->setImage(diffusedImg);
	_view_w->update();
	return true;
}

bool ImageProcessor::explicitPeronaMalik(float timeStep, double K_coeff, double* data)
{
	int height = _view_w->getImgHeight();
	int width = _view_w->getImgWidth();
	int row = _view_w->getImage()->bytesPerLine();
	int depth = _view_w->getImage()->depth();

	float diagCoeff;
	float coeff = timeStep;
	double* dataNeighbors = new double[9];

	QImage filteredImg = QImage(QSize(width - 2, height - 2), _view_w->getImage()->format());
	filteredImg.fill(QColor(0, 0, 0));

	float mean = 0.0f, mean_red = 0.0f, mean_green = 0.0f, mean_blue = 0.0f;
	for (int x = 0; x < filteredImg.width(); x++) {
		for (int y = 0; y < filteredImg.height(); y++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			// neighboring pixels:
			int xNorth = xPos, yNorth = y;
			int xSouth = xPos, ySouth = yPos + 1;
			int xWest = x, yWest = yPos;
			int xEast = xPos + 1, yEast = yPos;
			// ------
			if (depth == 8) {
				dataNeighbors[0] = data[yNorth * row + xWest]; // northwest
				dataNeighbors[1] = data[yNorth * row + xPos]; // north
				dataNeighbors[2] = data[yNorth * row + xEast]; // northeast
				dataNeighbors[3] = data[yPos * row + xWest]; // west
				dataNeighbors[4] = data[yPos * row + xPos]; // mid
				dataNeighbors[5] = data[yPos * row + xEast]; // east
				dataNeighbors[6] = data[ySouth * row + xWest]; // southwest
				dataNeighbors[7] = data[ySouth * row + xPos]; // south
				dataNeighbors[8] = data[ySouth * row + xEast]; // southeast

				WeightedGrads grads = getWeightedGradsAt(dataNeighbors, K_coeff);
				diagCoeff = 1.0 - timeStep * grads.sum();

				float val = diagCoeff * data[yPos * row + xPos] +
					coeff * sumNeighborPixels(
						data[yNorth * row + xNorth], data[yWest * row + xWest],
						data[ySouth * row + xSouth], data[yEast * row + xEast], &grads);
				mean += val;
				_view_w->setPixel(&filteredImg, x, y, static_cast<uchar>(std::max(std::min(255, (int)std::round(val)), 0)));
			}
			else {
				// ======== RED CHANNEL =======================================
				dataNeighbors[0] = data[yNorth * row + xWest * 4]; // northwest
				dataNeighbors[1] = data[yNorth * row + xNorth * 4]; // north
				dataNeighbors[2] = data[yNorth * row + xEast * 4]; // northeast
				dataNeighbors[3] = data[yWest * row + xWest * 4]; // west
				dataNeighbors[4] = data[yPos * row + xPos * 4]; // mid
				dataNeighbors[5] = data[yEast * row + xEast * 4]; // east
				dataNeighbors[6] = data[ySouth * row + xWest * 4]; // southwest
				dataNeighbors[7] = data[ySouth * row + xSouth * 4]; // south
				dataNeighbors[8] = data[ySouth * row + xEast * 4]; // southeast

				WeightedGrads grads = getWeightedGradsAt(dataNeighbors, K_coeff);
				diagCoeff = 1.0 - timeStep * grads.sum();

				double r = data[yPos * row + xPos * 4];
				double rN = data[yNorth * row + xNorth * 4];
				double rS = data[ySouth * row + xSouth * 4];
				double rW = data[yWest * row + xWest * 4];
				double rE = data[yEast * row + xEast * 4];

				double valRed = diagCoeff * r + coeff * sumNeighborPixels(rN, rW, rS, rE, &grads);
				mean_red += valRed;

				// ======== GREEN CHANNEL =======================================
				dataNeighbors[0] = data[yNorth * row + xWest * 4 + 1]; // northwest
				dataNeighbors[1] = data[yNorth * row + xNorth * 4 + 1]; // north
				dataNeighbors[2] = data[yNorth * row + xEast * 4 + 1]; // northeast
				dataNeighbors[3] = data[yWest * row + xWest * 4 + 1]; // west
				dataNeighbors[4] = data[yPos * row + xPos * 4 + 1]; // mid
				dataNeighbors[5] = data[yEast * row + xEast * 4 + 1]; // east
				dataNeighbors[6] = data[ySouth * row + xWest * 4 + 1]; // southwest
				dataNeighbors[7] = data[ySouth * row + xSouth * 4 + 1]; // south
				dataNeighbors[8] = data[ySouth * row + xEast * 4 + 1]; // southeast

				grads = getWeightedGradsAt(dataNeighbors, K_coeff);
				diagCoeff = 1.0 - timeStep * grads.sum();

				double g = data[yPos * row + xPos * 4 + 1];
				double gN = data[yNorth * row + xNorth * 4 + 1];
				double gS = data[ySouth * row + xSouth * 4 + 1];
				double gW = data[yWest * row + xWest * 4 + 1];
				double gE = data[yEast * row + xEast * 4 + 1];

				double valGreen = diagCoeff * g + coeff * sumNeighborPixels(gN, gW, gS, gE, &grads);
				mean_green += valGreen;

				// ======== BLUE CHANNEL =======================================
				dataNeighbors[0] = data[yNorth * row + xWest * 4 + 2]; // northwest
				dataNeighbors[1] = data[yNorth * row + xNorth * 4 + 2]; // north
				dataNeighbors[2] = data[yNorth * row + xEast * 4 + 2]; // northeast
				dataNeighbors[3] = data[yWest * row + xWest * 4 + 2]; // west
				dataNeighbors[4] = data[yPos * row + xPos * 4 + 2]; // mid
				dataNeighbors[5] = data[yEast * row + xEast * 4 + 2]; // east
				dataNeighbors[6] = data[ySouth * row + xWest * 4 + 2]; // southwest
				dataNeighbors[7] = data[ySouth * row + xSouth * 4 + 2]; // south
				dataNeighbors[8] = data[ySouth * row + xEast * 4 + 2]; // southeast

				grads = getWeightedGradsAt(dataNeighbors, K_coeff);
				diagCoeff = 1.0 - timeStep * grads.sum();

				double b = data[yPos * row + xPos * 4 + 2];
				double bN = data[yNorth * row + xNorth * 4 + 2];
				double bS = data[ySouth * row + xSouth * 4 + 2];
				double bW = data[yWest * row + xWest * 4 + 2];
				double bE = data[yEast * row + xEast * 4 + 2];

				double valBlue = diagCoeff * b + coeff * sumNeighborPixels(bN, bW, bS, bE, &grads);
				mean_blue += valBlue;

				// ==============================================================

				_view_w->setPixel(&filteredImg, x, y,
					static_cast<uchar>(std::max(std::min(255, (int)std::round(valRed)), 0)),
					static_cast<uchar>(std::max(std::min(255, (int)std::round(valGreen)), 0)),
					static_cast<uchar>(std::max(std::min(255, (int)std::round(valBlue)), 0))
				);
			}
		}
	}
	if (depth == 8) {
		mean /= (width * height);
		printf("u_mean = %f\n", mean);
	}
	else {
		mean_red /= (width * height);
		mean_green /= (width * height);
		mean_blue /= (width * height);
		printf("u_mean(RGB) = (%f, %f, %f)\n", mean_red, mean_green, mean_blue);
	}

	_view_w->setImage(filteredImg);
	_view_w->update();

	delete[] dataNeighbors;

	return true;
}

bool ImageProcessor::implicitPeronaMalik(float timeStep, double K_coeff, double* uData, double* rhsData, bool printMean, bool printIter)
{
	int heightTotal = _view_w->getImgHeight();
	int widthTotal = _view_w->getImgWidth();

	int height = heightTotal - 2;
	int width = widthTotal - 2;

	int row = _view_w->getImage()->bytesPerLine();
	int depth = _view_w->getImage()->depth();

	int dataSize = _view_w->getImage()->sizeInBytes();

	// SOR params:
	double omega = 1.5, tol = 1e-4;
	// SPD matrix coeffs:
	double diagCoeff = 1.0 + 4.0 * (double)timeStep;
	double coeff = -(double)timeStep;

	QImage::Format format = _view_w->getImage()->format();
	QImage filteredImg = QImage(QSize(width, height), format); // solution
	filteredImg.fill(QColor(0, 0, 0));

	size_t nGrads = width * height * (depth == 8 ? 1 : 3);
	std::vector<WeightedGrads> grads(nGrads);

	int iter = 0, maxIter = 100;

	ImageParams imPar(height, width, depth, row);
	IterParams itPar(timeStep, diagCoeff, coeff, omega, printMean, printIter, K_coeff);

	/*
	xToSave = std::floor((double)width / 4.0);
	yToSave = std::floor((double)height / 4.0);

	outValsRed = std::fstream("outRed.txt", std::fstream::out);
	outValsGreen = std::fstream("outGreen.txt", std::fstream::out);
	outValsBlue = std::fstream("outBlue.txt", std::fstream::out);*/

	do {
		iter++;
		if (printIter) printf("SOR iter %d: ", iter);
		computeDataGrads(uData, &imPar, &grads, K_coeff);
		doSORIter(&imPar, &itPar, uData, rhsData, &grads);

	} while ((iter < maxIter) && (itPar.res > tol));

	/*
	outValsRed.close();
	outValsGreen.close();
	outValsBlue.close();*/

	// write result
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int xPos = x + 1, yPos = y + 1;

			if (depth == 8) {
				int pos = yPos * row + xPos;
				_view_w->setPixel(&filteredImg, x, y, static_cast<uchar>(std::max(std::min(255, (int)std::round(uData[pos])), 0)));
			}
			else {
				uchar r = static_cast<uchar>(std::max(std::min(255, (int)std::round(uData[yPos * row + xPos * 4])), 0));
				uchar g = static_cast<uchar>(std::max(std::min(255, (int)std::round(uData[yPos * row + xPos * 4 + 1])), 0));
				uchar b = static_cast<uchar>(std::max(std::min(255, (int)std::round(uData[yPos * row + xPos * 4 + 2])), 0));

				_view_w->setPixel(&filteredImg, x, y, r, g, b);
			}
		}
	}

	// rhsOld = dataNew
	for (int i = 0; i < dataSize; i++) {
		rhsData[i] = uData[i];
	}

	_view_w->setImage(filteredImg);
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

WeightedGrads ImageProcessor::getWeightedGradsAt(double* dataNeighbors, double K_coeff)
{
	WeightedGrads grads;

	double dataNorthWest = dataNeighbors[0], dataNorth = dataNeighbors[1], dataNorthEast = dataNeighbors[2];
	double dataWest = dataNeighbors[3], dataMid = dataNeighbors[4], dataEast = dataNeighbors[5];
	double dataSouthWest = dataNeighbors[6], dataSouth = dataNeighbors[7], dataSouthEast = dataNeighbors[8];

	double gradX, gradY, normSq;

	gradX = ((dataNorthWest - dataNorthEast) * 0.5 + (dataWest - dataEast) * 0.5) * 0.5;
	gradY = dataNorth - dataMid;
	normSq = gradX * gradX + gradY * gradY;
	grads.north = 1.0 / (1.0 + K_coeff * normSq);

	gradX = dataWest - dataMid;
	gradY = ((dataSouthWest - dataNorthWest) * 0.5 + (dataSouth - dataNorth) * 0.5) * 0.5;
	normSq = gradX * gradX + gradY * gradY;
	grads.west = 1.0 / (1.0 + K_coeff * normSq);

	gradX = dataEast - dataMid;
	gradY = ((dataNorth - dataSouth) * 0.5 + (dataNorthEast - dataSouthEast) * 0.5) * 0.5;
	normSq = gradX * gradX + gradY * gradY;
	grads.east = 1.0 / (1.0 + K_coeff * normSq);

	gradX = ((dataSouth - dataNorth) * 0.5 + (dataSouthEast - dataSouthWest) * 0.5) * 0.5;
	gradY = dataSouth - dataMid;
	normSq = gradX * gradX + gradY * gradY;
	grads.south = 1.0 / (1.0 + K_coeff * normSq);

	return grads;
}

void ImageProcessor::computeDataGrads(double* data, ImageParams* params, std::vector<WeightedGrads>* results, double K_coeff)
{
	int height = params->height;
	int width = params->width;
	int depth = params->depth;
	int row = params->row;
	double* dataNeighbors = new double[9];

	int count = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			// neighboring pixels:
			int xNorth = xPos, yNorth = y;
			int xSouth = xPos, ySouth = yPos + 1;
			int xWest = x, yWest = yPos;
			int xEast = xPos + 1, yEast = yPos;
			// ------
			if (depth == 8) {
				dataNeighbors[0] = data[yNorth * row + xWest]; // northwest
				dataNeighbors[1] = data[yNorth * row + xPos]; // north
				dataNeighbors[2] = data[yNorth * row + xEast]; // northeast
				dataNeighbors[3] = data[yPos * row + xWest]; // west
				dataNeighbors[4] = data[yPos * row + xPos]; // mid
				dataNeighbors[5] = data[yPos * row + xEast]; // east
				dataNeighbors[6] = data[ySouth * row + xWest]; // southwest
				dataNeighbors[7] = data[ySouth * row + xPos]; // south
				dataNeighbors[8] = data[ySouth * row + xEast]; // southeast

				results->at(count++) = getWeightedGradsAt(dataNeighbors, K_coeff);
			}
			else {
				// ======== RED CHANNEL =======================================
				dataNeighbors[0] = data[yNorth * row + xWest * 4]; // northwest
				dataNeighbors[1] = data[yNorth * row + xNorth * 4]; // north
				dataNeighbors[2] = data[yNorth * row + xEast * 4]; // northeast
				dataNeighbors[3] = data[yWest * row + xWest * 4]; // west
				dataNeighbors[4] = data[yPos * row + xPos * 4]; // mid
				dataNeighbors[5] = data[yEast * row + xEast * 4]; // east
				dataNeighbors[6] = data[ySouth * row + xWest * 4]; // southwest
				dataNeighbors[7] = data[ySouth * row + xSouth * 4]; // south
				dataNeighbors[8] = data[ySouth * row + xEast * 4]; // southeast

				results->at(count++) = getWeightedGradsAt(dataNeighbors, K_coeff);

				// ======== GREEN CHANNEL =======================================
				dataNeighbors[0] = data[yNorth * row + xWest * 4 + 1]; // northwest
				dataNeighbors[1] = data[yNorth * row + xNorth * 4 + 1]; // north
				dataNeighbors[2] = data[yNorth * row + xEast * 4 + 1]; // northeast
				dataNeighbors[3] = data[yWest * row + xWest * 4 + 1]; // west
				dataNeighbors[4] = data[yPos * row + xPos * 4 + 1]; // mid
				dataNeighbors[5] = data[yEast * row + xEast * 4 + 1]; // east
				dataNeighbors[6] = data[ySouth * row + xWest * 4 + 1]; // southwest
				dataNeighbors[7] = data[ySouth * row + xSouth * 4 + 1]; // south
				dataNeighbors[8] = data[ySouth * row + xEast * 4 + 1]; // southeast

				results->at(count++) = getWeightedGradsAt(dataNeighbors, K_coeff);

				// ======== BLUE CHANNEL =======================================
				dataNeighbors[0] = data[yNorth * row + xWest * 4 + 2]; // northwest
				dataNeighbors[1] = data[yNorth * row + xNorth * 4 + 2]; // north
				dataNeighbors[2] = data[yNorth * row + xEast * 4 + 2]; // northeast
				dataNeighbors[3] = data[yWest * row + xWest * 4 + 2]; // west
				dataNeighbors[4] = data[yPos * row + xPos * 4 + 2]; // mid
				dataNeighbors[5] = data[yEast * row + xEast * 4 + 2]; // east
				dataNeighbors[6] = data[ySouth * row + xWest * 4 + 2]; // southwest
				dataNeighbors[7] = data[ySouth * row + xSouth * 4 + 2]; // south
				dataNeighbors[8] = data[ySouth * row + xEast * 4 + 2]; // southeast

				results->at(count++) = getWeightedGradsAt(dataNeighbors, K_coeff);
			}
		}
	}
	delete[] dataNeighbors;
}

double ImageProcessor::sumNeighborPixels(double dataNorth, double dataWest, double dataSouth, double dataEast, WeightedGrads* g)
{
	if (!g) return (dataNorth + dataWest + dataSouth + dataEast);

	return (g->north * dataNorth + g->west * dataWest + g->south * dataSouth + g->east * dataEast);
}

void ImageProcessor::doSORIter(ImageParams* imgPar, IterParams* itPar, double* uData, double* rhsData, std::vector<WeightedGrads>* grads)
{
	int height = imgPar->height;
	int width = imgPar->width;
	int depth = imgPar->depth;
	int row = imgPar->row;

	int heightTotal = height + 2, widthTotal = width + 2;

	double coeff = itPar->coeff, diagCoeff = itPar->diagCoeff, omega = itPar->omega;
	bool useGradients = grads != nullptr;

	int count = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			// neighboring pixels:
			int xNorth = xPos, yNorth = y;
			int xSouth = xPos, ySouth = yPos + 1;
			int xWest = x, yWest = yPos;
			int xEast = xPos + 1, yEast = yPos;

			if (depth == 8) {
				int pos = yPos * row + xPos;
				
				if (useGradients) {
					diagCoeff = 1.0 + itPar->tau * grads->at(count).sum();
				}

				double sumNeighbors = coeff * sumNeighborPixels(
					uData[yNorth * row + xNorth], uData[yWest * row + xWest],
					uData[ySouth * row + xSouth], uData[yEast * row + xEast], (useGradients ? &grads->at(count) : nullptr));
				count++;

				double val = (omega / diagCoeff) * (rhsData[pos] - sumNeighbors) + (1.0 - omega) * uData[pos];
				uData[pos] = val;
			}
			else {
				int pos_red = yPos * row + xPos * 4;
				int pos_green = yPos * row + xPos * 4 + 1;
				int pos_blue = yPos * row + xPos * 4 + 2;

				// ============= RED CHANNEL =======================================
				if (useGradients) {
					diagCoeff = 1.0 + itPar->tau * grads->at(count).sum();
				}

				double sumNeighbors_red = coeff * sumNeighborPixels(
					uData[yNorth * row + xNorth * 4], uData[yWest * row + xWest * 4],
					uData[ySouth * row + xSouth * 4], uData[yEast * row + xEast * 4], (useGradients ? &grads->at(count) : nullptr));
				count++;

				// ============== GREEN CHANNEL ==================================
				if (useGradients) {
					diagCoeff = 1.0 + itPar->tau * grads->at(count).sum();
				}

				double sumNeighbors_green = coeff * sumNeighborPixels(
					uData[yNorth * row + xNorth * 4 + 1], uData[yWest * row + xWest * 4 + 1],
					uData[ySouth * row + xSouth * 4 + 1], uData[yEast * row + xEast * 4 + 1], (useGradients ? &grads->at(count) : nullptr));
				count++;

				// ============= BLUE CHANNEL =====================================
				if (useGradients) {
					diagCoeff = 1.0 + itPar->tau * grads->at(count).sum();
				}

				double sumNeighbors_blue = coeff * sumNeighborPixels(
					uData[yNorth * row + xNorth * 4 + 2], uData[yWest * row + xWest * 4 + 2],
					uData[ySouth * row + xSouth * 4 + 2], uData[yEast * row + xEast * 4 + 2], (useGradients ? &grads->at(count) : nullptr));
				count++;

				double val_red = (omega / diagCoeff) * (rhsData[pos_red] - sumNeighbors_red) + (1.0 - omega) * uData[pos_red];
				double val_green = (omega / diagCoeff) * (rhsData[pos_green] - sumNeighbors_green) + (1.0 - omega) * uData[pos_green];
				double val_blue = (omega / diagCoeff) * (rhsData[pos_blue] - sumNeighbors_blue) + (1.0 - omega) * uData[pos_blue];

				/*
				if (xPos == xToSave && yPos == yToSave) {
					outValsRed << val_red << std::endl;
					outValsGreen << val_green << std::endl;
					outValsBlue << val_blue << std::endl;
				}*/

				uData[pos_red] = val_red;
				uData[pos_green] = val_green;
				uData[pos_blue] = val_blue;
			}
		}
	}

	// copy extended
	double mean = 0.0, mean_red = 0.0, mean_green = 0.0, mean_blue = 0.0;
	for (int y = 0; y < heightTotal; y++) {
		for (int x = 0; x < widthTotal; x++) {
			int xPos = (x == 0 ? 1 : (x == widthTotal - 1 ? width : x));
			int yPos = (y == 0 ? 1 : (y == heightTotal - 1 ? height : y));

			if (depth == 8) {
				uData[y * row + x] = uData[yPos * row + xPos];
				mean += uData[yPos * row + xPos];
			}
			else {
				for (int j = 0; j < 3; j++) uData[y * row + x * 4 + j] = uData[yPos * row + xPos * 4 + j];
				mean_red += uData[yPos * row + xPos * 4];
				mean_green += uData[yPos * row + xPos * 4 + 1];
				mean_blue += uData[yPos * row + xPos * 4 + 2];
			}
		}
	}
	if (itPar->printMean && depth == 8) {
		mean /= (widthTotal * heightTotal);
		printf("u_mean = %lf, ", mean);
	}
	else if (itPar->printMean) {
		mean_red /= (widthTotal * heightTotal);
		mean_green /= (widthTotal * heightTotal);
		mean_blue /= (widthTotal * heightTotal);
		printf("u_mean(RGB) = (%lf, %lf, %lf),", mean_red, mean_green, mean_blue);
	}

	// compute residuum
	if (depth == 8) {
		itPar->res = 0.0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				// current pixel:
				int xPos = x + 1, yPos = y + 1;
				int pos = yPos * row + xPos;
				// neighboring pixels:
				int xNorth = xPos, yNorth = y;
				int xSouth = xPos, ySouth = yPos + 1;
				int xWest = x, yWest = yPos;
				int xEast = xPos + 1, yEast = yPos;

				if (useGradients) {
					diagCoeff = 1.0 + itPar->tau * grads->at(y * width + x).sum();
				}

				// (A * u - b)_i
				double sum = diagCoeff * uData[pos] +
					coeff * sumNeighborPixels(
						uData[yNorth * row + xNorth], uData[yWest * row + xWest], 
						uData[ySouth * row + xSouth], uData[yEast * row + xEast], (useGradients ? &grads->at(y * width + x) : nullptr)) -
					rhsData[pos];
				itPar->res += sum * sum;
			}
		}
		// not counting extended pixels, since they do not change
		itPar->res = sqrt(itPar->res);
		if (itPar->printIter) printf("res = %lf\n", itPar->res);
	}
	else {
		double res_red = 0.0, res_green = 0.0, res_blue = 0.0;
		count = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				// current pixel:
				int xPos = x + 1, yPos = y + 1;
				int pos_red = yPos * row + xPos * 4;
				int pos_green = yPos * row + xPos * 4 + 1;
				int pos_blue = yPos * row + xPos * 4 + 2;
				// neighboring pixels:
				int xNorth = xPos, yNorth = y;
				int xSouth = xPos, ySouth = yPos + 1;
				int xWest = x, yWest = yPos;
				int xEast = xPos + 1, yEast = yPos;

				// ============= RED CHANNEL =======================================
				if (useGradients) {
					diagCoeff = 1.0 + itPar->tau * grads->at(count).sum();
				}

				// (A * u - b)_i
				double sum_red = diagCoeff * uData[pos_red] +
					coeff * sumNeighborPixels(
						uData[yNorth * row + xNorth * 4], uData[yWest * row + xWest * 4],
						uData[ySouth * row + xSouth * 4], uData[yEast * row + xEast * 4], (useGradients ? &grads->at(count) : nullptr)) -
					rhsData[pos_red];
				count++;

				// ============== GREEN CHANNEL ==================================
				if (useGradients) {
					diagCoeff = 1.0 + itPar->tau * grads->at(count).sum();
				}

				double sum_green = diagCoeff * uData[pos_green] +
					coeff * sumNeighborPixels(
						uData[yNorth * row + xNorth * 4 + 1], uData[yWest * row + xWest * 4 + 1],
						uData[ySouth * row + xSouth * 4 + 1], uData[yEast * row + xEast * 4 + 1], (useGradients ? &grads->at(count) : nullptr)) -
					rhsData[pos_green];
				count++;

				// ============= BLUE CHANNEL =====================================
				if (useGradients) {
					diagCoeff = 1.0 + itPar->tau * grads->at(count).sum();
				}

				double sum_blue = diagCoeff * uData[pos_blue] +
					coeff * sumNeighborPixels(
						uData[yNorth * row + xNorth * 4 + 2], uData[yWest * row + xWest * 4 + 2],
						uData[ySouth * row + xSouth * 4 + 2], uData[yEast * row + xEast * 4 + 2], (useGradients ? &grads->at(count) : nullptr)) -
					rhsData[pos_blue];
				count++;

				res_red += sum_red * sum_red;
				res_green += sum_green * sum_green;
				res_blue += sum_blue * sum_blue;
			}
		}
		// not counting extended pixels, since they do not change
		res_red = sqrt(res_red);
		res_green = sqrt(res_green);
		res_blue = sqrt(res_blue);

		printf("res(RGB) = (%lf, %lf, %lf)\n", res_red, res_green, res_blue);
		itPar->res = std::max(res_red, std::max(res_green, res_blue)); // use the largest res for iter control
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

ImageParams::ImageParams(int height, int width, int depth, int row)
{
	this->height = height; this->width = width; this->depth = depth; this->row = row;
}

IterParams::IterParams(
	double tau, double diagCoeff, double coeff, double omega, 
	bool printMean, bool printIter, double K_coeff)
{
	this->tau = tau; this->diagCoeff = diagCoeff; this->coeff = coeff; this->omega = omega;
	this->printMean = printMean; this->printIter = printIter;
	this->K_coeff = K_coeff;
}
