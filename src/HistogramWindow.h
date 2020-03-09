#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QtCore/QObject>
#include <QGraphicsPixmapItem>
#include "../build/ui_HistogramWindow.h"

class HistogramWindow :	public QWidget
{
	Q_OBJECT

public:
	HistogramWindow(QWidget* parent = Q_NULLPTR);
	~HistogramWindow();
	void setImage(QImage* targetImage);
	Ui::histogramWindow ui;
public slots:
	void plotHistogram(bool plotMinmax = false);
	void ActionStretch();
	void ActionApply();
	void ActionOpenCumHist();
	void ActionLockChannels();
signals:
	float sigStretch();
protected:
	void resizeEvent(QResizeEvent* event);
private:
	QImage* _targetImage = nullptr;
	QImage* _histogramPlot = nullptr;

	std::vector<int> _count_RED = std::vector<int>(256, 0);
	std::vector<int> _count_GREEN = std::vector<int>(256, 0);
	std::vector<int> _count_BLUE = std::vector<int>(256, 0);

	int _min_RED = 0; int _max_RED = 255;
	int _min_GREEN = 0; int _max_GREEN = 255;
	int _min_BLUE = 0; int _max_BLUE = 255;

	int _newMin_RED = 0; int _newMax_RED = 255;
	int _newMin_GREEN = 0; int _newMax_GREEN = 255;
	int _newMin_BLUE = 0; int _newMax_BLUE = 255;

	unsigned int _max_value = 1000;
	bool _grayscale = false;

	bool _channelsLocked = false;

	void drawLine(const QPoint& startPt, const QPoint& endPt, QColor color, int width, Qt::PenStyle style = Qt::SolidLine);
	void drawText(QString text, const QPoint& position, QColor color, int width);
	void clearCounters();

	void processImage();
	void displayImage(QImage* image);
	void getIntensityBounds(float percentile1 = 0.0f, float percentile2 = 1.0f);
	void contrastStretch();

	QImage getResized(QImage* image, const QSize& newSize, bool keepAspectRatio);

	void lockChannelUI(bool lock);
};

#endif