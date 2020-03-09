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

	// ImageProcessor imProc = ImageProcessor();

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

	//Histogram slots
	void on_stretch();
};
