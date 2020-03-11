#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::ImageViewerClass)
{
	ui->setupUi(this);
}

//ViewerWidget functions
ViewerWidget* ImageViewer::getViewerWidget(int tabId)
{
	QScrollArea* s = static_cast<QScrollArea*>(ui->tabWidget->widget(tabId));
	if (s) {
		ViewerWidget* vW = static_cast<ViewerWidget*>(s->widget());
		return vW;
	}
	return nullptr;
}
ViewerWidget* ImageViewer::getCurrentViewerWidget()
{
	return getViewerWidget(ui->tabWidget->currentIndex());
}

// Event filters
bool ImageViewer::eventFilter(QObject* obj, QEvent* event)
{
	if (obj->objectName() == "ViewerWidget") {
		return ViewerWidgetEventFilter(obj, event);
	}
	return false;
}

//ViewerWidget Events
bool ImageViewer::ViewerWidgetEventFilter(QObject* obj, QEvent* event)
{
	ViewerWidget* w = static_cast<ViewerWidget*>(obj);

	if (!w) {
		return false;
	}

	if (event->type() == QEvent::MouseButtonPress) {
		ViewerWidgetMouseButtonPress(w, event);
	}
	else if (event->type() == QEvent::MouseButtonRelease) {
		ViewerWidgetMouseButtonRelease(w, event);
	}
	else if (event->type() == QEvent::MouseMove) {
		ViewerWidgetMouseMove(w, event);
	}
	else if (event->type() == QEvent::Leave) {
		ViewerWidgetLeave(w, event);
	}
	else if (event->type() == QEvent::Enter) {
		ViewerWidgetEnter(w, event);
	}
	else if (event->type() == QEvent::Wheel) {
		ViewerWidgetWheel(w, event);
	}

	return QObject::eventFilter(obj, event);
}
void ImageViewer::ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (e->button() == Qt::LeftButton) {
		w->setFreeDrawBegin(e->pos());
		w->setFreeDrawActivated(true);
	}
}
void ImageViewer::ViewerWidgetMouseButtonRelease(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (e->button() == Qt::LeftButton && w->getFreeDrawActivated()) {
		w->freeDraw(e->pos(), QPen(Qt::red));
		w->setFreeDrawActivated(false);
	}
}
void ImageViewer::ViewerWidgetMouseMove(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (e->buttons() == Qt::LeftButton && w->getFreeDrawActivated()) {
		w->freeDraw(e->pos(), QPen(Qt::red));
		w->setFreeDrawBegin(e->pos());
	}
}
void ImageViewer::ViewerWidgetLeave(ViewerWidget* w, QEvent* event)
{
}
void ImageViewer::ViewerWidgetEnter(ViewerWidget* w, QEvent* event)
{
}
void ImageViewer::ViewerWidgetWheel(ViewerWidget* w, QEvent* event)
{
	QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
}

//ImageViewer Events
void ImageViewer::closeEvent(QCloseEvent* event)
{
	if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No))
	{
		event->accept();
	}
	else {
		event->ignore();
	}
}

//Image functions
void ImageViewer::openNewTabForImg(ViewerWidget* vW)
{
	QScrollArea* scrollArea = new QScrollArea;
	scrollArea->setObjectName("QScrollArea");
	scrollArea->setWidget(vW);

	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidgetResizable(true);
	scrollArea->installEventFilter(this);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	vW->setObjectName("ViewerWidget");
	vW->installEventFilter(this);

	QString name = vW->getName();

	ui->tabWidget->addTab(scrollArea, name);
}
bool ImageViewer::openImage(QString filename)
{
	QFileInfo fi(filename);

	QString name = fi.baseName();
	openNewTabForImg(new ViewerWidget(name, QSize(0, 0)));
	ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

	ViewerWidget* w = getCurrentViewerWidget();

	QImage loadedImg(filename);
	return w->setImage(loadedImg);
}
bool ImageViewer::saveImage(QString filename)
{
	QFileInfo fi(filename);
	QString extension = fi.completeSuffix();
	ViewerWidget* w = getCurrentViewerWidget();

	QImage* img = w->getImage();
	return img->save(filename, extension.toStdString().c_str());
}
bool ImageViewer::clearImage()
{
	ViewerWidget* w = getCurrentViewerWidget();
	w->clear();
	w->update();
	return true;
}
bool ImageViewer::invertColors()
{
	ViewerWidget* w = getCurrentViewerWidget();
	uchar* data = w->getData();
	
	int row = w->getImage()->bytesPerLine();
	int depth = w->getImage()->depth();

	for (int i = 0; i < w->getImgHeight(); i++)
	{
		for (int j = 0; j < w->getImgWidth(); j++)
		{
			if (depth == 8) {
				w->setPixel(j, i, static_cast<uchar>(255 - data[i * row + j ]));
			}
			else {
				uchar r = static_cast<uchar>(255 - data[i * row + j * 4]);
				uchar g = static_cast<uchar>(255 - data[i * row + j * 4 + 1]);
				uchar b = static_cast<uchar>(255 - data[i * row + j * 4 + 2]);
				w->setPixel(j, i, r, g, b);
			}			
		}
	}
	w->update();
	return true;
}

