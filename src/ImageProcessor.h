#ifndef IMAGEPROCESSOR_H_
#define IMAGEPROCESSOR_H_

#define _USE_MATH_DEFINES

#include <qimage.h>
#include <QtCore/QObject>
#include "ViewerWidget.h"
#include "Dialogs.h"
#include "HistogramWindow.h"

class ImageProcessor : public QObject
{
	Q_OBJECT

public:
	ImageProcessor(ViewerWidget* viewer);
	~ImageProcessor();

	int nSteps;

	// Image functions
	bool clearImage();
	bool invertColors();
	bool grayscale(float wRed = 0.3f, float wGreen = 0.6f, float wBlue = 0.11f, float gamma = 0.5f);

public slots:
	// Image Slots
	void mirrorExtendAccepted();
	void blurAccepted();
	void bernsenThresholdAccepted();
	void multiBlurAccepted();
	void heatEquationAccepted();

	//Histogram slots
	void on_stretch();
	void on_histogramGrayscale();
	void on_singleThreshold();

private:
	ViewerWidget* _view_w = nullptr;

	// Image functions
	bool mirrorExtendImageBy(int nPixels);

	// Mask image functions
	uchar kernelSumGS(uchar* img, int row, int x, int y, int r);
	QRgb kernelSumRGB(uchar* img, int row, int x, int y, int r);
	void findGrayscaleExtremesWithinCircle(uchar* img, int row, int x, int y, uchar* minVal, uchar* maxVal);
	void findRGBExtremesWithinCircle(uchar* img, int row, int x, int y, QColor* minVal, QColor* maxVal);

	bool blurImage(int radius);
	bool bernsenThreshold(int radius, int minContrast, int bgType);
	bool explicitHeatEquation(float timeStep);
	bool implicitHeatEquation(float timeStep, double* rhsData, bool printMean = true, bool printIter = true);

	void getGaussianKernel(int radius, bool print = false);
	void getAveragingKernel(int radius, bool print = false);
	void getCircularKernel(int radius, bool print = false);

	// Mask values
	std::vector<float>* W = nullptr;
	std::vector<int>* iMask = nullptr;

	void clearMasks();
signals:
	void sigGrayscaled();
	void multiImageComplete();
};

#endif

