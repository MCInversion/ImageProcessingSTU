#include "HistogramWindow.h"

HistogramWindow::HistogramWindow(QWidget* parent)
{
	ui.setupUi(this);
}

HistogramWindow::~HistogramWindow()
{
	delete _histogramPlot;
}

void HistogramWindow::setImage(QImage* targetImage)
{
	_targetImage = targetImage;
	processImage();
	lockChannelUI(_grayscale);
}

void HistogramWindow::processImage()
{
	this->clearCounters();
	int depth = _targetImage->depth();
	_grayscale = depth == 8;

	int height = _targetImage->height();
	int width = _targetImage->width();

	_max_value = 0;
	int red_id, green_id, blue_id;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (_grayscale) {
				red_id = _targetImage->pixelColor(x, y).value();
				_count_RED[red_id]++;
				if (_count_RED[red_id] > _max_value) _max_value = _count_RED[red_id];
			}
			else {
				red_id = _targetImage->pixelColor(x, y).red();
				green_id = _targetImage->pixelColor(x, y).green();
				blue_id = _targetImage->pixelColor(x, y).blue();
				_count_RED[red_id]++;
				_count_GREEN[green_id]++;
				_count_BLUE[blue_id]++;
				if (_count_RED[red_id] > _max_value) _max_value = _count_RED[red_id];
				if (_count_GREEN[green_id] > _max_value) _max_value = _count_GREEN[green_id];
				if (_count_BLUE[blue_id] > _max_value) _max_value = _count_BLUE[blue_id];
			}
		}
	}
	
	getIntensityBounds();
}

void HistogramWindow::ActionStretch()
{
	if (_grayscale) {
		if (ui.minSlider->value() >= ui.maxSlider->value()) {
			ui.minSlider->setValue(ui.maxSlider->value() - 1);
		}
		int val1 = ui.minSlider->value();
		int val2 = ui.maxSlider->value();
		ui.minLabel->setText(QString::number(val1));
		ui.maxLabel->setText(QString::number(val2));

		_newMin_RED = val1; _newMax_RED = val2;
		_newMin_GREEN = val1; _newMax_GREEN = val2;
		_newMin_BLUE = val1; _newMax_BLUE = val2;

	} else {
		if (ui.minGreenSlider->value() >= ui.maxGreenSlider->value()) {
			ui.minGreenSlider->setValue(ui.maxGreenSlider->value() - 1);
		}
		if (ui.minBlueSlider->value() >= ui.maxBlueSlider->value()) {
			ui.minBlueSlider->setValue(ui.maxBlueSlider->value() - 1);
		}

		int val1red = ui.minSlider->value(); int val2red = ui.maxSlider->value();
		int val1green = (_channelsLocked ? val1red : ui.minGreenSlider->value());
		int val2green = (_channelsLocked ? val2red : ui.maxGreenSlider->value());
		int val1blue = (_channelsLocked ? val1red : ui.minBlueSlider->value());
		int val2blue = (_channelsLocked ? val2red : ui.maxBlueSlider->value());

		ui.minLabel->setText(QString::number(val1red));
		ui.maxLabel->setText(QString::number(val2red));
		ui.minGreenLabel->setText(QString::number(val1green));
		ui.maxGreenLabel->setText(QString::number(val2green));
		ui.minBlueLabel->setText(QString::number(val1blue));
		ui.maxBlueLabel->setText(QString::number(val2blue));

		_newMin_RED = val1red; _newMax_RED = val2red;
		_newMin_GREEN = val1green; _newMax_GREEN = val2green;
		_newMin_BLUE = val1blue; _newMax_BLUE = val2blue;
	}

	plotHistogram(true);
}

void HistogramWindow::ActionApply()
{
	contrastStretch();
	emit sigStretch();

	processImage();
	plotHistogram(true);
}

void HistogramWindow::ActionOpenCumHist()
{
}

void HistogramWindow::ActionLockChannels()
{
	_channelsLocked = ui.checkBoxChannelLock->isChecked();
	lockChannelUI(_channelsLocked);
	plotHistogram(true);
}

void HistogramWindow::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

	if (_histogramPlot != nullptr) {
		QSize viewerSize = ui.histogramView->size();
		QImage displayedImg = getResized(_histogramPlot, viewerSize, true);
		displayImage(&displayedImg);
	}
}