bool ImageViewer::mirrorExtendImageBy(int nPixels)
{
	ViewerWidget* w = getCurrentViewerWidget();
	int oldDataSize = w->getImage()->sizeInBytes();
	uchar* data = w->getData();

	int row = w->getImage()->bytesPerLine();
	int depth = w->getImage()->depth();

	const int oldHeight = w->getImgHeight();
	const int oldWidth = w->getImgWidth();
	const int newHeight = oldHeight + 2 * nPixels;
	const int newWidth = oldWidth + 2 * nPixels;

	QImage::Format format = w->getImage()->format();
	QImage newImg = QImage(QSize(newWidth, newHeight), format);
	newImg.fill(QColor(0, 0, 0));
	w->setImage(newImg);

	for (int i = 0; i < newHeight; i++) {
		for (int j = 0; j < newWidth; j++) {

			int iPos; int jPos;
			// upper left corner
			if (i < nPixels && j < nPixels) {
				iPos = nPixels - 1 - i;
				jPos = nPixels - 1 - j;
			}
			// top strip
			else if (i < nPixels && j >= nPixels && j < newWidth - nPixels) {
				iPos = nPixels - 1 - i;
				jPos = j - nPixels;
			}
			// top right corner
			else if (i < nPixels && newWidth - j <= nPixels) {
				iPos = nPixels - 1 - i;
				jPos = nPixels + 2 * oldWidth - 1 - j;
			}
			// left strip
			else if (i >= nPixels && i < nPixels + oldHeight && j < nPixels) {
				iPos = i - nPixels;
				jPos = nPixels - 1 - j;
			}
			// right strip
			else if (i >= nPixels && i < nPixels + oldHeight && j >= newWidth - nPixels) {
				iPos = i - nPixels;
				jPos = nPixels + 2 * oldWidth - 1 - j;
			}
			// bottom left corner
			else if (i >= nPixels + oldHeight && j < nPixels) {
				iPos = nPixels + 2 * oldHeight - 1 - i;
				jPos = nPixels - 1 - j;
			}
			// bottom strip
			else if (i >= nPixels + oldHeight && j >= nPixels && j < newWidth - nPixels) {
				iPos = nPixels + 2 * oldHeight - 1 - i;
				jPos = j - nPixels;
			}
			// bottom right corner
			else if (i >= nPixels + oldHeight && j >= newWidth - nPixels) {
				iPos = nPixels + 2 * oldHeight - 1 - i;
				jPos = nPixels + 2 * oldWidth - 1 - j;
			}
			// inside old image
			else {
				iPos = i - nPixels;
				jPos = j - nPixels;
			}

			if (depth == 8) {
				w->setPixel(j, i, static_cast<uchar>(data[iPos * row + jPos]));
			}
			else {
				uchar r = static_cast<uchar>(data[iPos * row + jPos * 4]);
				uchar g = static_cast<uchar>(data[iPos * row + jPos * 4 + 1]);
				uchar b = static_cast<uchar>(data[iPos * row + jPos * 4 + 2]);
				w->setPixel(j, i, r, g, b);
			}
		}
	}

	w->update();
	return true;
}

uchar ImageViewer::kernelSum(uchar* img, int row, int x, int y, int r)
{
	float sum = 0.0f;
	for (int i = 0; i < 2 * r + 1; i++) {
		for (int j = 0; j < 2 * r + 1; j++) {
			int xPos = x - r + j, yPos = y - r + i;
			sum += ((float)img[yPos * row + xPos]) * W->at((size_t)i * (2 * r + 1) + j);
		}
	}

	return (uchar)std::fminf(sum + 0.5, 255.f);
}

QRgb ImageViewer::kernelSum(uchar* img, int row, QPoint px, int r)
{
	float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
	for (int i = 0; i < 2 * r + 1; i++) {
		for (int j = 0; j < 2 * r + 1; j++) {
			int xPos = px.x() - r + j, yPos = px.y() - r + i;
			sumR += ((float)img[yPos * row + xPos * 4]) * W->at((size_t)i * (2 * r + 1) + j);
			sumG += ((float)img[yPos * row + xPos * 4 + 1]) * W->at((size_t)i * (2 * r + 1) + j);
			sumB += ((float)img[yPos * row + xPos * 4 + 2]) * W->at((size_t)i * (2 * r + 1) + j);
		}
	}

	return qRgb(
		(int)std::fminf(sumR + 0.5, 255.f),
		(int)std::fminf(sumG + 0.5, 255.f),
		(int)std::fminf(sumB + 0.5, 255.f)
	);
}

