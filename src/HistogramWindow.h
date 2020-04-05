#ifndef HISTOGRAMWINDOW_H_
#define HISTOGRAMWINDOW_H_

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

	QImage* _targetImage = nullptr;
	QImage* _histogramPlot = nullptr;

	int threshold();
	void updateHistogram();
public slots:
	void plotHistogram(bool plotMinmax = false);
	void ActionStretch();
	void ActionApply();
	void ActionIsodata();
	void ActionIsodataCompute();

	void ActionDarkMode();
	void ActionCumulativeHist();
	void ActionLockChannels();
signals:
	float sigStretch();
	void sigGrayscale();
	int sigThreshold();
protected:
	void resizeEvent(QResizeEvent* event);
private:
	QGraphicsScene* _scene = nullptr;

	// data
	std::vector<int> _count_RED = std::vector<int>(256, 0);
	std::vector<int> _count_GREEN = std::vector<int>(256, 0);
	std::vector<int> _count_BLUE = std::vector<int>(256, 0);

	std::vector<float> _sums_RED = std::vector<float>(256, 0);
	std::vector<float> _sums_GREEN = std::vector<float>(256, 0);
	std::vector<float> _sums_BLUE = std::vector<float>(256, 0);

	// bounds
	int _min_RED = 0; int _max_RED = 255;
	int _min_GREEN = 0; int _max_GREEN = 255;
	int _min_BLUE = 0; int _max_BLUE = 255;

	int _newMin_RED = 0; int _newMax_RED = 255;
	int _newMin_GREEN = 0; int _newMax_GREEN = 255;
	int _newMin_BLUE = 0; int _newMax_BLUE = 255;

	unsigned int _max_value = 1000;

	// threshold
	int _threshold = 125;

	// flags
	bool _grayscale = false;
	bool _channelsLocked = true;
	bool _darkMode = true;
	bool _plotCumulative = false;
	bool _cumulativeComputed = false;

	// colors
	QColor _bgColor = QColor(11, 45, 77);
	QColor _axisColor = QColor(255, 255, 255);

	// drawers
	void drawLine(const QPoint& startPt, const QPoint& endPt, QColor color, int width, Qt::PenStyle style = Qt::SolidLine);
	void drawText(QString text, const QPoint& position, QColor color, int width);

	// cleanup
	void clearCounters();
	void clearSums();

	// processors
	void processImage();
	void displayImage(QImage* image);
	void getIntensityBounds(float percentile1 = 0.0f, float percentile2 = 1.0f);
	void contrastStretch();
	void getCumulativeSums();

	// thresholds
	int getIsodataThresholdRED();
	int getIsodataThresholdGREEN();
	int getIsodataThresholdBLUE();

	// stats
	int pixelMeanRED(int iMin, int iMax);
	int pixelMeanGREEN(int iMin, int iMax);
	int pixelMeanBLUE(int iMin, int iMax);

	QImage getResized(QImage* image, const QSize& newSize, bool keepAspectRatio);

	void lockChannelUI(bool lock);
};

#endif