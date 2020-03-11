#pragma once

#include <QtWidgets/QMainWindow>
#include "../build/ui_ImageViewer.h"
#include "ViewerWidget.h"
#include "NewImageDialog.h"
#include "MirrorExtendDialog.h"
#include "HistogramWindow.h"

class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	ImageViewer(QWidget* parent = Q_NULLPTR);

private:
	Ui::ImageViewerClass* ui;
	NewImageDialog* newImgDialog;
	MirrorExtendDialog* mirrorExtendDialog;
	HistogramWindow* histogramWindow;

	QSettings settings;
	QMessageBox msgBox;

	//ViewerWidget functions
	ViewerWidget* getViewerWidget(int tabId);
	ViewerWidget* getCurrentViewerWidget();

	//Event filters
	bool eventFilter(QObject* obj, QEvent* event);

	//ViewerWidget Events
	bool ViewerWidgetEventFilter(QObject* obj, QEvent* event);
	void ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event);
	void ViewerWidgetMouseButtonRelease(ViewerWidget* w, QEvent* event);
	void ViewerWidgetMouseMove(ViewerWidget* w, QEvent* event);
	void ViewerWidgetLeave(ViewerWidget* w, QEvent* event);
	void ViewerWidgetEnter(ViewerWidget* w, QEvent* event);
	void ViewerWidgetWheel(ViewerWidget* w, QEvent* event);

	//ImageViewer Events
	void closeEvent(QCloseEvent* event);

	//Image functions
	void openNewTabForImg(ViewerWidget* vW);
	bool openImage(QString filename);
	bool saveImage(QString filename);
	bool clearImage();
	bool invertColors();
	bool mirrorExtendImageBy(int nPixels);
	bool blurImage(int radius);
	uchar kernelSum(uchar* img, int row, int x, int y, int r);
	QRgb kernelSum(uchar* img, int row, QPoint px, int r);

	std::vector<float> _weights = {
		1.f / 9.f,		1.f / 9.f,	   1.f / 9.f,
		1.f / 9.f,		1.f / 9.f,	   1.f / 9.f,
		1.f / 9.f,		1.f / 9.f,	   1.f / 9.f
	};

	std::vector<float> _weights2 = {
		1.f / 25.f,		1.f / 25.f,	   1.f / 25.f,     1.f / 25.f,     1.f / 25.f,		1.f / 25.f,
		1.f / 25.f,		1.f / 25.f,	   1.f / 25.f,     1.f / 25.f,     1.f / 25.f,		1.f / 25.f,
		1.f / 25.f,		1.f / 25.f,	   1.f / 25.f,     1.f / 25.f,     1.f / 25.f,		1.f / 25.f,
		1.f / 25.f,		1.f / 25.f,	   1.f / 25.f,     1.f / 25.f,     1.f / 25.f,		1.f / 25.f,
		1.f / 25.f,		1.f / 25.f,	   1.f / 25.f,     1.f / 25.f,     1.f / 25.f,		1.f / 25.f
	};

	std::vector<float> _weights5 = {
		0.00550747f,	0.0177061f,		0.026117f,		0.0177061f,		0.00550747f,
		0.0177061f,	    0.0569236f,		0.0839642f,		0.0569236f,		0.0177061f,
		0.026117f,		0.0839642f,		0.168302f,		0.0839642f,		0.026117f,
		0.0177061f,		0.0569236f,		0.0839642f,		0.0569236f,		0.0177061f,
		0.00550747f,	0.0177061f,		0.026117f,		0.0177061f,		0.00550747f
	};

	std::vector<float>* W = nullptr;

	//Processing actions:

	//Inline functions
	inline bool isImgOpened() { return ui->tabWidget->count() == 0 ? false : true; }

private slots:
	//Tabs slots
	void on_tabWidget_tabCloseRequested(int tabId);
	void on_actionRename_triggered();

	//Image slots
	void on_actionNew_triggered();
	void newImageAccepted();
	void mirrorExtendAccepted();
	void on_actionOpen_triggered();
	void on_actionSave_as_triggered();
	void on_actionClear_triggered();
	void on_actionInvert_colors_triggered();
	void on_actionMirror_Extend_test_triggered();
	void on_actionHistogram_triggered();
	void on_actionBlur_triggered();

	//Histogram slots
	void on_stretch();
};
