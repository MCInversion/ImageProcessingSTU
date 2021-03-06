#include "ViewerWidget.h"

ViewerWidget::ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	name = viewerName;
	if (imgSize != QSize(0, 0)) {
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
	}
}

ViewerWidget::~ViewerWidget()
{
	painter->end();
	delete img;
}

void ViewerWidget::resizeWidget(QSize size)
{
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}

void ViewerWidget::allocateImages(int n)
{
	imgArray = std::vector<QImage>(n);
}

bool ViewerWidget::setImageAt(const QImage& img, int i)
{
	if (i >= imgArray.size()) {
		return false;
	}
	imgArray[i] = QImage(img);
	return true;
}

void ViewerWidget::clearImages()
{
	imgArray.clear();
}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg)
{
	if (img) {
		delete img;
	}
	img = new QImage(inputImg);
	if (!img) {
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	setDataPtr();
	return true;
}

void ViewerWidget::setPixel(QImage* img, int x, int y, uchar r, uchar g, uchar b, uchar a)
{
	uchar* d = img->bits();
	if (isInside(img, x, y)) {
		int startbyte = y * img->bytesPerLine() + x * 4;
		d[startbyte] = r;
		d[startbyte + 1] = g;
		d[startbyte + 2] = b;
		d[startbyte + 3] = a;
	}
}

void ViewerWidget::setPixel(QImage* img, int x, int y, uchar val)
{
	uchar * d = img->bits();
	if (isInside(img, x, y)) {
		d[y * img->bytesPerLine() + x] = val;
	}
}

void ViewerWidget::setPixel(QImage* img, int x, int y, double val)
{
	if (isInside(img, x, y)) {
		if (val > 1) val = 1;
		if (val < 0) val = 0;
		setPixel(img, x, y, static_cast<uchar>(255 * val));
	}
}

void ViewerWidget::setPixel(QImage* img, int x, int y, double valR, double valG, double valB, double valA)
{
	if (isInside(img, x, y)) {
		if (valR > 1) valR = 1;
		if (valG > 1) valG = 1;
		if (valB > 1) valB = 1;
		

		if (valR < 0) valR = 0;
		if (valG < 0) valG = 0;
		if (valB < 0) valB = 0;
		
		double newValA = valA;
		if (newValA > 1) newValA = 1;
		if (newValA < 0) newValA = 0;
		setPixel(img, x, y, static_cast<uchar>(255 * valR), static_cast<uchar>(255 * valG), static_cast<uchar>(255 * valB), static_cast<uchar>(255 * valA));
	}
}

ImageParams ViewerWidget::getImageParams()
{
	return ImageParams(
		img->height(),
		img->width(),
		img->depth(),
		img->bytesPerLine(),
		img->format(),
		img->sizeInBytes()
	);
}

//Draw functions
void ViewerWidget::freeDraw(const QPoint& end, const QPen& pen)
{
	painter->setPen(pen);
	painter->drawLine(freeDrawBegin, end);
	update();
}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}