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
}

void HistogramWindow::processImage()
{
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
	if (ui.minSlider->value() >= ui.maxSlider->value()) {
		ui.minSlider->setValue(ui.maxSlider->value() - 1);
	}
	int val1 = ui.minSlider->value();
	int val2 = ui.maxSlider->value();
	ui.minLabel->setText(QString::number(val1));
	ui.maxLabel->setText(QString::number(val2));

	_newMin = val1; _newMax = val2;
	plotHistogram(true);
}

void HistogramWindow::ActionApply()
{
	contrastStretch();
	emit sigStretch();
	processImage();
	plotHistogram();
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
	float normalization_value = ((float)(_max_value));
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
		drawLine(QPoint(_min_RED * width_step, y_offset), QPoint(_min_RED * width_step, height - y_offset), QColor(0, 0, 0, 100), bound_width, Qt::DashLine);
		drawLine(QPoint(_max_RED * width_step, y_offset), QPoint(_max_RED * width_step, height - y_offset), QColor(0, 0, 0, 100), bound_width, Qt::DashLine);
	}
	else {
		drawLine(QPoint(_min_RED * width_step, y_offset), QPoint(_min_RED * width_step, height - y_offset), QColor(255, 0, 0, 100), bound_width, Qt::DashLine);
		drawLine(QPoint(_max_RED * width_step, y_offset), QPoint(_max_RED * width_step, height - y_offset), QColor(255, 0, 0, 100), bound_width, Qt::DashLine);

		drawLine(QPoint(_min_GREEN * width_step, y_offset), QPoint(_min_GREEN * width_step, height - y_offset), QColor(0, 255, 0, 100), bound_width, Qt::DashLine);
		drawLine(QPoint(_max_GREEN * width_step, y_offset), QPoint(_max_GREEN * width_step, height - y_offset), QColor(0, 255, 0, 100), bound_width, Qt::DashLine);

		drawLine(QPoint(_min_BLUE * width_step, y_offset), QPoint(_min_BLUE * width_step, height - y_offset), QColor(0, 0, 255, 100), bound_width, Qt::DashLine);
		drawLine(QPoint(_max_BLUE* width_step, y_offset), QPoint(_max_BLUE* width_step, height - y_offset), QColor(0, 0, 255, 100), bound_width, Qt::DashLine);
	}

	// controlled bounds
	if (plotMinmax) {
		drawLine(QPoint(_newMin * width_step, y_offset), QPoint(_newMin * width_step, height - y_offset), QColor(22, 130, 201, 100), bound_width, Qt::DashLine);
		drawLine(QPoint(_newMax * width_step, y_offset), QPoint(_newMax * width_step, height - y_offset), QColor(22, 130, 201, 100), bound_width, Qt::DashLine);
	}

	displayImage(_histogramPlot);
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
		if (cumsum >= sumL) {
			_min_RED = i;
			break;
		}
	}
	for (j = i + 1; j < 256; j++) {
		cumsum += _count_RED[j];
		if (cumsum >= sumU) {
			_max_RED = j;
			break;
		}
	}

	_minTotal = _min_RED; _maxTotal = _max_RED;

	if (!_grayscale) {
		// green channel
		cumsum = 0;
		for (i = 0; i < 256; i++) {
			cumsum += _count_GREEN[i];
			if (cumsum >= sumL) {
				_min_GREEN = i;
				break;
			}
		}
		for (j = i + 1; j < 256; j++) {
			cumsum += _count_GREEN[j];
			if (cumsum >= sumU) {
				_max_GREEN = j;
				break;
			}
		}
		_minTotal = _min_GREEN < _minTotal ? _min_GREEN : _minTotal;
		_maxTotal = _max_GREEN > _maxTotal ? _max_GREEN : _maxTotal;

		// blue channel
		cumsum = 0;
		for (i = 0; i < 256; i++) {
			cumsum += _count_BLUE[i];
			if (cumsum >= sumL) {
				_min_BLUE = i;
				break;
			}
		}
		for (j = i + 1; j < 256; j++) {
			cumsum += _count_BLUE[j];
			if (cumsum >= sumU) {
				_max_BLUE = j;
				break;
			}
		}

		_minTotal = _min_BLUE < _minTotal ? _min_BLUE : _minTotal;
		_maxTotal = _max_BLUE > _maxTotal ? _max_BLUE : _maxTotal;
	}

	// ui.minSlider->setValue(_minTotal); ui.maxSlider->setValue(_maxTotal);
	// ui.minLabel->setText(QString::number(_minTotal)); ui.maxLabel->setText(QString::number(_maxTotal));
}

// unsigned char newC = (data[i * row + j] - min)*(newMax - newMin) / (max - min) + newMin;

void HistogramWindow::contrastStretch()
{
	uint h = _targetImage->height();
	uint w = _targetImage->width();
	uint NPixels = w * h;
	float center = 0.5f * (_max_RED + _min_RED);
	float px, scale = (_newMax - _newMin) / ((float)(_max_RED - _min_RED));
	printf("scale factor = %f\n", scale);
	int pxOut;
	QImage stretchedImg = QImage(QSize(w, h), _targetImage->format());

	for (uint i = 0; i < NPixels; i++) {
		px = std::round((float)(_targetImage->pixelColor(i % w, i / w).red() - center) * scale + center);
		pxOut = (px < 0.0f) ? 0 : (px > 255.0 ? 255 : px);
		stretchedImg.setPixelColor(QPoint(i % w, i / w), QColor(pxOut, pxOut, pxOut));
	}

	if (!_grayscale) {
		float px2; int pxOut2;
		center = 0.5f * (_max_GREEN + _min_GREEN);
		scale = (_newMax - _newMin) / ((float)(_max_GREEN - _min_GREEN));
		float center2 = 0.5f * (_max_BLUE + _min_BLUE);
		float scale2 = (_newMax - _newMin) / ((float)(_max_BLUE - _min_BLUE));
		for (uint i = 0; i < NPixels; i++) {
			px = std::round((float)(_targetImage->pixelColor(i % w, i / w).green() - center) * scale + center);
			px2 = std::round((float)(_targetImage->pixelColor(i % w, i / w).blue() - center2) * scale2 + center2);
			pxOut = (px < 0.0f) ? 0 : (px > 255.0 ? 255 : px);
			pxOut2 = (px2 < 0.0f) ? 0 : (px2 > 255.0 ? 255 : px2);
			stretchedImg.pixelColor(i % w, i / w).setGreen(pxOut);
			stretchedImg.pixelColor(i % w, i / w).setBlue(pxOut2);
		}
	}

	*_targetImage = QImage(stretchedImg);
}
