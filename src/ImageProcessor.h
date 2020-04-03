#ifndef IMAGEPROCESSOR_H_
#define IMAGEPROCESSOR_H_

#define _USE_MATH_DEFINES

#include <qimage.h>
#include <QtCore/QObject>
#include "ViewerWidget.h"
#include "NewImageDialog.h"
#include "MirrorExtendDialog.h"
#include "HistogramWindow.h"
#include "BlurDialog.h"

class ImageProcessor : public QObject
{
	Q_OBJECT
public:
	ImageProcessor(ViewerWidget* viewer);
	~ImageProcessor();

	// Image functions
	bool clearImage();
	bool invertColors();

public slots:
	// Image Slots
	void mirrorExtendAccepted();
	void blurAccepted();

	//Histogram slots
	void on_stretch();

private:
	ViewerWidget* _view_w = nullptr;

	// Image functions
	bool mirrorExtendImageBy(int nPixels);

	// Mask image functions
	uchar kernelSum(uchar* img, int row, int x, int y, int r);
	QRgb kernelSum(uchar* img, int row, QPoint px, int r);

	bool blurImage(int radius);

	void getGaussianKernel(int radius, bool print = false);
	void getAveragingKernel(int radius, bool print = false);

	// Mask values
	std::vector<float>* W = nullptr;
};

#endif

