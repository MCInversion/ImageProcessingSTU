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
	double timeStep = heqDialog->getTimeStep();
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

	ImageParams params = _view_w->getImageParams();
	int dataSize = params.dataSize;
	uchar* data = _view_w->getData();

	double* uData = new double[dataSize]; // raw img data for each step
	for (int i = 0; i < dataSize; i++) 	uData[i] = (double)data[i];

	double* rhsData;
	if (scheme > 0) {
		rhsData = new double[dataSize];
		for (int i = 0; i < dataSize; i++) rhsData[i] = uData[i];
	}

	for (int i = 1; i <= nSteps; i++) {
		printf("<=== heatEquation step %d...\n", i);
		if (scheme == 0) {
			explicitHeatEquation(timeStep, uData, &params);
		}
		else {
			implicitHeatEquation(timeStep, uData, rhsData, &params);
		}
		writeDataToImageAndUpdate(uData, &params, i);
		printf("=== heatEquation step %d complete ===>\n", i);
	}

	delete[] uData;
	if (scheme > 0)	delete[] rhsData;

	emit multiImageComplete();
}

void ImageProcessor::peronaMalikAccepted()
{
	PeronaMalikDialog* pmDialog = static_cast<PeronaMalikDialog*>(sender());
	double timeStep = pmDialog->getTimeStep();
	int scheme = pmDialog->getScheme();
	double K_coeff = pmDialog->getKCoeff();
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

	ImageParams params = _view_w->getImageParams();
	const int dataSize = params.dataSize;
	uchar* data = _view_w->getData();
	double* uData = new double[dataSize]; // raw img data for each step
	double* uSigmaData = new double[dataSize]; // pre-smoothed data
	for (int i = 0; i < dataSize; i++) {
		uData[i] = (double)data[i]; uSigmaData[i] = uData[i];
	}
	double* rhsData;
	if (scheme > 0) {	// save double px values from previous time step
		rhsData = new double[dataSize];
		for (int i = 0; i < dataSize; i++) rhsData[i] = uData[i];
	}

	for (int i = 1; i <= nSteps; i++) {
		printf("<=== Perona-Malik Equation step %d...\n", i);
		if (scheme == 0) {
			printf("... heat step: \n");
			explicitHeatEquation(timeStep, uSigmaData, &params);
			printf("... heat step complete. Perona-Malik step: \n");
			explicitPeronaMalik(timeStep, K_coeff, uData, uSigmaData, &params);
			printf("... Perona-Malik step complete\n");
		}
		else {
			printf("... heat step: \n");
			implicitHeatEquation(std::min(timeStep, 0.1), uSigmaData, rhsData, &params, false, false);
			// explicitHeatEquation(timeStep, uSigmaData, &params);
			printf("... heat step complete. Perona-Malik step: \n");
			size_t nGs = (params.width - 2) * (params.height - 2) * (params.depth == 8 ? 1 : 3);
			std::vector<EdgeDiffusionCoeffs> G(nGs);
			computeAllEdgeDiffusionCoeffs(uData, uSigmaData, &params, &G, K_coeff);
			semiImplicitPeronaMalik(timeStep, K_coeff, uData, rhsData, &G, &params);
			printf("... Perona-Malik step complete\n");
		}
		writeDataToImageAndUpdate(uData, &params, i);
		printf("=== Perona-Malik Equation step %d complete ===>\n", i);
	}

	if (scheme > 0)	delete[] rhsData;
	delete[] uData;

	emit multiImageComplete();
}

void ImageProcessor::curvatureFlowAccepted()
{
	CurvatureFilterDialog* cfDialog = static_cast<CurvatureFilterDialog*>(sender());
	double timeStep = cfDialog->getTimeStep();
	int scheme = cfDialog->getScheme();
	double K_coeff = cfDialog->getKCoeff();
	double epsilon = cfDialog->getEpsilon();
	nSteps = cfDialog->getNSteps();

	// time step array prep:
	_view_w->clearImages();
	_view_w->allocateImages(nSteps + 1);

	// mirror extend first img
	QImage img = QImage(*_view_w->getImage());
	_view_w->setImageAt(img, 0);
	QImage extended = QImage(img);
	_view_w->setImage(extended);
	mirrorExtendImageBy(1);

	ImageParams params = _view_w->getImageParams();
	const int dataSize = params.dataSize;
	uchar* data = _view_w->getData();
	double* uData = new double[dataSize]; // raw img data for each step
	double* uSigmaData = new double[dataSize]; // pre-smoothed data (GMCF)
	double* rhsData = new double[dataSize];
	for (int i = 0; i < dataSize; i++) {
		uData[i] = (double)data[i];
		uSigmaData[i] = uData[i];
		rhsData[i] = uData[i];
	}
	
	size_t nGs = (params.width - 2) * (params.height - 2) * (params.depth == 8 ? 1 : 3);

	for (int i = 1; i <= nSteps; i++) {
		printf("<=== %s Mean Curvature Flow step %d...\n", (scheme > 0 ? "Geodesic" : ""), i);
		
		if (scheme == 0) {
			printf("... MCF step: \n");

			std::vector<EdgeDiffusionCoeffs> grads(nGs);
			computeAllEdgeRegularizedGradientCoeffs(uData, &params, &grads, epsilon);

			meanCurvatureFlow(timeStep, uData, rhsData, &grads, &params);

			printf("... MCF step complete\n");
		}
		else {
			printf("... heat step: \n");
			implicitHeatEquation(std::min(timeStep, 0.1), uSigmaData, rhsData, &params, false, false);
			// explicitHeatEquation(timeStep, uSigmaData, &params);
			printf("... heat step complete. GMCF step: \n");

			std::vector<EdgeDiffusionCoeffs> grads(nGs);
			std::vector<EdgeDiffusionCoeffs> gCoeffs(nGs);

			computeAllEdgeRegularizedGradientCoeffs(uData, &params, &grads, epsilon);
			computeAllEdgeDiffusionCoeffs(uData, uSigmaData, &params, &gCoeffs, K_coeff);

			geodesicMeanCurvatureFlow(timeStep, K_coeff, uData, rhsData, &gCoeffs, &grads, &params);

			printf("... GMCF step complete\n");
		}
		writeDataToImageAndUpdate(uData, &params, i);
		printf("=== %s Mean Curvature Flow step %d complete ===>\n", (scheme > 0 ? "Geodesic" : ""), i);
	}

	delete[] rhsData;
	delete[] uSigmaData;
	delete[] uData;

	emit multiImageComplete();
}

void ImageProcessor::threshDistanceAccepted()
{
	// time step array prep:
	nSteps = 2;
	_view_w->clearImages();
	_view_w->allocateImages(3);
	QImage img = QImage(*_view_w->getImage());
	_view_w->setImageAt(img, 0);

	// isoData
	HistogramWindow* hist = new HistogramWindow();
	hist->setImage(_view_w->getImage());
	connect(hist, SIGNAL(sigGrayscale()), this, SLOT(on_histogramGrayscale()));
	connect(this, SIGNAL(sigGrayscaled()), hist, SLOT(ActionIsodataCompute()));
	connect(hist, SIGNAL(sigThreshold()), this, SLOT(on_singleThreshold()));
	hist->ActionIsodata();
	QImage thresholdImg = QImage(*_view_w->getImage());
	_view_w->setImageAt(thresholdImg, 1);

	// prepare data
	QImage extended = QImage(thresholdImg);
	_view_w->setImage(extended);
	mirrorExtendImageBy(1);
	ImageParams params = _view_w->getImageParams();
	const int dataSize = params.dataSize;
	uchar* data = _view_w->getData();
	double* uData = new double[dataSize]; // raw img data for each step
	double* distData = new double[dataSize]; // SDF data
	bool* frozenCells = new bool[dataSize]; // SDF cell status
	for (int i = 0; i < dataSize; i++) {
		uData[i] = (double)data[i];
		distData[i] = DBL_MAX; // large val for fastSweep2D
		frozenCells[i] = false;
	}

	setZeroLevelCurveFromIsoData(uData, distData, frozenCells, &params);
	fastSweep2D(distData, frozenCells, &params);
	writeDistanceDataToImageAndUpdate(distData, frozenCells, &params, 2);

	delete[] distData;
	delete[] frozenCells;
	delete[] uData;

	emit multiImageComplete();
}