void HistogramWindow::plotHistogram(bool plotMinmax)
{
	int pen_width = 4;

	int width = 800;
	int height = 600;
	int y_offset = 0.1 * height;
	_histogramPlot = new QImage(width, height, QImage::Format_RGB32);
	_histogramPlot->fill(qRgb(255, 255, 255));
	int width_step = ((int)(width / 256. + 0.5));
	float normalization_value = ((float)(1.2 * _max_value));
	float normalizedValue0, normalizedValue1;
	int x0, x1, y0, y1;
	drawLine(QPoint(0, height - y_offset), QPoint(width - 10 * width_step, height - y_offset), QColor(0, 0, 0), pen_width); // x-axis
	drawText(
		"px intensity",
		QPoint((int)(0.5 * (width - y_offset) + 0.5), (int)(height - 0.5 * y_offset)),
		QColor(0, 0, 0), 2 * pen_width
	);
	// ticks
	drawText(
		QString::number(0),
		QPoint(0, (int)(height - (1 - 0.2) * y_offset)),
		QColor(0, 0, 0), pen_width
	);
	for (int i = 1; i < 51; i++) {
		float tick_size = i % 2 == 0 ? 0.4 : 0.2;
		drawLine(
			QPoint(5 * i * width_step, (int)(height - (1 + 0.5 * tick_size) * y_offset)),
			QPoint(5 * i * width_step, (int)(height - (1 - 0.5 * tick_size) * y_offset)),
			QColor(0, 0, 0), pen_width);
		if (i % 10 == 0) {
			drawText(
				QString::number(5 * i),
				QPoint(5 * i * width_step, (int)(height - (1 - 0.2) * y_offset)),
				QColor(0, 0, 0), pen_width
			);
		}
	}
	drawText(
		QString::number(255),
		QPoint(width - 8 * width_step, (int)(height - y_offset)),
		QColor(0, 0, 0), pen_width
	);

	drawLine(QPoint(0, height - y_offset), QPoint(0, y_offset), QColor(0, 0, 0), pen_width); // y-axis
	drawLine(QPoint(width - 10 * width_step, height - y_offset), QPoint(width - 10 * width_step, y_offset), QColor(0, 0, 0), (int)(0.5 * pen_width + 0.5));
	drawLine(QPoint(0, y_offset), QPoint(width - 10 * width_step, y_offset), QColor(0, 0, 0), (int)(0.5 * pen_width + 0.5));

	for (int i = 0; i < 256; i++) {
		if (_grayscale) {

			normalizedValue0 = (float)_count_RED[i] / normalization_value;
			normalizedValue1 = (i < 255 ? (float)_count_RED[i + 1] : 0.0f) / normalization_value;
			x0 = i * width_step, x1 = (i + 1) * width_step;
			y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
			y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
			drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(50, 50, 50), pen_width);

		} else {
			normalizedValue0 = (float)_count_RED[i] / normalization_value;
			normalizedValue1 = (i < 255 ? (float)_count_RED[i + 1] : 0.0f) / normalization_value;
			x0 = i * width_step, x1 = (i + 1) * width_step;
			y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
			y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
			drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(255, 0, 0, 125), pen_width);

			normalizedValue0 = (float)_count_GREEN[i] / normalization_value;
			normalizedValue1 = (i < 255 ? (float)_count_GREEN[i + 1] : 0.0f) / normalization_value;
			x0 = i * width_step, x1 = (i + 1) * width_step;
			y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
			y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
			drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(0, 255, 0, 125), pen_width);

			normalizedValue0 = (float)_count_BLUE[i] / normalization_value;
			normalizedValue1 = (i < 255 ? (float)_count_BLUE[i + 1] : 0.0f) / normalization_value;
			x0 = i * width_step, x1 = (i + 1) * width_step;
			y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
			y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
			drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(0, 0, 255, 125), pen_width);
		}
	}

	// intensity bounds
	int bound_width = pen_width / 2;
	if (_grayscale) {
		drawLine(QPoint(_min_RED * width_step, y_offset), QPoint(_min_RED * width_step, height - y_offset), QColor(0, 0, 0, 100), bound_width, Qt::DotLine);
		drawLine(QPoint(_max_RED * width_step, y_offset), QPoint(_max_RED * width_step, height - y_offset), QColor(0, 0, 0, 100), bound_width, Qt::DotLine);
	}
	else {
		drawLine(QPoint(_min_RED * width_step, y_offset), QPoint(_min_RED * width_step, height - y_offset), QColor(255, 0, 0, 100), bound_width, Qt::DotLine);
		drawLine(QPoint(_max_RED * width_step, y_offset), QPoint(_max_RED * width_step, height - y_offset), QColor(255, 0, 0, 100), bound_width, Qt::DotLine);

		drawLine(QPoint(_min_GREEN * width_step, y_offset), QPoint(_min_GREEN * width_step, height - y_offset), QColor(0, 255, 0, 100), bound_width, Qt::DotLine);
		drawLine(QPoint(_max_GREEN * width_step, y_offset), QPoint(_max_GREEN * width_step, height - y_offset), QColor(0, 255, 0, 100), bound_width, Qt::DotLine);

		drawLine(QPoint(_min_BLUE * width_step, y_offset), QPoint(_min_BLUE * width_step, height - y_offset), QColor(0, 0, 255, 100), bound_width, Qt::DotLine);
		drawLine(QPoint(_max_BLUE* width_step, y_offset), QPoint(_max_BLUE* width_step, height - y_offset), QColor(0, 0, 255, 100), bound_width, Qt::DotLine);
	}

	// controlled bounds
	if (plotMinmax) {
		drawLine(QPoint(_newMin_RED * width_step, y_offset), QPoint(_newMin_RED * width_step, height - y_offset), (_grayscale ? QColor(22, 130, 201, 100) : QColor(255, 0, 0, 100)), bound_width, Qt::DashLine);
		drawLine(QPoint(_newMax_RED * width_step, y_offset), QPoint(_newMax_RED * width_step, height - y_offset), (_grayscale ? QColor(22, 130, 201, 100) : QColor(255, 0, 0, 100)), bound_width, Qt::DashLine);

		if (!_grayscale) {
			drawLine(QPoint(_newMin_GREEN * width_step, y_offset), QPoint(_newMin_GREEN * width_step, height - y_offset), QColor(0, 255, 0, 100), bound_width, Qt::DashLine);
			drawLine(QPoint(_newMax_GREEN * width_step, y_offset), QPoint(_newMax_GREEN * width_step, height - y_offset), QColor(0, 255, 0, 100), bound_width, Qt::DashLine);

			drawLine(QPoint(_newMin_BLUE * width_step, y_offset), QPoint(_newMin_BLUE * width_step, height - y_offset), QColor(0, 0, 255, 100), bound_width, Qt::DashLine);
			drawLine(QPoint(_newMax_BLUE * width_step, y_offset), QPoint(_newMax_BLUE * width_step, height - y_offset), QColor(0, 0, 255, 100), bound_width, Qt::DashLine);
		}
	}

	QSize viewerSize = ui.histogramView->size();
	QImage displayedImg = getResized(_histogramPlot, viewerSize, true);
	displayImage(&displayedImg);
}