bool ImageViewer::blurImage(int radius)
{
	ViewerWidget* w = getCurrentViewerWidget();
	int height = w->getImgHeight();
	int width = w->getImgWidth();
	uchar* data = w->getData();

	int row = w->getImage()->bytesPerLine();
	int depth = w->getImage()->depth();

	QImage blurredImg = QImage(QSize(width - 2 * radius, height - 2 * radius), w->getImage()->format());
	blurredImg.fill(QColor(0, 0, 0));
	w->setImage(blurredImg);

	for (int x = 0; x < blurredImg.width(); x++) {
		for (int y = 0; y < blurredImg.height(); y++) {
			int xPos = x + radius, yPos = y + radius;
			if (depth == 8) {
				w->setPixel(x, y, static_cast<uchar>(kernelSum(data, row, xPos, yPos, radius)));
			}
			else {
				QColor col = kernelSum(data, row, QPoint(xPos, yPos), radius);
				w->setPixel(x, y, col.red(), col.green(), col.blue());
			}
		}
	}

	w->update();
	return true;
}

void ImageViewer::getGaussianKernel(int radius, bool print)
{
	if (W) delete W;
	const int size = (2 * radius + 1) * (2 * radius + 1);
	W = new std::vector<float>(size, 0.0f);	
	
	float sigma = radius / 3.0f, h = 1.0f;
	float x0, x1, y0, y1;
	std::vector<float> w = {};

	// symmetrical (triangular) slice of the kernel
	for (int i = 0; i <= radius; i++) {
		for (int j = 0; j <= i; j++) {
			x0 = (2 * i - 1) * h / 2; x1 = (2 * i + 1) * h / 2;
			y0 = (2 * j - 1) * h / 2; y1 = (2 * j + 1) * h / 2;
			w.push_back( 0.25f * (erf(x1 / (M_SQRT2 * sigma)) - erf(x0 / (M_SQRT2 * sigma))) * (erf(y1 / (M_SQRT2 * sigma)) - erf(y0 / (M_SQRT2 * sigma))) );
		}
	}

	// compose kernel from symmetrical slices
	for (int sy = -1; sy <= 1; sy += 2) {
		for (int sx = -1; sx <= 1; sx += 2) {

			float** Wsym = new float* [(size_t)radius + 1];
			for (int i = 0; i <= radius; i++) Wsym[i] = new float[(size_t)radius + 1]();

			// compose symmetrical sub-kernel
			for (int i = 0; i <= radius; i++) {
				for (int j = 0; j <= radius; j++) {
					int iPos = (sy > 0 ? i : radius - i); int jPos = (sx > 0 ? j : radius - j);
					int id = iPos * (iPos + 1) / 2 + jPos;
					Wsym[i][j] = w[id];
				}
			}

			// corner bounds
			int iMin = ((1 + sy) / 2) * radius, iMax = ((1 + sy) / 2 + 1) * radius;
			int jMin = ((1 + sx) / 2) * radius, jMax = ((1 + sx) / 2 + 1) * radius;

			for (int iy = iMin, i = 0; iy <= iMax; iy++, i++) {
				for (int jx = jMin, j = 0; jx <= jMax; jx++, j++) {
					int id = iy * (2 * radius + 1) + jx;
					W->at(id) = Wsym[i][j];
				}
			}

			// cleanup
			for (int i = 0; i <= radius; i++) delete[] Wsym[i];
			delete[] Wsym;
		}
	}

	// count weights
	float wSum = 0.0f;
	for (int i = 0; i <= 2 * radius; i++) {
		for (int j = 0; j <= 2 * radius; j++) {
			int id = i * (2 * radius + 1) + j;
			wSum += W->at(id);
		}
	}

	// normalize
	for (int i = 0; i <= 2 * radius; i++) {
		for (int j = 0; j <= 2 * radius; j++) {
			int id = i * (2 * radius + 1) + j;
			W->at(id) /= wSum;
		}
	}

	w.clear();

	if (print) {
		printf("K = \n");
		for (int i = 0; i <= 2 * radius; i++) {
			for (int j = 0; j <= 2 * radius; j++) {
				int id = i * (2 * radius + 1) + j;
				printf("%f ", W->at(id));
			}
			printf("\n");
		}
	}
}

void ImageViewer::getAveragingKernel(int radius, bool print)
{
	if (W) delete W;
	int size = (2 * radius + 1) * (2 * radius + 1);
	float pxWeight = 1.0f / size;
	W = new std::vector<float>(size, pxWeight);

	if (print) {
		printf("K = \n");
		for (int i = 0; i <= 2 * radius; i++) {
			for (int j = 0; j <= 2 * radius; j++) {
				int id = i * (2 * radius + 1) + j;
				printf("%f ", W->at(id));
			}
			printf("\n");
		}
	}
}