void ImageProcessor::threshSignedDistanceAccepted()
{
	// time step array prep:
	nSteps = 2;
	_view_w->clearImages();
	_view_w->allocateImages(3);
	QImage img = QImage(*_view_w->getImage());
	_view_w->setImageAt(img, 0);

	// isoData
	HistogramWindow* hist = new HistogramWindow();
	hist->setImage(_view_w->getImage());
	connect(hist, SIGNAL(sigGrayscale()), this, SLOT(on_histogramGrayscale()));
	connect(this, SIGNAL(sigGrayscaled()), hist, SLOT(ActionIsodataCompute()));
	connect(hist, SIGNAL(sigThreshold()), this, SLOT(on_singleThreshold()));
	hist->ActionIsodata();
	QImage thresholdImg = QImage(*_view_w->getImage());
	_view_w->setImageAt(thresholdImg, 1);

	// prepare data
	QImage extended = QImage(thresholdImg);
	_view_w->setImage(extended);
	mirrorExtendImageBy(1);
	ImageParams params = _view_w->getImageParams();
	const int dataSize = params.dataSize;
	uchar* data = _view_w->getData();
	double* uData = new double[dataSize]; // raw img data for each step
	double* distData = new double[dataSize]; // SDF data
	bool* contourCells = new bool[dataSize]; // contour data
	bool* frozenCells = new bool[dataSize]; // SDF cell status
	for (int i = 0; i < dataSize; i++) {
		uData[i] = (double)data[i];
		distData[i] = DBL_MAX; // large val for fastSweep2D
		frozenCells[i] = false;
	}

	setZeroLevelCurveFromIsoData(uData, distData, frozenCells, &params);
	for (int i = 0; i < dataSize; i++) contourCells[i] = frozenCells[i];

	fastSweep2D(distData, frozenCells, &params);
	computeSignFloodFill(distData, frozenCells, &params);
	// computeSignFromIsoData(distData, data, &params);

	writeDistanceDataToImageAndUpdate(distData, contourCells, &params, 2);

	delete[] distData;
	delete[] frozenCells;
	delete[] contourCells;
	delete[] uData;

	emit multiImageComplete();
}

void ImageProcessor::subsurfAccepted()
{
	SegmentDialog* sDialog = static_cast<SegmentDialog*>(sender());
	double uMin = sDialog->getUMin(), uMax = sDialog->getUMax();
	double timeStep = sDialog->getTimeStep();
	double K_coeff = sDialog->getKCoeff();
	double epsilon = sDialog->getEpsilon();
	nSteps = sDialog->getNSteps();

	bool addThresholdedToList = sDialog->addIsoDataImage();

	// time step array prep:
	_view_w->clearImages();
	_view_w->allocateImages(nSteps + 1 + (int)addThresholdedToList);

	// grayscale original
	grayscale();
	QImage img = QImage(*_view_w->getImage());
	_view_w->setImageAt(img, 0);

	// prepare data
	QImage extended = QImage(*_view_w->getImage());
	_view_w->setImage(extended);
	mirrorExtendImageBy(1);
	ImageParams params = _view_w->getImageParams();
	const int dataSize = params.dataSize;

	uchar* origImgData = _view_w->getData();
	double* iData = new double[dataSize];
	for (int i = 0; i < dataSize; i++) {
		iData[i] = (double)origImgData[i] / 255.0; // [0, 255] switch to [0, 1] ?
	}
	explicitHeatEquation(0.2, iData, &params); // img data smoothing

	// isoData
	HistogramWindow* hist = new HistogramWindow();
	hist->setImage(_view_w->getImage());
	connect(hist, SIGNAL(sigGrayscale()), this, SLOT(on_histogramGrayscale()));
	connect(this, SIGNAL(sigGrayscaled()), hist, SLOT(ActionIsodataCompute()));
	connect(hist, SIGNAL(sigThreshold()), this, SLOT(on_singleThreshold()));
	hist->ActionIsodata();

	uchar* threshData = _view_w->getData();
	double* uData = new double[dataSize]; // raw img data for each step
	double* rhsData = new double[dataSize];
	for (int i = 0; i < dataSize; i++) {
		uData[i] = ((double)threshData[i] / 255.0) * (uMax - uMin) + uMin; // rescale to [uMin, uMax]
		rhsData[i] = uData[i];
	}

	int startId = 1;
	if (addThresholdedToList) {
		writeScalarDataToImageAndUpdate(uData, uMin, uMax, &params, startId);
		startId++;
	}
	
	size_t nGs = (params.width - 2) * (params.height - 2) * (params.depth == 8 ? 1 : 3);
	std::vector<EdgeDiffusionCoeffs> gCoeffs(nGs);
	computeAllEdgeDiffusionCoeffs(iData, iData, &params, &gCoeffs, K_coeff);

	for (int i = startId; i <= startId + nSteps; i++) {
		printf("SUBSURF step : %d ...\n", i - (int)addThresholdedToList);
		std::vector<EdgeDiffusionCoeffs> grads(nGs);

		computeAllEdgeRegularizedGradientCoeffs(uData, &params, &grads, epsilon);

		geodesicMeanCurvatureFlow(timeStep, K_coeff, uData, rhsData, &gCoeffs, &grads, &params);

		writeScalarDataToImageAndUpdate(uData, uMin, uMax, &params, i);
		printf("... SUBSURF step : %d complete\n", i - (int)addThresholdedToList);
	}

	delete[] rhsData;
	delete[] uData;
	delete[] iData;

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
	if (threshold < 0) {
		_view_w->getImage()->fill(QColor(0, 0, 0));
	}

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

void ImageProcessor::writeDataToImageAndUpdate(double* uData, ImageParams* params, int i)
{
	QImage newImg = QImage(QSize(params->width - 2, params->height - 2), params->format);
	newImg.fill(QColor(0, 0, 0));

	for (int y = 0; y < params->height - 2; y++) {
		for (int x = 0; x < params->width - 2; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			if (params->depth == 8) {
				_view_w->setPixel(&newImg, x, y, uData[yPos * params->row + xPos] / 255.);
			} else {
				_view_w->setPixel(&newImg, x, y,
					uData[yPos * params->row + 4 * xPos] / 255.,
					uData[yPos * params->row + 4 * xPos + 1] / 255.,
					uData[yPos * params->row + 4 * xPos + 2] / 255.
				);
			}
		}
	}
	_view_w->setImageAt(newImg, i);
	_view_w->setImage(newImg);
	_view_w->update();
}

void ImageProcessor::writeDistanceDataToImageAndUpdate(double* dData, bool* frozenCells, ImageParams* params, int i, bool drawContour)
{
	if (params->depth > 8) {
		printf("Error, attempting to write scalar data to multi-channel image!\n");
		return;
	}

	QImage newImg = QImage(QSize(params->width - 2, params->height - 2), params->format);
	newImg.fill(QColor(0, 0, 0));

	double minVal = 256.0, maxVal = -1.0;
	for (int y = 0; y < params->height - 2; y++) {
		for (int x = 0; x < params->width - 2; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			minVal = (dData[yPos * params->row + xPos] < minVal ? dData[yPos * params->row + xPos] : minVal);
			maxVal = (dData[yPos * params->row + xPos] > maxVal ? dData[yPos * params->row + xPos] : maxVal);
		}
	}
	double dataRange = maxVal - minVal;

	for (int y = 0; y < params->height - 2; y++) {
		for (int x = 0; x < params->width - 2; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			if (drawContour && frozenCells[yPos * params->row + xPos]) {
				_view_w->setPixel(&newImg, x, y, 1.0);
				continue;
			}

			_view_w->setPixel(&newImg, x, y, (dData[yPos * params->row + xPos] - minVal) / dataRange);
		}
	}
	_view_w->setImageAt(newImg, i);
	_view_w->setImage(newImg);
	_view_w->update();
}

void ImageProcessor::writeScalarDataToImageAndUpdate(double* data, double dataMin, double dataMax, ImageParams* params, int i)
{
	if (params->depth > 8) {
		printf("Error, attempting to write scalar data to multi-channel image!\n");
		return;
	}

	double dataRange = dataMax - dataMin;
	QImage newImg = QImage(QSize(params->width - 2, params->height - 2), params->format);
	newImg.fill(QColor(0, 0, 0));

	for (int y = 0; y < params->height - 2; y++) {
		for (int x = 0; x < params->width - 2; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;

			double val = (data[yPos * params->row + xPos] - dataMin) / dataRange;
			_view_w->setPixel(&newImg, x, y, val);
		}
	}
	_view_w->setImageAt(newImg, i);
	_view_w->setImage(newImg);
	_view_w->update();
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

bool ImageProcessor::explicitHeatEquation(double timeStep, double* uData, ImageParams* params, bool printMean)
{
	int height = params->height;
	int width = params->width;
	int row = params->row;
	int depth = params->depth;

	double diagCoeff = 1.0f - 4.0f * timeStep;
	double coeff = timeStep;

	double* newData = new double[params->dataSize];

	double mean = 0.0, mean_red = 0.0, mean_green = 0.0, mean_blue = 0.0;
	for (int x = 0; x < width - 2; x++) {
		for (int y = 0; y < height - 2; y++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			// neighboring pixels:
			int xNorth = xPos, yNorth = y;
			int xSouth = xPos, ySouth = yPos + 1;
			int xWest = x, yWest = yPos;
			int xEast = xPos + 1, yEast = yPos;
			// ------
			if (depth == 8) {
				double val = diagCoeff * uData[yPos * row + xPos] +
					coeff * (uData[yNorth * row + xNorth] + uData[ySouth * row + xSouth] + uData[yWest * row + xWest] + uData[yEast * row + xEast]);
				newData[yPos * row + xPos] = val;
			}
			else {
				double r = uData[yPos * row + xPos * 4];
				double rN = uData[yNorth * row + xNorth * 4];
				double rS = uData[ySouth * row + xSouth * 4];
				double rW = uData[yWest * row + xWest * 4];
				double rE = uData[yEast * row + xEast * 4];

				double valRed = diagCoeff * r + coeff * (rN + rS + rW + rE);
				newData[yPos * row + xPos * 4] = valRed;

				double g = uData[yPos * row + xPos * 4 + 1];
				double gN = uData[yNorth * row + xNorth * 4 + 1];
				double gS = uData[ySouth * row + xSouth * 4 + 1];
				double gW = uData[yWest * row + xWest * 4 + 1];
				double gE = uData[yEast * row + xEast * 4 + 1];

				double valGreen = diagCoeff * g + coeff * (gN + gS + gW + gE);
				newData[yPos * row + xPos * 4 + 1] = valGreen;

				double b = uData[yPos * row + xPos * 4 + 2];
				double bN = uData[yNorth * row + xNorth * 4 + 2];
				double bS = uData[ySouth * row + xSouth * 4 + 2];
				double bW = uData[yWest * row + xWest * 4 + 2];
				double bE = uData[yEast * row + xEast * 4 + 2];

				double valBlue = diagCoeff * b + coeff * (bN + bS + bW + bE);
				newData[yPos * row + xPos * 4 + 2] = valBlue;
			}
		}
	}

	// copy extended & compute mean
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int xPos = (x == 0 ? 1 : (x == width - 1 ? width - 2 : x));
			int yPos = (y == 0 ? 1 : (y == height- 1 ? height - 2 : y));

			if (depth == 8) {
				uData[y * row + x] = newData[yPos * row + xPos];
				mean += newData[yPos * row + xPos];
			}
			else {
				for (int j = 0; j < 3; j++) uData[y * row + x * 4 + j] = newData[yPos * row + xPos * 4 + j];
				mean_red += uData[yPos * row + xPos * 4];
				mean_green += uData[yPos * row + xPos * 4 + 1];
				mean_blue += uData[yPos * row + xPos * 4 + 2];
			}
		}
	}
	if (printMean) {
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
	}

	delete[] newData;

	return true;
}

