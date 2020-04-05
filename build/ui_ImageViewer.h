/********************************************************************************
** Form generated from reading UI file 'ImageViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEWER_H
#define UI_IMAGEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageViewerClass
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave_as;
    QAction *actionRename;
    QAction *actionClear;
    QAction *actionInvert_colors;
    QAction *actionMirror_Extend_test;
    QAction *actionHistogram;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionBlur;
    QAction *actionThreshold;
    QAction *actionGrayscale;
    QAction *actionThreshold_Isodata;
    QAction *actionMulti_Blur_test;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuImage;
    QMenu *menuTools;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageViewerClass)
    {
        if (ImageViewerClass->objectName().isEmpty())
            ImageViewerClass->setObjectName(QString::fromUtf8("ImageViewerClass"));
        ImageViewerClass->resize(818, 617);
        actionNew = new QAction(ImageViewerClass);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionOpen = new QAction(ImageViewerClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave_as = new QAction(ImageViewerClass);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        actionRename = new QAction(ImageViewerClass);
        actionRename->setObjectName(QString::fromUtf8("actionRename"));
        actionClear = new QAction(ImageViewerClass);
        actionClear->setObjectName(QString::fromUtf8("actionClear"));
        actionClear->setEnabled(false);
        actionInvert_colors = new QAction(ImageViewerClass);
        actionInvert_colors->setObjectName(QString::fromUtf8("actionInvert_colors"));
        actionMirror_Extend_test = new QAction(ImageViewerClass);
        actionMirror_Extend_test->setObjectName(QString::fromUtf8("actionMirror_Extend_test"));
        actionHistogram = new QAction(ImageViewerClass);
        actionHistogram->setObjectName(QString::fromUtf8("actionHistogram"));
        actionUndo = new QAction(ImageViewerClass);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionUndo->setEnabled(false);
        actionRedo = new QAction(ImageViewerClass);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionRedo->setEnabled(false);
        actionBlur = new QAction(ImageViewerClass);
        actionBlur->setObjectName(QString::fromUtf8("actionBlur"));
        actionThreshold = new QAction(ImageViewerClass);
        actionThreshold->setObjectName(QString::fromUtf8("actionThreshold"));
        actionGrayscale = new QAction(ImageViewerClass);
        actionGrayscale->setObjectName(QString::fromUtf8("actionGrayscale"));
        actionThreshold_Isodata = new QAction(ImageViewerClass);
        actionThreshold_Isodata->setObjectName(QString::fromUtf8("actionThreshold_Isodata"));
        actionMulti_Blur_test = new QAction(ImageViewerClass);
        actionMulti_Blur_test->setObjectName(QString::fromUtf8("actionMulti_Blur_test"));
        centralWidget = new QWidget(ImageViewerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(true);
        tabWidget->setMovable(true);

        horizontalLayout->addWidget(tabWidget);

        ImageViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ImageViewerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 818, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuImage = new QMenu(menuBar);
        menuImage->setObjectName(QString::fromUtf8("menuImage"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        ImageViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImageViewerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ImageViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ImageViewerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ImageViewerClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuImage->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave_as);
        menuImage->addAction(actionRename);
        menuImage->addAction(actionClear);
        menuImage->addSeparator();
        menuImage->addAction(actionInvert_colors);
        menuImage->addAction(actionGrayscale);
        menuImage->addSeparator();
        menuImage->addAction(actionMirror_Extend_test);
        menuImage->addAction(actionBlur);
        menuImage->addAction(actionThreshold);
        menuImage->addAction(actionThreshold_Isodata);
        menuImage->addAction(actionMulti_Blur_test);
        menuTools->addAction(actionHistogram);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);

        retranslateUi(ImageViewerClass);

        QMetaObject::connectSlotsByName(ImageViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImageViewerClass)
    {
        ImageViewerClass->setWindowTitle(QCoreApplication::translate("ImageViewerClass", "ImageViewer", nullptr));
        actionNew->setText(QCoreApplication::translate("ImageViewerClass", "New", nullptr));
        actionOpen->setText(QCoreApplication::translate("ImageViewerClass", "Open", nullptr));
        actionSave_as->setText(QCoreApplication::translate("ImageViewerClass", "Save as", nullptr));
        actionRename->setText(QCoreApplication::translate("ImageViewerClass", "Rename", nullptr));
        actionClear->setText(QCoreApplication::translate("ImageViewerClass", "Clear", nullptr));
        actionInvert_colors->setText(QCoreApplication::translate("ImageViewerClass", "Invert colors", nullptr));
        actionMirror_Extend_test->setText(QCoreApplication::translate("ImageViewerClass", "Mirror Extend (test)", nullptr));
        actionHistogram->setText(QCoreApplication::translate("ImageViewerClass", "Histogram", nullptr));
        actionUndo->setText(QCoreApplication::translate("ImageViewerClass", "Undo", nullptr));
        actionRedo->setText(QCoreApplication::translate("ImageViewerClass", "Redo", nullptr));
        actionBlur->setText(QCoreApplication::translate("ImageViewerClass", "Blur", nullptr));
        actionThreshold->setText(QCoreApplication::translate("ImageViewerClass", "Threshold (Bernsen)", nullptr));
        actionGrayscale->setText(QCoreApplication::translate("ImageViewerClass", "Grayscale", nullptr));
        actionThreshold_Isodata->setText(QCoreApplication::translate("ImageViewerClass", "Threshold (Isodata)", nullptr));
        actionMulti_Blur_test->setText(QCoreApplication::translate("ImageViewerClass", "Multi-Blur (test)", nullptr));
        menuFile->setTitle(QCoreApplication::translate("ImageViewerClass", "File", nullptr));
        menuImage->setTitle(QCoreApplication::translate("ImageViewerClass", "Image", nullptr));
        menuTools->setTitle(QCoreApplication::translate("ImageViewerClass", "Tools", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("ImageViewerClass", "Edit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageViewerClass: public Ui_ImageViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEWER_H
