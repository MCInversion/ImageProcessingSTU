#pragma once

#include <QWidget>
#include <QGraphicsPixmapItem>

#include "ui_HistogramWindow.h"

class HistogramWindow :	public QWidget
{
public:
	HistogramWindow(QWidget* parent = Q_NULLPTR)
	{
		histogramWindowUi.setupUi(this);
	}
	void processImage(QImage& targetImage);
private:
	Ui::HistogramWindow histogramWindowUi;
};