bool ImageProcessor::implicitHeatEquation(double timeStep, double* uData, double* rhsData, ImageParams* params, bool printMean, bool printIter)
{
	int height = params->height;
	int width = params->width;

	int row = params->row;
	int depth = params->depth;

	int dataSize = params->dataSize;

	// old and new img data (old and new iter)
	double* iterData = new double[dataSize];
	for (int i = 0; i < dataSize; i++) iterData[i] = 0.0;

	// SOR params:
	double omega = 1.5, tol = 1e-4;
	// SPD matrix coeffs:
	double diagCoeff = 1.0 + 4.0 * timeStep;
	double coeff = -timeStep;

	int iter = 0, maxIter = 100;

	IterParams itPar(timeStep, diagCoeff, coeff, omega, printMean, printIter);

	do {
		iter++;
		if (printIter) printf("SOR iter %d: ", iter);		
		doSORIter(params, &itPar, uData, rhsData);

	} while ((iter < maxIter) && (itPar.res > tol));

	// rhsOld = dataNew
	for (int i = 0; i < dataSize; i++) rhsData[i] = uData[i];
	return true;
}

bool ImageProcessor::explicitPeronaMalik(double timeStep, double K_coeff, double* uData, double* uSigmaData, ImageParams* params)
{
	int height = params->height;
	int width = params->width;
	int row = params->row;
	int depth = params->depth;

	double diagCoeff;
	double coeff = timeStep;
	double* dataNeighbors = new double[9];
	double* smoothedDataNeighbors = new double[9];

	double* newData = new double[params->dataSize];

	for (int y = 0; y < height - 2; y++) {
		for (int x = 0; x < width - 2; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			// neighboring pixel coords:
			int xNorth = xPos, yNorth = y;
			int xSouth = xPos, ySouth = yPos + 1;
			int xWest = x, yWest = yPos;
			int xEast = xPos + 1, yEast = yPos;
			// ------
			if (depth == 8) {
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row);
				getDataNeighbors(smoothedDataNeighbors, uSigmaData, xPos, yPos, row);

				EdgeDiffusionCoeffs G = getEdgeDiffusionWeights(dataNeighbors, smoothedDataNeighbors, K_coeff);
				diagCoeff = 1.0 - timeStep * G.sum();

				double val = diagCoeff * uData[yPos * row + xPos] +
					coeff * sumNeighborPixels(
						uData[yNorth * row + xNorth], uData[yWest * row + xWest],
						uData[ySouth * row + xSouth], uData[yEast * row + xEast], &G);
				newData[yPos * row + xPos] = val;
			}
			else {
				// ======== RED CHANNEL =======================================
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row, 0);
				getDataNeighbors(smoothedDataNeighbors, uSigmaData, xPos, yPos, row, 0);

				EdgeDiffusionCoeffs G = getEdgeDiffusionWeights(dataNeighbors, smoothedDataNeighbors, K_coeff);
				diagCoeff = 1.0 - timeStep * G.sum();

				double r = uData[yPos * row + xPos * 4];
				double rN = uData[yNorth * row + xNorth * 4];
				double rS = uData[ySouth * row + xSouth * 4];
				double rW = uData[yWest * row + xWest * 4];
				double rE = uData[yEast * row + xEast * 4];

				double valRed = diagCoeff * r + coeff * sumNeighborPixels(rN, rW, rS, rE, &G);
				newData[yPos * row + xPos * 4] = valRed;

				// ======== GREEN CHANNEL =======================================
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row, 1);
				getDataNeighbors(smoothedDataNeighbors, uSigmaData, xPos, yPos, row, 1);

				G = getEdgeDiffusionWeights(dataNeighbors, smoothedDataNeighbors, K_coeff);
				diagCoeff = 1.0 - timeStep * G.sum();

				double g = uData[yPos * row + xPos * 4 + 1];
				double gN = uData[yNorth * row + xNorth * 4 + 1];
				double gS = uData[ySouth * row + xSouth * 4 + 1];
				double gW = uData[yWest * row + xWest * 4 + 1];
				double gE = uData[yEast * row + xEast * 4 + 1];

				double valGreen = diagCoeff * g + coeff * sumNeighborPixels(gN, gW, gS, gE, &G);
				newData[yPos * row + xPos * 4 + 1] = valGreen;

				// ======== BLUE CHANNEL =======================================
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row, 2);
				getDataNeighbors(smoothedDataNeighbors, uSigmaData, xPos, yPos, row, 2);

				G = getEdgeDiffusionWeights(dataNeighbors, smoothedDataNeighbors, K_coeff);
				diagCoeff = 1.0 - timeStep * G.sum();

				double b = uData[yPos * row + xPos * 4 + 2];
				double bN = uData[yNorth * row + xNorth * 4 + 2];
				double bS = uData[ySouth * row + xSouth * 4 + 2];
				double bW = uData[yWest * row + xWest * 4 + 2];
				double bE = uData[yEast * row + xEast * 4 + 2];

				double valBlue = diagCoeff * b + coeff * sumNeighborPixels(bN, bW, bS, bE, &G);
				newData[yPos * row + xPos * 4 + 2] = valBlue;
				// ==============================================================
			}
		}
	}
	// copy extended & compute mean
	double mean = 0.0, mean_red = 0.0, mean_green = 0.0, mean_blue = 0.0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int xPos = (x == 0 ? 1 : (x == width - 1 ? width - 2 : x));
			int yPos = (y == 0 ? 1 : (y == height - 1 ? height - 2 : y));

			if (depth == 8) {
				uData[y * row + x] = newData[yPos * row + xPos];
				mean += newData[yPos * row + xPos];
			}
			else {
				for (int j = 0; j < 3; j++) uData[y * row + x * 4 + j] = newData[yPos * row + xPos * 4 + j];
				mean_red += newData[yPos * row + xPos * 4];
				mean_green += newData[yPos * row + xPos * 4 + 1];
				mean_blue += newData[yPos * row + xPos * 4 + 2];
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
	delete[] newData;
	delete[] smoothedDataNeighbors;
	delete[] dataNeighbors;

	return true;
}

