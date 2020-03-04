#include "ImageProcessor.h"

ImageProcessor::ImageProcessor()
{
}

ImageProcessor::~ImageProcessor()
{
	delete viewer;
	delete img;
}

void ImageProcessor::setCurrentImg(QImage* img)
{
	this->img = img;
}