void HistogramWindow::drawLine(const QPoint& startPt, const QPoint& endPt, QColor color, int width, Qt::PenStyle style)
{
	QPainter painter(_histogramPlot);
	painter.setPen(QPen(color, width, style, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(startPt, endPt);
}

void HistogramWindow::drawText(QString text, const QPoint& position, QColor color, int width)
{
	QPainter painter(_histogramPlot);
	painter.setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawText(position.x(), position.y(), 15 * width, 20 * width, Qt::AlignLeft, text);
}

void HistogramWindow::clearCounters()
{
	_count_RED.clear(); _count_GREEN.clear(); _count_BLUE.clear();
	_count_RED = std::vector<int>(256, 0);
	_count_GREEN = std::vector<int>(256, 0);
	_count_BLUE = std::vector<int>(256, 0);
}

void HistogramWindow::displayImage(QImage* image)
{
	QGraphicsScene* scene = new QGraphicsScene();
	QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
	QPixmap pixmap = QPixmap().fromImage(*image);
	scene->addPixmap(pixmap);
	ui.histogramView->setScene(scene);
	update();
}

void HistogramWindow::getIntensityBounds(float percentile1, float percentile2)
{
	uint NPixels = _targetImage->width() * _targetImage->height();
	uint sumL = std::round((float)NPixels * percentile1);
	uint sumU = std::round((float)NPixels * percentile2);

	// red (gray) channel
	uint cumsum = 0;
	int i, j;
	for (i = 0; i < 256; i++) {
		cumsum += _count_RED[i];
		if (cumsum > sumL) {
			_min_RED = i;
			break;
		}
	}
	for (j = i + 1; j < 256; j++) {
		cumsum += _count_RED[j];
		if (cumsum > sumU) {
			_max_RED = j;
			break;
		}
	}

	if (!_grayscale) {
		// green channel
		cumsum = 0;
		for (i = 0; i < 256; i++) {
			cumsum += _count_GREEN[i];
			if (cumsum > sumL) {
				_min_GREEN = i;
				break;
			}
		}
		for (j = i + 1; j < 256; j++) {
			cumsum += _count_GREEN[j];
			if (cumsum > sumU) {
				_max_GREEN = j;
				break;
			}
		}

		// blue channel
		cumsum = 0;
		for (i = 0; i < 256; i++) {
			cumsum += _count_BLUE[i];
			if (cumsum > sumL) {
				_min_BLUE = i;
				break;
			}
		}
		for (j = i + 1; j < 256; j++) {
			cumsum += _count_BLUE[j];
			if (cumsum > sumU) {
				_max_BLUE = j;
				break;
			}
		}
	}

	/*
	_newMin_RED = _min_RED; _newMax_RED = _max_RED;
	_newMin_GREEN = _min_GREEN; _newMax_GREEN = _max_GREEN;
	_newMin_BLUE = _min_BLUE; _newMax_BLUE = _max_BLUE;*/

	ui.minSlider->setValue(_min_RED); ui.maxSlider->setValue(_max_RED);
	ui.minLabel->setText(QString::number(_min_RED)); ui.maxLabel->setText(QString::number(_max_RED));

	if (!_grayscale) {
		ui.minGreenSlider->setValue(_min_GREEN); ui.maxGreenSlider->setValue(_max_GREEN);
		ui.minGreenLabel->setText(QString::number(_min_GREEN)); ui.maxGreenLabel->setText(QString::number(_max_GREEN));

		ui.minBlueSlider->setValue(_min_BLUE); ui.maxBlueSlider->setValue(_max_BLUE);
		ui.minBlueLabel->setText(QString::number(_min_BLUE)); ui.maxBlueLabel->setText(QString::number(_max_BLUE));
	}
}

void HistogramWindow::contrastStretch()
{
	uint h = _targetImage->height();
	uint w = _targetImage->width();
	uint NPixels = w * h;

	float pxR, pxG, pxB;
	float scaleR = 255.0f / (_newMax_RED - _newMin_RED);
	float scaleG = 255.0f / (_newMax_GREEN - _newMin_GREEN);
	float scaleB = 255.0f / (_newMax_BLUE - _newMin_BLUE);
	int pxOutR, pxOutG, pxOutB;
	QImage stretchedImg = QImage(QSize(w, h), _targetImage->format());

	for (uint i = 0; i < NPixels; i++) {
		pxR = std::round((float)(_targetImage->pixelColor(i % w, i / w).red() - _newMin_RED) * scaleR);
		pxOutR = (pxR < 0.0f) ? 0 : (pxR > 255.0 ? 255 : pxR);

		if (_grayscale) {
			*((uchar*)stretchedImg.scanLine(i / w) + i % w) = pxOutR;
		}
		else {
			pxG = std::round((float)(_targetImage->pixelColor(i % w, i / w).green() - _newMin_GREEN) * scaleG);
			pxB = std::round((float)(_targetImage->pixelColor(i % w, i / w).blue() - _newMin_BLUE) * scaleB);

			pxOutG = (pxG < 0.0f) ? 0 : (pxG > 255.0 ? 255 : pxG);
			pxOutB = (pxB < 0.0f) ? 0 : (pxB > 255.0 ? 255 : pxB);

			*((QRgb*)stretchedImg.scanLine(i / w) + i % w) = qRgb(pxOutR, pxOutG, pxOutB);
		}
	}

	*_targetImage = stretchedImg;
}

QImage HistogramWindow::getResized(QImage* image, const QSize& newSize, bool keepAspectRatio)
{
	return image->scaled(newSize, (keepAspectRatio ? Qt::KeepAspectRatio : Qt::IgnoreAspectRatio), Qt::FastTransformation);
}

void HistogramWindow::lockChannelUI(bool lock)
{
	if (lock) {
		// green channel ctrls:
		ui.label_3->hide(); ui.label_4->hide();
		ui.minGreenLabel->hide(); ui.maxGreenLabel->hide();
		ui.minGreenSlider->hide(); ui.maxGreenSlider->hide();
		// blue channel ctrls:
		ui.label_5->hide(); ui.label_7->hide();
		ui.minBlueLabel->hide(); ui.maxBlueLabel->hide();
		ui.minBlueSlider->hide(); ui.maxBlueSlider->hide();
		// red channel labels:
		ui.label->setText(QString("intensity min:")); ui.label_2->setText(QString("intensity max:"));	
	}
	else {
		// red channel ctrls:
		ui.label->setText(QString("red min:     ")); ui.label_2->setText(QString("red max:     "));
		// green channel ctrls:
		ui.label_3->show(); ui.label_4->show();
		ui.minGreenLabel->show(); ui.maxGreenLabel->show();
		ui.minGreenSlider->show(); ui.maxGreenSlider->show();
		// blue channel ctrls:
		ui.label_5->show(); ui.label_7->show();
		ui.minBlueLabel->show(); ui.maxBlueLabel->show();
		ui.minBlueSlider->show(); ui.maxBlueSlider->show();	
	}

	// lock channel switch:
	if (_grayscale) {
		ui.checkBoxChannelLock->hide();
	}
	else {
		ui.checkBoxChannelLock->show();
	}
}