bool ImageProcessor::semiImplicitPeronaMalik(
	double timeStep, double K_coeff, double* uData, double* rhsData, std::vector<EdgeDiffusionCoeffs>* G,
	ImageParams* params, bool printMean, bool printIter)
{
	int height = params->height;
	int width = params->width;

	int row = params->row;
	int depth = params->depth;

	int dataSize = params->dataSize;

	// SOR params:
	double omega = 1.5, tol = 1e-4;
	// SPD matrix coeffs:
	double diagCoeff = 1.0 + 4.0 * timeStep;
	double coeff = -timeStep;

	int iter = 0, maxIter = 100;

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
		doSORIter(params, &itPar, uData, rhsData, G);

	} while ((iter < maxIter) && (itPar.res > tol));

	/*
	outValsRed.close();
	outValsGreen.close();
	outValsBlue.close();*/

	// rhsOld = dataNew
	for (int i = 0; i < dataSize; i++) {
		rhsData[i] = uData[i];
	}
	return true;
}

bool ImageProcessor::meanCurvatureFlow(
	double timeStep, double* uData, double* rhsData,
	std::vector<EdgeDiffusionCoeffs>* G, ImageParams* params,
	bool printMean, bool printIter)
{
	int height = params->height;
	int width = params->width;

	int row = params->row;
	int depth = params->depth;

	int dataSize = params->dataSize;

	// SOR params:
	double omega = 1.5, tol = 1e-3;
	// SPD matrix coeffs:
	double diagCoeff = 1.0 + 4.0 * timeStep;
	double coeff = -timeStep;

	int iter = 0, maxIter = 100;

	IterParams itPar(timeStep, diagCoeff, coeff, omega, printMean, printIter, 0.0);

	do {
		iter++;
		printf("SOR iter %d: ", iter);
		doSORIter(params, &itPar, uData, rhsData, nullptr, G);

	} while ((iter < maxIter) && (itPar.res > tol));


	// rhsOld = dataNew
	for (int i = 0; i < dataSize; i++) {
		rhsData[i] = uData[i];
	}
	return true;

	return false;
}

