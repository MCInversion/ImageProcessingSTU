#include "HistogramWindow.h"

HistogramWindow::HistogramWindow(QWidget* parent)
{
	ui.setupUi(this);
}

HistogramWindow::~HistogramWindow()
{
}

void HistogramWindow::processImage(QImage* targetImage)
{
	_targetImage = targetImage;
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
}

void HistogramWindow::ActionStretch()
{
	int val1 = ui.minSlider->value();
	int val2 = ui.maxSlider->value();
	printf("changed min %d, max %d\n", val1, val2);
}

void HistogramWindow::ActionApply()
{
	printf("bohaaa zrob nieco!\n");
}

void HistogramWindow::plotHistogram()
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

	for (int i = 0; i < 255; i++) {
		if (_grayscale) {

			normalizedValue0 = (float)_count_RED[i] / normalization_value;
			normalizedValue1 = (float)_count_RED[i + 1] / normalization_value;
			x0 = i * width_step, x1 = (i + 1) * width_step;
			y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
			y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
			drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(50, 50, 50), pen_width);

		} else {
			normalizedValue0 = (float)_count_RED[i] / normalization_value;
			normalizedValue1 = (float)_count_RED[i + 1] / normalization_value;
			x0 = i * width_step, x1 = (i + 1) * width_step;
			y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
			y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
			drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(255, 0, 0), pen_width);

			normalizedValue0 = (float)_count_GREEN[i] / normalization_value;
			normalizedValue1 = (float)_count_GREEN[i + 1] / normalization_value;
			x0 = i * width_step, x1 = (i + 1) * width_step;
			y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
			y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
			drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(0, 255, 0), pen_width);

			normalizedValue0 = (float)_count_BLUE[i] / normalization_value;
			normalizedValue1 = (float)_count_BLUE[i + 1] / normalization_value;
			x0 = i * width_step, x1 = (i + 1) * width_step;
			y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
			y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
			drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(0, 0, 255), pen_width);
		}
	}

	if (_grayscale) {

		normalizedValue0 = (float)_count_RED[255] / normalization_value;
		normalizedValue1 = 0.;
		x0 = 255 * width_step, x1 = 256 * width_step;
		y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
		y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
		drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(50, 50, 50), pen_width);

	} else {
		normalizedValue0 = (float)_count_RED[255] / normalization_value;
		normalizedValue1 = 0.;
		x0 = 255 * width_step, x1 = 256 * width_step;
		y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
		y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
		drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(255, 0, 0), pen_width);

		normalizedValue0 = (float)_count_GREEN[255] / normalization_value;
		normalizedValue1 = 0.;
		x0 = 255 * width_step, x1 = 256 * width_step;
		y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
		y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
		drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(0, 255, 0), pen_width);

		normalizedValue0 = (float)_count_BLUE[255] / normalization_value;
		normalizedValue1 = 0.;
		x0 = 255 * width_step, x1 = 256 * width_step;
		y0 = ((int)(normalizedValue0 * (height - 2 * y_offset) + 0.5)) + y_offset;
		y1 = ((int)(normalizedValue1 * (height - 2 * y_offset) + 0.5)) + y_offset;
		drawLine(QPoint(x0, height - y0), QPoint(x1, height - y1), QColor(0, 0, 255), pen_width);
	}

	displayImage(_histogramPlot);
}

void HistogramWindow::drawLine(const QPoint& startPt, const QPoint& endPt, QColor color, int width)
{
	QPainter painter(_histogramPlot);
	painter.setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
