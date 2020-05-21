#ifndef IMAGEPROCESSOR_H_
#define IMAGEPROCESSOR_H_

#define _USE_MATH_DEFINES

#include <qimage.h>
#include <QtCore/QObject>
#include <vector>
#include <fstream>
#include "ViewerWidget.h"
#include "Dialogs.h"
#include "HistogramWindow.h"

struct IterParams
{
	double tau;
	double diagCoeff, coeff;
	double omega;

	double K_coeff;

	double res = DBL_MAX;
	bool printMean = false, printIter = false;
	int printStep = -1;

	IterParams(double tau, double diagCoeff, double coeff, double omega, 
		bool printMean, bool printIter, double K_coeff = 1.0, int printStep = -1);
};

struct EdgeDiffusionCoeffs
{
	double north, south, east, west;
	double sum() { return north + west + east + south; };
	double sumInv() { return 1.0 / north + 1.0 / west + 1.0 / east + 1.0 / south; };
	double sumGInv(EdgeDiffusionCoeffs* g) { return (g->north / north + g->west / west + g->east / east + g->south / south); };
};

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
	void peronaMalikAccepted();
	void curvatureFlowAccepted();

	//Histogram slots
	void on_stretch();
	void on_histogramGrayscale();
	void on_singleThreshold();

private:
	ViewerWidget* _view_w = nullptr;

	/*
	int xToSave, yToSave;
	std::fstream outValsRed;
	std::fstream outValsGreen;
	std::fstream outValsBlue;*/

	// Image functions
	bool mirrorExtendImageBy(int nPixels);
	void writeDataToImageAndUpdate(double* uData, ImageParams* params, int i);

	// Mask image functions
	uchar kernelSumGS(uchar* img, int row, int x, int y, int r);
	QRgb kernelSumRGB(uchar* img, int row, int x, int y, int r);
	void findGrayscaleExtremesWithinCircle(uchar* img, int row, int x, int y, uchar* minVal, uchar* maxVal);
	void findRGBExtremesWithinCircle(uchar* img, int row, int x, int y, QColor* minVal, QColor* maxVal);

	bool blurImage(int radius);
	bool bernsenThreshold(int radius, int minContrast, int bgType);
	// --- Filtration operations ------
	bool explicitHeatEquation(
		double timeStep, double* uData, ImageParams* params
	);
	bool implicitHeatEquation(
		double timeStep, double* uData, double* rhsData,
		ImageParams* params, bool printMean = true, bool printIter = true
	);
	bool explicitPeronaMalik(
		double timeStep, double K_coeff, double* uData, double* uSigmaData, ImageParams* params
	);
	bool semiImplicitPeronaMalik(
		double timeStep, double K_coeff, double* uData, double* rhsData, std::vector<EdgeDiffusionCoeffs>* G,
		ImageParams* params, bool printMean = true, bool printIter = true
	);
	bool meanCurvatureFlow(
		double timeStep, double* uData, double* rhsData, std::vector<EdgeDiffusionCoeffs>* G,
		ImageParams* params, bool printMean = true, bool printIter = true
	);
	bool geodesicMeanCurvatureFlow(
		double timeStep, double K_coeff, double* uData, double* rhsData, 
		std::vector<EdgeDiffusionCoeffs>* gCoeffs, std::vector<EdgeDiffusionCoeffs>* grads,
		ImageParams* params, bool printMean = true, bool printIter = true
	);
	// ------------------------------

	void getGaussianKernel(int radius, bool print = false);
	void getAveragingKernel(int radius, bool print = false);
	void getCircularKernel(int radius, bool print = false);

	EdgeDiffusionCoeffs getEdgeDiffusionWeights(double* dataNeighbors, double* smoothedDataNeighbors, double K_coeff);
	EdgeDiffusionCoeffs getRegularizedEdgeGradients(double* dataNeighbors, double epsilon);
	void computeAllEdgeDiffusionCoeffs(double* uData, double* uSigmaData, ImageParams* params, std::vector<EdgeDiffusionCoeffs>* results, double K_coeff);
	void computeAllEdgeRegularizedGradientCoeffs(double* uData, ImageParams* params, std::vector<EdgeDiffusionCoeffs>* results, double epsilon);
	double sumNeighborPixels(
		double dataNorth, double dataWest, double dataSouth, double dataEast, 
		EdgeDiffusionCoeffs* g = nullptr, EdgeDiffusionCoeffs* gradU = nullptr
	);
	void doSORIter(
		ImageParams* imgPar, IterParams* itPar, double* uData, double* rhsData, 
		std::vector<EdgeDiffusionCoeffs>* g = nullptr, std::vector<EdgeDiffusionCoeffs>* gradU = nullptr
	);


	void getDataNeighbors(double* dataNeighbors, double* uData, int xPos, int yPos, int row, int channel = -1);

	// Mask values
	std::vector<float>* W = nullptr;
	std::vector<int>* iMask = nullptr;

	void clearMasks();
signals:
	void sigGrayscaled();
	void multiImageComplete();
};

#endif