bool ImageProcessor::geodesicMeanCurvatureFlow(
	double timeStep, double K_coeff, double* uData, double* rhsData,
	std::vector<EdgeDiffusionCoeffs>* gCoeffs, std::vector<EdgeDiffusionCoeffs>* grads,
	ImageParams* params, bool printMean, bool printIter)
{
	int height = params->height;
	int width = params->width;

	int row = params->row;
	int depth = params->depth;

	int dataSize = params->dataSize;

	// SOR params:
	double omega = 1.5, tol = 1e-3;
	// SPD matrix coeffs:
	double diagCoeff = 1.0 + 4.0 * timeStep;
	double coeff = -timeStep;

	int iter = 0, maxIter = 100;

	IterParams itPar(timeStep, diagCoeff, coeff, omega, printMean, printIter, K_coeff);

	do {
		iter++;
		printf("SOR iter %d: ", iter);
		doSORIter(params, &itPar, uData, rhsData, gCoeffs, grads);

	} while ((iter < maxIter) && (itPar.res > tol));

	// rhsOld = dataNew
	for (int i = 0; i < dataSize; i++) {
		rhsData[i] = uData[i];
	}
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

EdgeDiffusionCoeffs ImageProcessor::getEdgeDiffusionWeights(double* dataNeighbors, double* smoothedDataNeighbors, double K_coeff)
{
	EdgeDiffusionCoeffs g;

	double dataNorthWest = dataNeighbors[0], dataNorth = dataNeighbors[1], dataNorthEast = dataNeighbors[2];
	double dataWest = dataNeighbors[3], dataMid = dataNeighbors[4], dataEast = dataNeighbors[5];
	double dataSouthWest = dataNeighbors[6], dataSouth = dataNeighbors[7], dataSouthEast = dataNeighbors[8];

	double dataSigmaNorthWest = smoothedDataNeighbors[0], dataSigmaNorth = smoothedDataNeighbors[1], dataSigmaNorthEast = smoothedDataNeighbors[2];
	double dataSigmaWest = smoothedDataNeighbors[3], dataSigmaMid = smoothedDataNeighbors[4], dataSigmaEast = smoothedDataNeighbors[5];
	double dataSigmaSouthWest = smoothedDataNeighbors[6], dataSigmaSouth = smoothedDataNeighbors[7], dataSigmaSouthEast = smoothedDataNeighbors[8];

	double gX, gY, normSq;

	// all sigma
	/*
	gX = ((dataSigmaNorthEast - dataSigmaNorthWest) * 0.5 + (dataSigmaEast - dataSigmaWest) * 0.5) * 0.5;
	gY = dataSigmaNorth - dataSigmaMid;
	normSq = gX * gX + gY * gY;
	g.north = 1.0 / (1.0 + K_coeff * normSq);

	gX = dataSigmaMid- dataSigmaWest;
	gY = ((dataSigmaNorthWest - dataSigmaSouthWest) * 0.5 + (dataSigmaNorth - dataSigmaSouth) * 0.5) * 0.5;
	normSq = gX * gX + gY * gY;
	g.west = 1.0 / (1.0 + K_coeff * normSq);

	gX = dataSigmaEast - dataSigmaMid;
	gY = ((dataSigmaNorth - dataSigmaSouth) * 0.5 + (dataSigmaNorthEast - dataSigmaSouthEast) * 0.5) * 0.5;
	normSq = gX * gX + gY * gY;
	g.east = 1.0 / (1.0 + K_coeff * normSq);

	gX = ((dataSigmaNorth - dataSigmaSouth) * 0.5 + (dataSigmaSouthEast - dataSigmaSouthWest) * 0.5) * 0.5;
	gY = dataSigmaMid - dataSigmaSouth;
	normSq = gX * gX + gY * gY;
	g.south = 1.0 / (1.0 + K_coeff * normSq);*/

	// just forward difference sigma
	/**/
	gX = ((dataNorthEast - dataNorthWest) * 0.5 + (dataEast - dataWest) * 0.5) * 0.5;
	gY = dataSigmaNorth - dataSigmaMid;
	normSq = gX * gX + gY * gY;
	g.north = 1.0 / (1.0 + K_coeff * normSq);

	gX = dataSigmaMid - dataSigmaWest;
	gY = ((dataNorthWest - dataSouthWest) * 0.5 + (dataNorth - dataSouth) * 0.5) * 0.5;
	normSq = gX * gX + gY * gY;
	g.west = 1.0 / (1.0 + K_coeff * normSq);

	gX = dataSigmaEast - dataSigmaMid;
	gY = ((dataNorth - dataSouth) * 0.5 + (dataNorthEast - dataSouthEast) * 0.5) * 0.5;
	normSq = gX * gX + gY * gY;
	g.east = 1.0 / (1.0 + K_coeff * normSq);

	gX = ((dataNorth - dataSouth) * 0.5 + (dataSouthEast - dataSouthWest) * 0.5) * 0.5;
	gY = dataSigmaMid - dataSigmaSouth;
	normSq = gX * gX + gY * gY;
	g.south = 1.0 / (1.0 + K_coeff * normSq);

	// no sigma
	/*
	gX = ((dataNorthEast - dataNorthWest) * 0.5 + (dataEast - dataWest) * 0.5) * 0.5;
	gY = dataNorth - dataMid;
	normSq = gX * gX + gY * gY;
	g.north = 1.0 / (1.0 + K_coeff * normSq);

	gX = dataMid - dataWest;
	gY = ((dataNorthWest - dataSouthWest) * 0.5 + (dataNorth - dataSouth) * 0.5) * 0.5;
	normSq = gX * gX + gY * gY;
	g.west = 1.0 / (1.0 + K_coeff * normSq);

	gX = dataEast - dataMid;
	gY = ((dataNorth - dataSouth) * 0.5 + (dataNorthEast - dataSouthEast) * 0.5) * 0.5;
	normSq = gX * gX + gY * gY;
	g.east = 1.0 / (1.0 + K_coeff * normSq);

	gX = ((dataNorth - dataSouth) * 0.5 + (dataSouthEast - dataSouthWest) * 0.5) * 0.5;
	gY = dataMid - dataSouth;
	normSq = gX * gX + gY * gY;
	g.south = 1.0 / (1.0 + K_coeff * normSq);*/

	// just central difference sigma
	/*
	gX = ((dataSigmaNorthEast - dataSigmaNorthWest) * 0.5 + (dataSigmaEast - dataSigmaWest) * 0.5) * 0.5;
	gY = dataNorth - dataMid;
	normSq = gX * gX + gY * gY;
	g.north = 1.0 / (1.0 + K_coeff * normSq);

	gX = dataMid - dataWest;
	gY = ((dataSigmaNorthWest - dataSigmaSouthWest) * 0.5 + (dataSigmaNorth - dataSigmaSouth) * 0.5) * 0.5;
	normSq = gX * gX + gY * gY;
	g.west = 1.0 / (1.0 + K_coeff * normSq);

	gX = dataEast - dataMid;
	gY = ((dataSigmaNorth - dataSigmaSouth) * 0.5 + (dataSigmaNorthEast - dataSigmaSouthEast) * 0.5) * 0.5;
	normSq = gX * gX + gY * gY;
	g.east = 1.0 / (1.0 + K_coeff * normSq);

	gX = ((dataSigmaNorth - dataSigmaSouth) * 0.5 + (dataSigmaSouthEast - dataSigmaSouthWest) * 0.5) * 0.5;
	gY = dataMid - dataSouth;
	normSq = gX * gX + gY * gY;
	g.south = 1.0 / (1.0 + K_coeff * normSq);*/

	return g;
}

EdgeDiffusionCoeffs ImageProcessor::getRegularizedEdgeGradients(double* dataNeighbors, double epsilon)
{
	EdgeDiffusionCoeffs g;

	double dataNorthWest = dataNeighbors[0], dataNorth = dataNeighbors[1], dataNorthEast = dataNeighbors[2];
	double dataWest = dataNeighbors[3], dataMid = dataNeighbors[4], dataEast = dataNeighbors[5];
	double dataSouthWest = dataNeighbors[6], dataSouth = dataNeighbors[7], dataSouthEast = dataNeighbors[8];

	double gX, gY;

	gX = ((dataNorthEast - dataNorthWest) * 0.5 + (dataEast - dataWest) * 0.5) * 0.5;
	gY = dataNorth - dataMid;
	g.north = sqrt(gX * gX + gY * gY + epsilon);

	gX = dataMid - dataWest;
	gY = ((dataNorthWest - dataSouthWest) * 0.5 + (dataNorth - dataSouth) * 0.5) * 0.5;
	g.west = sqrt(gX * gX + gY * gY + epsilon);

	gX = dataEast - dataMid;
	gY = ((dataNorth - dataSouth) * 0.5 + (dataNorthEast - dataSouthEast) * 0.5) * 0.5;
	g.east = sqrt(gX * gX + gY * gY + epsilon);

	gX = ((dataNorth - dataSouth) * 0.5 + (dataSouthEast - dataSouthWest) * 0.5) * 0.5;
	gY = dataMid - dataSouth;
	g.south = sqrt(gX * gX + gY * gY + epsilon);

	return g;
}

void ImageProcessor::computeAllEdgeDiffusionCoeffs(double* uData, double* uSigmaData, ImageParams* params, std::vector<EdgeDiffusionCoeffs>* results, double K_coeff)
{
	int height = params->height;
	int width = params->width;
	int depth = params->depth;
	int row = params->row;
	double* dataNeighbors = new double[9];
	double* smoothedDataNeighbors = new double[9];

	int count = 0;
	for (int y = 0; y < height - 2; y++) {
		for (int x = 0; x < width - 2; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			// neighboring pixels:
			int xNorth = xPos, yNorth = y;
			int xSouth = xPos, ySouth = yPos + 1;
			int xWest = x, yWest = yPos;
			int xEast = xPos + 1, yEast = yPos;
			// ------
			if (depth == 8) {
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row);
				getDataNeighbors(smoothedDataNeighbors, uSigmaData, xPos, yPos, row);

				results->at(count++) = getEdgeDiffusionWeights(dataNeighbors, smoothedDataNeighbors, K_coeff);
			}
			else {
				// ======== RED CHANNEL =======================================
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row, 0);
				getDataNeighbors(smoothedDataNeighbors, uSigmaData, xPos, yPos, row, 0);

				results->at(count++) = getEdgeDiffusionWeights(dataNeighbors, smoothedDataNeighbors, K_coeff);

				// ======== GREEN CHANNEL =======================================
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row, 1);
				getDataNeighbors(smoothedDataNeighbors, uSigmaData, xPos, yPos, row, 1);

				results->at(count++) = getEdgeDiffusionWeights(dataNeighbors, smoothedDataNeighbors, K_coeff);

				// ======== BLUE CHANNEL =======================================
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row, 2);
				getDataNeighbors(smoothedDataNeighbors, uSigmaData, xPos, yPos, row, 2);

				results->at(count++) = getEdgeDiffusionWeights(dataNeighbors, smoothedDataNeighbors, K_coeff);
			}
		}
	}
	delete[] dataNeighbors; delete[] smoothedDataNeighbors;
}

