#pragma once
#include <QtWidgets>

class ViewerWidget :public QWidget {
	Q_OBJECT

public:
	ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent = Q_NULLPTR);
	~ViewerWidget();
	void resizeWidget(QSize size);

	//Image functions
	bool setImage(const QImage& inputImg);
	void setPixel(QImage * img, int x, int y, uchar r, uchar g, uchar b, uchar a = 255);
	void setPixel(QImage * img, int x, int y, uchar val);
	void setPixel(QImage * img, int x, int y, double val);
	void setPixel(QImage * img, int x, int y, double valR, double valG, double valB, const double valA = 1.);

	//Draw functions
	void freeDraw(const QPoint& end, const QPen& pen);

	//Class inline functions
	inline bool isEmpty() { if (img->size() == QSize(0, 0)) return true; else return false; };
	inline const QString& getName() { return name; }
	inline const void setName(QString newName) { name = newName; }
	inline int getImgWidth() { return img->width(); };
	inline int getImgHeight() { return img->height(); };
	inline void clear() {  };
	inline QImage* getImage() { return img; };
	inline uchar* getImageData() { return data; };
	inline bool isInside(QImage* image, int x, int y) { return (x >= 0 && y >= 0 && x < image->width() && y < image->height()); }
	inline void setPainter() { painter = new QPainter(img); }
	inline void setDataPtr() { data = img->bits(); }

	inline void setFreeDrawBegin(const QPoint& begin) { freeDrawBegin = begin; }
	inline const QPoint& getFreeDrawBegin() { return freeDrawBegin; }
	inline void setFreeDrawActivated(bool state) { freeDrawActivated = state; }
	inline bool getFreeDrawActivated() { return freeDrawActivated; }

	inline uchar* getData() { return data; }
private:
	QPointer<QTab> tab;
	QString name = "";
	QSize areaSize = QSize(0, 0);
	QImage* img = nullptr;
	uchar* data = nullptr;
	QPainter* painter = nullptr;

	bool freeDrawActivated = false;
	QPoint freeDrawBegin = QPoint(0, 0);

public slots:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};