#ifndef IMAGEPROCESSOR_H_
#define IMAGEPROCESSOR_H_

#define _USE_MATH_DEFINES

#include "ViewerWidget.h"

// contains all tools for image processing and communicating with the ViewerWidget
class ImageProcessor
{
private: 
	QImage* img = nullptr;
public:
	ViewerWidget* viewer = nullptr;

	ImageProcessor();
	~ImageProcessor();

	void setCurrentImg(QImage* img);
};

#endif