void ImageProcessor::computeAllEdgeRegularizedGradientCoeffs(double* uData, ImageParams* params, std::vector<EdgeDiffusionCoeffs>* results, double epsilon)
{
	int height = params->height;
	int width = params->width;
	int depth = params->depth;
	int row = params->row;
	double* dataNeighbors = new double[9];

	int count = 0;
	for (int y = 0; y < height - 2; y++) {
		for (int x = 0; x < width - 2; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			// neighboring pixels:
			int xNorth = xPos, yNorth = y;
			int xSouth = xPos, ySouth = yPos + 1;
			int xWest = x, yWest = yPos;
			int xEast = xPos + 1, yEast = yPos;
			// ------
			if (depth == 8) {
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row);

				results->at(count++) = getRegularizedEdgeGradients(dataNeighbors, epsilon);
			}
			else {
				// ======== RED CHANNEL =======================================
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row, 0);

				results->at(count++) = getRegularizedEdgeGradients(dataNeighbors, epsilon);

				// ======== GREEN CHANNEL =======================================
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row, 1);

				results->at(count++) = getRegularizedEdgeGradients(dataNeighbors, epsilon);

				// ======== BLUE CHANNEL =======================================
				getDataNeighbors(dataNeighbors, uData, xPos, yPos, row, 2);

				results->at(count++) = getRegularizedEdgeGradients(dataNeighbors, epsilon);
			}
		}
	}
	delete[] dataNeighbors;
}

double ImageProcessor::sumNeighborPixels(
	double dataNorth, double dataWest, double dataSouth, double dataEast, 
	EdgeDiffusionCoeffs* g, EdgeDiffusionCoeffs* gradU)
{
	if (!g && !gradU) return (dataNorth + dataWest + dataSouth + dataEast);
	if (g && !gradU) return (g->north * dataNorth + g->west * dataWest + g->south * dataSouth + g->east * dataEast);
	if (!g && gradU) return (
		(1.0 / gradU->north) * dataNorth + (1.0 / gradU->west) * dataWest +
		(1.0 / gradU->south) * dataSouth + (1.0 / gradU->east) * dataEast
	);

	return (
		(g->north / gradU->north) * dataNorth + (g->west / gradU->west) * dataWest +
		(g->south / gradU->south) * dataSouth + (g->east / gradU->east) * dataEast
	);
}

