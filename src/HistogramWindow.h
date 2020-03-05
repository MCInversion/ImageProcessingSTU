#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QWidget>
#include <QtCore/QObject>
#include <QGraphicsPixmapItem>
#include "ui_HistogramWindow.h"

class HistogramWindow :	public QWidget
{
	Q_OBJECT

public:
	HistogramWindow(QWidget* parent = Q_NULLPTR);
	~HistogramWindow();
	void processImage(QImage* targetImage);
	Ui::histogramWindow ui;
public slots:
	void plotHistogram();
	void ActionStretch();
	void ActionApply();
private:
	QImage* _targetImage = nullptr;
	QImage* _histogramPlot = nullptr;

	std::vector<int> _count_RED = std::vector<int>(256, 0);
	std::vector<int> _count_GREEN = std::vector<int>(256, 0);
	std::vector<int> _count_BLUE = std::vector<int>(256, 0);

	unsigned int _max_value = 1000;
	bool _grayscale = false;

	void drawLine(const QPoint& startPt, const QPoint& endPt, QColor color, int width);
	void drawText(QString text, const QPoint& position, QColor color, int width);

	void displayImage(QImage* image);
};

#endif