//Slots

//Tabs slots
void ImageViewer::on_tabWidget_tabCloseRequested(int tabId)
{
	ViewerWidget* vW = getViewerWidget(tabId);
	vW->~ViewerWidget();
	ui->tabWidget->removeTab(tabId);
}
void ImageViewer::on_actionRename_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	ViewerWidget* w = getCurrentViewerWidget();
	bool ok;
	QString text = QInputDialog::getText(this, QString("Rename"), tr("Image name:"), QLineEdit::Normal, w->getName(), &ok);
	if (ok && !text.trimmed().isEmpty())
	{
		w->setName(text);
		ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), text);
	}
}

//Image slots
void ImageViewer::on_actionNew_triggered()
{
	newImgDialog = new NewImageDialog(this);
	connect(newImgDialog, SIGNAL(accepted()), this, SLOT(newImageAccepted()));
	newImgDialog->exec();
}
void ImageViewer::newImageAccepted()
{
	NewImageDialog* newImgDialog = static_cast<NewImageDialog*>(sender());

	int width = newImgDialog->getWidth();
	int height = newImgDialog->getHeight();
	QString name = newImgDialog->getName();
	openNewTabForImg(new ViewerWidget(name, QSize(width, height)));
	ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void ImageViewer::mirrorExtendAccepted()
{
	MirrorExtendDialog* mirrorExtendDialog = static_cast<MirrorExtendDialog*>(sender());

	int nPixels = mirrorExtendDialog->getPixels();
	mirrorExtendImageBy(nPixels);
}

void ImageViewer::blurAccepted()
{
	BlurDialog* blurDialog = static_cast<BlurDialog*>(sender());

	int radius = blurDialog->getRadius();
	bool printToConsole = blurDialog->printKernelToConsole();
	int kerType = blurDialog->getKernelType();
	// 0 - Gaussian, 1 - Averaging
	if (kerType == 0) {
		getGaussianKernel(radius, printToConsole);
	}
	else {
		getAveragingKernel(radius, printToConsole);
	}
	
	ViewerWidget* w = getCurrentViewerWidget();
	QImage* img = w->getImage();
	QImage extended = QImage(*img);
	w->setImage(extended);
	mirrorExtendImageBy(radius);
	blurImage(radius);
}

void ImageViewer::on_actionOpen_triggered()
{
	QString folder = settings.value("folder_img_load_path", "").toString();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getOpenFileName(this, "Load image", folder, fileFilter);
	if (fileName.isEmpty()) { return; }

	QFileInfo fi(fileName);
	settings.setValue("folder_img_load_path", fi.absoluteDir().absolutePath());

	if (!openImage(fileName)) {
		msgBox.setText("Unable to open image.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}	
}
void ImageViewer::on_actionSave_as_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image to save.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	QString folder = settings.value("folder_img_save_path", "").toString();

	ViewerWidget* w = getCurrentViewerWidget();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getSaveFileName(this, "Save image", folder + "/" + w->getName(), fileFilter);
	if (fileName.isEmpty()) { return; }

	QFileInfo fi(fileName);
	settings.setValue("folder_img_save_path", fi.absoluteDir().absolutePath());

	if (!saveImage(fileName)) {
		msgBox.setText("Unable to save image.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}
	else {
		msgBox.setText(QString("File %1 saved.").arg(fileName));
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
	}
}
void ImageViewer::on_actionClear_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	clearImage();
}
void ImageViewer::on_actionInvert_colors_triggered() {
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	invertColors();
}

void ImageViewer::on_actionMirror_Extend_test_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	mirrorExtendDialog = new MirrorExtendDialog(this);
	connect(mirrorExtendDialog, SIGNAL(accepted()), this, SLOT(mirrorExtendAccepted()));
	mirrorExtendDialog->exec();
}

void ImageViewer::on_actionHistogram_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	histogramWindow = new HistogramWindow(this);
	connect(histogramWindow, SIGNAL(sigStretch()), this, SLOT(on_stretch()));

	ViewerWidget* w = getCurrentViewerWidget();
	histogramWindow->setImage(w->getImage());
	histogramWindow->plotHistogram();
	histogramWindow->show();
}

void ImageViewer::on_actionBlur_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	blurDialog = new BlurDialog(this);
	connect(blurDialog, SIGNAL(accepted()), this, SLOT(blurAccepted()));
	blurDialog->exec();
}

void ImageViewer::on_stretch()
{
	ViewerWidget* w = getCurrentViewerWidget();
	w->setDataPtr();
	w->update();
}