void ImageProcessor::doSORIter(
	ImageParams* imgPar, IterParams* itPar, double* uData, double* rhsData, 
	std::vector<EdgeDiffusionCoeffs>* g, std::vector<EdgeDiffusionCoeffs>* gradU)
{
	int height = imgPar->height;
	int width = imgPar->width;
	int depth = imgPar->depth;
	int row = imgPar->row;

	double coeff = itPar->coeff, diagCoeff = itPar->diagCoeff, omega = itPar->omega;
	bool useDiffusionCoeffs = g != nullptr;
	bool useGradients = gradU != nullptr;

	const int posMax = 5;

	int count = 0;
	for (int y = 0; y < height - 2; y++) {
		for (int x = 0; x < width - 2; x++) {
			// current pixel:
			int xPos = x + 1, yPos = y + 1;
			// neighboring pixels:
			int xNorth = xPos, yNorth = y;
			int xSouth = xPos, ySouth = yPos + 1;
			int xWest = x, yWest = yPos;
			int xEast = xPos + 1, yEast = yPos;

			if (depth == 8) {
				int pos = yPos * row + xPos;
		
				// default : implicit heat equation
				//
				if (useDiffusionCoeffs && !useGradients) {
					// semi-implicit Perona Malik
					diagCoeff = 1.0 + itPar->tau * g->at(count).sum();  
				}
				else if (useGradients && !useDiffusionCoeffs) {
					// semi-implicit MCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumInv();
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}
				else if (useGradients && useDiffusionCoeffs) {
					// semi-implicit GMCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumGInv(&g->at(count));
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}

				double sumNeighbors = coeff * sumNeighborPixels(
					uData[yNorth * row + xNorth], uData[yWest * row + xWest],
					uData[ySouth * row + xSouth], uData[yEast * row + xEast], 
					(useDiffusionCoeffs ? &g->at(count) : nullptr), (useGradients ? &gradU->at(count) : nullptr));

				double val = (omega / diagCoeff) * (rhsData[pos] - sumNeighbors) + (1.0 - omega) * uData[pos];

				/*
				if (useGradients && ((x < posMax && y == 0) || (x > width - 3 - posMax && y == height - 3))) {
					double coeffNorth = coeff * (useDiffusionCoeffs ? g->at(count).north : 1.0) / gradU->at(count).north;
					double coeffWest = coeff * (useDiffusionCoeffs ? g->at(count).west : 1.0) / gradU->at(count).west;
					double coeffEast = coeff * (useDiffusionCoeffs ? g->at(count).east : 1.0) / gradU->at(count).east;
					double coeffSouth = coeff * (useDiffusionCoeffs ? g->at(count).south : 1.0) / gradU->at(count).south;
					printf("diagCoeff = %lf, coeffNorth = %lf, coeffWest = %lf, coeffEast = %lf, coeffSouth = %lf\n", diagCoeff, coeffNorth, coeffWest, coeffEast, coeffSouth);
				}*/

				count++;
				uData[pos] = val;
			}
			else {
				int pos_red = yPos * row + xPos * 4;
				int pos_green = yPos * row + xPos * 4 + 1;
				int pos_blue = yPos * row + xPos * 4 + 2;

				// ============= RED CHANNEL =======================================
				if (useDiffusionCoeffs && !useGradients) {
					// semi-implicit Perona Malik
					diagCoeff = 1.0 + itPar->tau * g->at(count).sum();
				} 
				else if (useGradients && !useDiffusionCoeffs) {
					// semi-implicit MCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumInv();
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}
				else if (useGradients && useDiffusionCoeffs) {
					// semi-implicit GMCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumGInv(&g->at(count));
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}

				double sumNeighbors_red = coeff * sumNeighborPixels(
					uData[yNorth * row + xNorth * 4], uData[yWest * row + xWest * 4],
					uData[ySouth * row + xSouth * 4], uData[yEast * row + xEast * 4], 
					(useDiffusionCoeffs ? &g->at(count) : nullptr), (useGradients ? &gradU->at(count) : nullptr));
				count++;

				// ============== GREEN CHANNEL ==================================
				if (useDiffusionCoeffs && !useGradients) {
					// semi-implicit Perona Malik
					diagCoeff = 1.0 + itPar->tau * g->at(count).sum();
				}
				else if (useGradients && !useDiffusionCoeffs) {
					// semi-implicit MCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumInv();
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}
				else if (useGradients && useDiffusionCoeffs) {
					// semi-implicit GMCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumGInv(&g->at(count));
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}

				double sumNeighbors_green = coeff * sumNeighborPixels(
					uData[yNorth * row + xNorth * 4 + 1], uData[yWest * row + xWest * 4 + 1],
					uData[ySouth * row + xSouth * 4 + 1], uData[yEast * row + xEast * 4 + 1], 
					(useDiffusionCoeffs ? &g->at(count) : nullptr), (useGradients ? &gradU->at(count) : nullptr));
				count++;

				// ============= BLUE CHANNEL =====================================
				if (useDiffusionCoeffs && !useGradients) {
					// semi-implicit Perona Malik
					diagCoeff = 1.0 + itPar->tau * g->at(count).sum();
				}
				else if (useGradients && !useDiffusionCoeffs) {
					// semi-implicit MCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumInv();
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}
				else if (useGradients && useDiffusionCoeffs) {
					// semi-implicit GMCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumGInv(&g->at(count));
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}

				double sumNeighbors_blue = coeff * sumNeighborPixels(
					uData[yNorth * row + xNorth * 4 + 2], uData[yWest * row + xWest * 4 + 2],
					uData[ySouth * row + xSouth * 4 + 2], uData[yEast * row + xEast * 4 + 2], 
					(useDiffusionCoeffs ? &g->at(count) : nullptr), (useGradients ? &gradU->at(count) : nullptr));
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

	// copy extended & compute mean
	double mean = 0.0, mean_red = 0.0, mean_green = 0.0, mean_blue = 0.0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int xPos = (x == 0 ? 1 : (x == width - 1 ? width - 2 : x));
			int yPos = (y == 0 ? 1 : (y == height - 1 ? height - 2 : y));

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
		mean /= (width * height);
		printf("u_mean = %lf, ", mean);
	}
	else if (itPar->printMean) {
		mean_red /= (width * height);
		mean_green /= (width * height);
		mean_blue /= (width * height);
		printf("u_mean(RGB) = (%lf, %lf, %lf),", mean_red, mean_green, mean_blue);
	}

	// compute residuum
	if (depth == 8) {
		itPar->res = 0.0;
		count = 0;
		for (int y = 0; y < height - 2; y++) {
			for (int x = 0; x < width - 2; x++) {
				// current pixel:
				int xPos = x + 1, yPos = y + 1;
				int pos = yPos * row + xPos;
				// neighboring pixels:
				int xNorth = xPos, yNorth = y;
				int xSouth = xPos, ySouth = yPos + 1;
				int xWest = x, yWest = yPos;
				int xEast = xPos + 1, yEast = yPos;

				if (useDiffusionCoeffs && !useGradients) {
					// semi-implicit Perona Malik
					diagCoeff = 1.0 + itPar->tau * g->at(count).sum();
				}
				else if (useGradients && !useDiffusionCoeffs) {
					// semi-implicit MCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumInv();
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}
				else if (useGradients && useDiffusionCoeffs) {
					// semi-implicit GMCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumGInv(&g->at(count));
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}

				/*
				if (x == 0 && y == 0) printf("res:\n");
				if (useGradients && ((x < posMax && y == 0) || (x > width - 3 - posMax && y == height - 3))) {
					double coeffNorth = coeff * (useDiffusionCoeffs ? g->at(count).north : 1.0) / gradU->at(count).north;
					double coeffWest = coeff * (useDiffusionCoeffs ? g->at(count).west : 1.0) / gradU->at(count).west;
					double coeffEast = coeff * (useDiffusionCoeffs ? g->at(count).east : 1.0) / gradU->at(count).east;
					double coeffSouth = coeff * (useDiffusionCoeffs ? g->at(count).south : 1.0) / gradU->at(count).south;
					printf("diagCoeff = %lf, coeffNorth = %lf, coeffWest = %lf, coeffEast = %lf, coeffSouth = %lf\n", diagCoeff, coeffNorth, coeffWest, coeffEast, coeffSouth);
				}*/

				// (A * u - b)_i
				/* double uCurrent = uData[pos];
				double rhsCurrent = rhsData[pos];
				double neighborSum = sumNeighborPixels(
					uData[yNorth * row + xNorth], uData[yWest * row + xWest],
					uData[ySouth * row + xSouth], uData[yEast * row + xEast],
					(useDiffusionCoeffs ? &g->at(count) : nullptr), (useGradients ? &gradU->at(count) : nullptr)); */
				double sum = diagCoeff * uData[pos] +
					coeff * sumNeighborPixels(
						uData[yNorth * row + xNorth], uData[yWest * row + xWest], 
						uData[ySouth * row + xSouth], uData[yEast * row + xEast], 
						(useDiffusionCoeffs ? &g->at(count) : nullptr), (useGradients ? &gradU->at(count) : nullptr)) -
					rhsData[pos];
				itPar->res += sum * sum;
				count++;
			}
		}
		// not counting extended pixels, since they do not change
		itPar->res = sqrt(itPar->res);
		if (itPar->printIter) printf("res = %lf\n", itPar->res);
	}
	else {
		double res_red = 0.0, res_green = 0.0, res_blue = 0.0;
		count = 0;
		for (int y = 0; y < height - 2; y++) {
			for (int x = 0; x < width - 2; x++) {
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
				if (useDiffusionCoeffs && !useGradients) {
					// semi-implicit Perona Malik
					diagCoeff = 1.0 + itPar->tau * g->at(count).sum();
				}
				else if (useGradients && !useDiffusionCoeffs) {
					// semi-implicit MCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumInv();
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}
				else if (useGradients && useDiffusionCoeffs) {
					// semi-implicit GMCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumGInv(&g->at(count));
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}

				// (A * u - b)_i
				double sum_red = diagCoeff * uData[pos_red] +
					coeff * sumNeighborPixels(
						uData[yNorth * row + xNorth * 4], uData[yWest * row + xWest * 4],
						uData[ySouth * row + xSouth * 4], uData[yEast * row + xEast * 4], 
						(useDiffusionCoeffs ? &g->at(count) : nullptr), (useGradients ? &gradU->at(count) : nullptr)) -
					rhsData[pos_red];
				count++;

				// ============== GREEN CHANNEL ==================================
				if (useDiffusionCoeffs && !useGradients) {
					// semi-implicit Perona Malik
					diagCoeff = 1.0 + itPar->tau * g->at(count).sum();
				}
				else if (useGradients && !useDiffusionCoeffs) {
					// semi-implicit MCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumInv();
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}
				else if (useGradients && useDiffusionCoeffs) {
					// semi-implicit GMCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumGInv(&g->at(count));
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}

				double sum_green = diagCoeff * uData[pos_green] +
					coeff * sumNeighborPixels(
						uData[yNorth * row + xNorth * 4 + 1], uData[yWest * row + xWest * 4 + 1],
						uData[ySouth * row + xSouth * 4 + 1], uData[yEast * row + xEast * 4 + 1], 
						(useDiffusionCoeffs ? &g->at(count) : nullptr), (useGradients ? &gradU->at(count) : nullptr)) -
					rhsData[pos_green];
				count++;

				// ============= BLUE CHANNEL =====================================
				if (useDiffusionCoeffs && !useGradients) {
					// semi-implicit Perona Malik
					diagCoeff = 1.0 + itPar->tau * g->at(count).sum();
				}
				else if (useGradients && !useDiffusionCoeffs) {
					// semi-implicit MCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumInv();
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}
				else if (useGradients && useDiffusionCoeffs) {
					// semi-implicit GMCF
					diagCoeff = 1.0 + itPar->tau * (0.25 * gradU->at(count).sum()) * gradU->at(count).sumGInv(&g->at(count));
					coeff = -itPar->tau * (0.25 * gradU->at(count).sum());
				}

				double sum_blue = diagCoeff * uData[pos_blue] +
					coeff * sumNeighborPixels(
						uData[yNorth * row + xNorth * 4 + 2], uData[yWest * row + xWest * 4 + 2],
						uData[ySouth * row + xSouth * 4 + 2], uData[yEast * row + xEast * 4 + 2],
						(useDiffusionCoeffs ? &g->at(count) : nullptr), (useGradients ? &gradU->at(count) : nullptr)) -
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

void ImageProcessor::setZeroLevelCurveFromIsoData(double* isoData, double* distGrid, bool* frozenCells, ImageParams* imgPar)
{
	const int height = imgPar->height, width = imgPar->width;
	const int row = imgPar->row;
	double* dataNeighbors = new double[9];

	for (int y = 0; y < height - 2; y++) {
		for (int x = 0; x < width - 2; x++) {
			int xPos = x + 1, yPos = y + 1;
			getDataNeighbors(dataNeighbors, isoData, xPos, yPos, row);
			
			// first boundary test
			double neighborMean = 0.0;
			for (int i = 0; i < 9; i++) neighborMean += dataNeighbors[i];
			neighborMean /= 9.;

			if (fabs(neighborMean) < DBL_EPSILON || fabs(neighborMean - 255.0) < DBL_EPSILON) {
				continue; // not a boundary cell
			}

			frozenCells[yPos * row + xPos] = true; // processed means frozen

			// second boundary tests (cell edges and corners)
			double isoDataMid = dataNeighbors[4];
			bool edgeCells = (
				fabs(isoDataMid - dataNeighbors[1]) > DBL_EPSILON ||
				fabs(isoDataMid - dataNeighbors[3]) > DBL_EPSILON ||
				fabs(isoDataMid - dataNeighbors[5]) > DBL_EPSILON ||
				fabs(isoDataMid - dataNeighbors[7]) > DBL_EPSILON
			);
			bool cornerCells = (
				fabs(isoDataMid - dataNeighbors[0]) > DBL_EPSILON ||
				fabs(isoDataMid - dataNeighbors[2]) > DBL_EPSILON ||
				fabs(isoDataMid - dataNeighbors[6]) > DBL_EPSILON ||
				fabs(isoDataMid - dataNeighbors[8]) > DBL_EPSILON 
			);

			if (cornerCells && !edgeCells) {
				distGrid[yPos * row + xPos] = M_SQRT2 / 2.0; // distance from cell centroid to corner
			}
			else {
				distGrid[yPos * row + xPos] = 0.5; // distance from cell centroid to edge
			}
		}
	}

	delete[] dataNeighbors;
}

void ImageProcessor::fastSweep2D(double* distGrid, bool* frozenCells, ImageParams* imgPar)
{
	const int height = imgPar->height, width = imgPar->width;
	const int row = imgPar->row;

	const int NSweeps = 4;
	// sweep directions { start, end, step }
	const int dirX[NSweeps][3] = { {0, width - 1, 1} , {width - 1, 0, -1}, {width - 1, 0, -1}, {0, width - 1, 1} };
	const int dirY[NSweeps][3] = { {0, height - 1, 1}, {0, height - 1, 1}, {height - 1, 0, -1}, {height - 1, 0, -1} };
	double aa[2], tmp, eps = 1e-6;
	double d_curr, d_new, a, b, c, D;
	int s, ix, iy, gridPos;
	const double h = 1.0, f = 1.0;

	for (s = 0; s < NSweeps; s++) {
		//printf("sweep %d ... \n", s);
		for (iy = dirY[s][0]; dirY[s][2] * iy <= dirY[s][1]; iy += dirY[s][2]) {
			for (ix = dirX[s][0]; dirX[s][2] * ix <= dirX[s][1]; ix += dirX[s][2]) {

				gridPos = iy * row + ix;

				if (!frozenCells[gridPos]) {

					// === neighboring cells (Upwind Godunov) ===
					if (iy == 0 || iy == (height - 1)) {
						if (iy == 0) {
							aa[1] = distGrid[gridPos] < distGrid[(iy + 1) * row + ix] ? distGrid[gridPos] : distGrid[(iy + 1) * row + ix];
						}
						if (iy == (height - 1)) {
							aa[1] = distGrid[(iy - 1) * row + ix] < distGrid[gridPos] ? distGrid[(iy - 1) * row + ix] : distGrid[gridPos];
						}
					}
					else {
						aa[1] = distGrid[(iy - 1) * row + ix] < distGrid[(iy + 1) * row + ix] ? distGrid[(iy - 1) * row + ix] : distGrid[(iy + 1) * row + ix];
					}

					if (ix == 0 || ix == (width - 1)) {
						if (ix == 0) {
							aa[0] = distGrid[gridPos] < distGrid[iy * row + (ix + 1)] ? distGrid[gridPos] : distGrid[iy * row + (ix + 1)];
						}
						if (ix == (width - 1)) {
							aa[0] = distGrid[iy * row + (ix - 1)] < distGrid[gridPos] ? distGrid[iy * row + (ix - 1)] : distGrid[gridPos];
						}
					}
					else {
						aa[0] = distGrid[iy * row + (ix - 1)] < distGrid[iy * row + (ix + 1)] ? distGrid[iy * row + (ix - 1)] : distGrid[iy * row + (ix + 1)];
					}

					a = aa[0]; b = aa[1];
					d_new = (fabs(a - b) < f * h ? (a + b + sqrt(2.0 * f * f * h * h - (a - b) * (a - b))) * 0.5 : std::fminf(a, b) + f * h);

					distGrid[gridPos] = distGrid[gridPos] < d_new ? distGrid[gridPos] : d_new;
				}
			}
		}
	}
}

void ImageProcessor::negateData(double* data, ImageParams* imgPar)
{
	const int height = imgPar->height, width = imgPar->width;
	const int row = imgPar->row;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			data[y * row + x] = -data[y * row + x];
		}
	}
}

void ImageProcessor::computeSignFloodFill(double* distGrid, bool* frozenCells, ImageParams* imgPar)
{
	negateData(distGrid, imgPar);

	double val; int gridPos;
	const int row = imgPar->row;
	const int nx = imgPar->width - 1;
	const int ny = imgPar->height - 1;
	int ix = 0, iy = 0;

	std::stack<std::tuple<int, int>> stack = {};
	std::tuple<int, int> idsPair;
	// find the first unfrozen cell
	gridPos = 0;
	while (frozenCells[gridPos]) {
		ix += (ix < nx ? 1 : 0);
		iy += (iy < ny ? 1 : 0);
		gridPos = row * iy + ix;
	}
	stack.push({ ix, iy });
	// a simple pixel flood
	while (stack.size()) {
		idsPair = stack.top();
		stack.pop();
		ix = std::get<0>(idsPair);
		iy = std::get<1>(idsPair);
		gridPos = row * iy + ix;
		if (!frozenCells[gridPos]) {
			val = -1.0 * distGrid[gridPos];
			distGrid[gridPos] = val;
			frozenCells[gridPos] = true; // freeze cell when done
			if (ix > 0) {
				stack.push({ ix - 1, iy });
			}
			if (ix < nx) {
				stack.push({ ix + 1, iy });
			}
			if (iy > 0) {
				stack.push({ ix, iy - 1 });
			}
			if (iy < ny) {
				stack.push({ ix, iy + 1 });
			}
		}
	}
}

void ImageProcessor::computeSignFromIsoData(double* distGrid, uchar* isoData, ImageParams* imgPar)
{
	const int height = imgPar->height, width = imgPar->width;
	const int row = imgPar->row;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			distGrid[y * row + x] *= (isoData[y * row + x] > 0 ? 1.0 : -1.0);
		}
	}
}

void ImageProcessor::getDataNeighbors(double* dataNeighbors, double* data, int xPos, int yPos, int row, int channel)
{
	int xNorth = xPos, yNorth = yPos - 1;
	int xSouth = xPos, ySouth = yPos + 1;
	int xWest = xPos - 1, yWest = yPos;
	int xEast = xPos + 1, yEast = yPos;

	int stride = (channel >= 0 ? 4 : 1);
	channel = (channel >= 0 ? channel : 0);

	dataNeighbors[0] = data[yNorth * row + xWest * stride + channel]; // northwest
	dataNeighbors[1] = data[yNorth * row + xPos * stride + channel]; // north
	dataNeighbors[2] = data[yNorth * row + xEast * stride + channel]; // northeast
	dataNeighbors[3] = data[yPos * row + xWest * stride + channel]; // west
	dataNeighbors[4] = data[yPos * row + xPos * stride + channel]; // mid
	dataNeighbors[5] = data[yPos * row + xEast * stride + channel]; // east
	dataNeighbors[6] = data[ySouth * row + xWest * stride + channel]; // southwest
	dataNeighbors[7] = data[ySouth * row + xPos * stride + channel]; // south
	dataNeighbors[8] = data[ySouth * row + xEast * stride + channel]; // southeast
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

IterParams::IterParams(
	double tau, double diagCoeff, double coeff, double omega, 
	bool printMean, bool printIter, double K_coeff, int printStep)
{
	this->tau = tau; this->diagCoeff = diagCoeff; this->coeff = coeff; this->omega = omega;
	this->printMean = printMean; this->printIter = printIter;
	this->K_coeff = K_coeff; this->printStep = printStep;
}
