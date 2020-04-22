#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::ImageViewerClass)
{
	ui->setupUi(this);
	hideTimeControls();
	connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(ActionTabChanged()));
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

void ImageViewer::hideTimeControls()
{
	ui->timeSlider->hide();
	ui->timeLabel->hide();
}

void ImageViewer::enableTimeControls()
{
	ui->timeSlider->show();
	ui->timeLabel->show();
}

void ImageViewer::setUpTimeControls(int nSteps)
{
	enableTimeControls();
	ui->timeSlider->setRange(0, nSteps);
	ui->timeSlider->setValue(nSteps);
	setTimeLabel(nSteps);
}

void ImageViewer::setTimeLabel(int id)
{
	ui->timeLabel->setText("t = " + QString::number(id));
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
	NewImageDialog* newImgDialog = new NewImageDialog(this);
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
	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	ip->clearImage();
}

void ImageViewer::on_actionInvert_colors_triggered() {
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	ip->invertColors();
}

void ImageViewer::on_actionGrayscale_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}
	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	ip->grayscale();
}

void ImageViewer::on_actionMirror_Extend_test_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	MirrorExtendDialog* mirrorExtendDialog = new MirrorExtendDialog(this);
	connect(mirrorExtendDialog, SIGNAL(accepted()), ip, SLOT(mirrorExtendAccepted()));
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

	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	HistogramWindow* histogramWindow = new HistogramWindow(this);
	connect(histogramWindow, SIGNAL(sigStretch()), ip, SLOT(on_stretch()));
	connect(histogramWindow, SIGNAL(sigGrayscale()), ip, SLOT(on_histogramGrayscale()));
	connect(ip, SIGNAL(sigGrayscaled()), histogramWindow, SLOT(ActionIsodataCompute()));
	connect(histogramWindow, SIGNAL(sigThreshold()), ip, SLOT(on_singleThreshold()));

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

	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	BlurDialog* blurDialog = new BlurDialog(this);
	connect(blurDialog, SIGNAL(accepted()), ip, SLOT(blurAccepted()));
	blurDialog->exec();
}

void ImageViewer::on_actionThreshold_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	BernsenThresholdDialog* bernsenThresholdDialog = new BernsenThresholdDialog(this);
	connect(bernsenThresholdDialog, SIGNAL(accepted()), ip, SLOT(bernsenThresholdAccepted()));
	bernsenThresholdDialog->exec();
}

void ImageViewer::on_actionThreshold_Isodata_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	HistogramWindow* histogramWindow = new HistogramWindow(this);
	connect(histogramWindow, SIGNAL(sigStretch()), ip, SLOT(on_stretch()));
	connect(histogramWindow, SIGNAL(sigGrayscale()), ip, SLOT(on_histogramGrayscale()));
	connect(ip, SIGNAL(sigGrayscaled()), histogramWindow, SLOT(ActionIsodataCompute()));
	connect(histogramWindow, SIGNAL(sigThreshold()), ip, SLOT(on_singleThreshold()));

	ViewerWidget* w = getCurrentViewerWidget();
	histogramWindow->setImage(w->getImage());
	histogramWindow->plotHistogram();
	histogramWindow->show();

	ip->on_histogramGrayscale();
}

void ImageViewer::on_actionMulti_Blur_test_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	MultiBlurDialog* multiBlur = new MultiBlurDialog(this);
	connect(multiBlur, SIGNAL(accepted()), ip, SLOT(multiBlurAccepted()));
	connect(ip, SIGNAL(multiImageComplete()), this, SLOT(on_multiBlurControls()));
	multiBlur->exec();
}

void ImageViewer::on_multiBlurControls()
{
	ImageProcessor* ip = static_cast<ImageProcessor*>(sender());
	enableTimeControls();
	setUpTimeControls(ip->nSteps);
}

void ImageViewer::ActionTimeSlider()
{
	int sliderId = ui->timeSlider->value();
	setTimeLabel(sliderId);
	ViewerWidget* w = getCurrentViewerWidget();
	w->imgId = sliderId;
	w->setImage(w->imgArray[sliderId]);
	w->update();
}

void ImageViewer::ActionTabChanged()
{
	QTabWidget* tw = static_cast<QTabWidget*>(sender());
	int id = tw->currentIndex();
	ViewerWidget* vW = getViewerWidget(id);
	if (vW) {
		if (vW->imgArray.size() > 0) {
			enableTimeControls();
			ui->timeSlider->setRange(0, vW->imgArray.size() - 1);
			ui->timeSlider->setValue(vW->imgId);
			setTimeLabel(vW->imgId);
			vW->setImage(vW->imgArray[vW->imgId]);
		}
		else {
			hideTimeControls();
		}
	}
}

void ImageViewer::on_actionHeat_Equation_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	HeatEquationDialog* heqDialog = new HeatEquationDialog(this);
	connect(heqDialog, SIGNAL(accepted()), ip, SLOT(heatEquationAccepted()));
	connect(ip, SIGNAL(multiImageComplete()), this, SLOT(on_multiBlurControls()));
	heqDialog->exec();
}

void ImageViewer::on_actionPerona_Malik_triggered()
{
	if (!isImgOpened()) {
		msgBox.setText("No image is opened.");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return;
	}

	ImageProcessor* ip = new ImageProcessor(getCurrentViewerWidget());
	PeronaMalikDialog* pmDialog = new PeronaMalikDialog(this);
	connect(pmDialog, SIGNAL(accepted()), ip, SLOT(peronaMalikAccepted()));
	connect(ip, SIGNAL(multiImageComplete()), this, SLOT(on_multiBlurControls()));
	pmDialog->exec();
}
