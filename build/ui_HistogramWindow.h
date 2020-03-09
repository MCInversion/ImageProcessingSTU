/********************************************************************************
** Form generated from reading UI file 'HistogramWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTOGRAMWINDOW_H
#define UI_HISTOGRAMWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_histogramWindow
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hLayoutButtons;
    QCheckBox *checkBoxChannelLock;
    QPushButton *applyButton;
    QPushButton *openCumHistButton;
    QHBoxLayout *hLayoutGreen;
    QLabel *label_3;
    QLabel *minGreenLabel;
    QSlider *minGreenSlider;
    QLabel *label_4;
    QLabel *maxGreenLabel;
    QSlider *maxGreenSlider;
    QHBoxLayout *hLayoutRed;
    QLabel *label;
    QLabel *minLabel;
    QSlider *minSlider;
    QLabel *label_2;
    QLabel *maxLabel;
    QSlider *maxSlider;
    QHBoxLayout *hLayoutBlue;
    QLabel *label_5;
    QLabel *minBlueLabel;
    QSlider *minBlueSlider;
    QLabel *label_7;
    QLabel *maxBlueLabel;
    QSlider *maxBlueSlider;
    QGraphicsView *histogramView;

    void setupUi(QWidget *histogramWindow)
    {
        if (histogramWindow->objectName().isEmpty())
            histogramWindow->setObjectName(QString::fromUtf8("histogramWindow"));
        histogramWindow->resize(915, 600);
        gridLayout = new QGridLayout(histogramWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hLayoutButtons = new QHBoxLayout();
        hLayoutButtons->setObjectName(QString::fromUtf8("hLayoutButtons"));
        checkBoxChannelLock = new QCheckBox(histogramWindow);
        checkBoxChannelLock->setObjectName(QString::fromUtf8("checkBoxChannelLock"));

        hLayoutButtons->addWidget(checkBoxChannelLock);

        applyButton = new QPushButton(histogramWindow);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        hLayoutButtons->addWidget(applyButton);

        openCumHistButton = new QPushButton(histogramWindow);
        openCumHistButton->setObjectName(QString::fromUtf8("openCumHistButton"));

        hLayoutButtons->addWidget(openCumHistButton);


        gridLayout->addLayout(hLayoutButtons, 7, 0, 1, 1);

        hLayoutGreen = new QHBoxLayout();
        hLayoutGreen->setObjectName(QString::fromUtf8("hLayoutGreen"));
        label_3 = new QLabel(histogramWindow);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        hLayoutGreen->addWidget(label_3);

        minGreenLabel = new QLabel(histogramWindow);
        minGreenLabel->setObjectName(QString::fromUtf8("minGreenLabel"));

        hLayoutGreen->addWidget(minGreenLabel);

        minGreenSlider = new QSlider(histogramWindow);
        minGreenSlider->setObjectName(QString::fromUtf8("minGreenSlider"));
        minGreenSlider->setMaximum(255);
        minGreenSlider->setOrientation(Qt::Horizontal);

        hLayoutGreen->addWidget(minGreenSlider);

        label_4 = new QLabel(histogramWindow);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        hLayoutGreen->addWidget(label_4);

        maxGreenLabel = new QLabel(histogramWindow);
        maxGreenLabel->setObjectName(QString::fromUtf8("maxGreenLabel"));

        hLayoutGreen->addWidget(maxGreenLabel);

        maxGreenSlider = new QSlider(histogramWindow);
        maxGreenSlider->setObjectName(QString::fromUtf8("maxGreenSlider"));
        maxGreenSlider->setMaximum(255);
        maxGreenSlider->setValue(255);
        maxGreenSlider->setOrientation(Qt::Horizontal);

        hLayoutGreen->addWidget(maxGreenSlider);


        gridLayout->addLayout(hLayoutGreen, 3, 0, 1, 1);

        hLayoutRed = new QHBoxLayout();
        hLayoutRed->setObjectName(QString::fromUtf8("hLayoutRed"));
        label = new QLabel(histogramWindow);
        label->setObjectName(QString::fromUtf8("label"));

        hLayoutRed->addWidget(label);

        minLabel = new QLabel(histogramWindow);
        minLabel->setObjectName(QString::fromUtf8("minLabel"));

        hLayoutRed->addWidget(minLabel);

        minSlider = new QSlider(histogramWindow);
        minSlider->setObjectName(QString::fromUtf8("minSlider"));
        minSlider->setMaximum(255);
        minSlider->setOrientation(Qt::Horizontal);

        hLayoutRed->addWidget(minSlider);

        label_2 = new QLabel(histogramWindow);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        hLayoutRed->addWidget(label_2);

        maxLabel = new QLabel(histogramWindow);
        maxLabel->setObjectName(QString::fromUtf8("maxLabel"));

        hLayoutRed->addWidget(maxLabel);

        maxSlider = new QSlider(histogramWindow);
        maxSlider->setObjectName(QString::fromUtf8("maxSlider"));
        maxSlider->setMaximum(255);
        maxSlider->setValue(255);
        maxSlider->setOrientation(Qt::Horizontal);

        hLayoutRed->addWidget(maxSlider);


        gridLayout->addLayout(hLayoutRed, 1, 0, 1, 1);

        hLayoutBlue = new QHBoxLayout();
        hLayoutBlue->setObjectName(QString::fromUtf8("hLayoutBlue"));
        label_5 = new QLabel(histogramWindow);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        hLayoutBlue->addWidget(label_5);

        minBlueLabel = new QLabel(histogramWindow);
        minBlueLabel->setObjectName(QString::fromUtf8("minBlueLabel"));

        hLayoutBlue->addWidget(minBlueLabel);

        minBlueSlider = new QSlider(histogramWindow);
        minBlueSlider->setObjectName(QString::fromUtf8("minBlueSlider"));
        minBlueSlider->setMaximum(255);
        minBlueSlider->setOrientation(Qt::Horizontal);

        hLayoutBlue->addWidget(minBlueSlider);

        label_7 = new QLabel(histogramWindow);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        hLayoutBlue->addWidget(label_7);

        maxBlueLabel = new QLabel(histogramWindow);
        maxBlueLabel->setObjectName(QString::fromUtf8("maxBlueLabel"));

        hLayoutBlue->addWidget(maxBlueLabel);

        maxBlueSlider = new QSlider(histogramWindow);
        maxBlueSlider->setObjectName(QString::fromUtf8("maxBlueSlider"));
        maxBlueSlider->setMaximum(255);
        maxBlueSlider->setValue(255);
        maxBlueSlider->setOrientation(Qt::Horizontal);

        hLayoutBlue->addWidget(maxBlueSlider);


        gridLayout->addLayout(hLayoutBlue, 5, 0, 1, 1);

        histogramView = new QGraphicsView(histogramWindow);
        histogramView->setObjectName(QString::fromUtf8("histogramView"));

        gridLayout->addWidget(histogramView, 0, 0, 1, 1);


        retranslateUi(histogramWindow);
        QObject::connect(maxSlider, SIGNAL(valueChanged(int)), histogramWindow, SLOT(ActionStretch()));
        QObject::connect(minSlider, SIGNAL(valueChanged(int)), histogramWindow, SLOT(ActionStretch()));
        QObject::connect(minGreenSlider, SIGNAL(sliderMoved(int)), histogramWindow, SLOT(ActionStretch()));
        QObject::connect(maxGreenSlider, SIGNAL(sliderMoved(int)), histogramWindow, SLOT(ActionStretch()));
        QObject::connect(maxBlueSlider, SIGNAL(sliderMoved(int)), histogramWindow, SLOT(ActionStretch()));
        QObject::connect(openCumHistButton, SIGNAL(clicked()), histogramWindow, SLOT(ActionOpenCumHist()));
        QObject::connect(minBlueSlider, SIGNAL(sliderMoved(int)), histogramWindow, SLOT(ActionStretch()));
        QObject::connect(checkBoxChannelLock, SIGNAL(clicked(bool)), histogramWindow, SLOT(ActionLockChannels()));
        QObject::connect(applyButton, SIGNAL(clicked()), histogramWindow, SLOT(ActionApply()));

        QMetaObject::connectSlotsByName(histogramWindow);
    } // setupUi

    void retranslateUi(QWidget *histogramWindow)
    {
        histogramWindow->setWindowTitle(QCoreApplication::translate("histogramWindow", "Histogram", nullptr));
        checkBoxChannelLock->setText(QCoreApplication::translate("histogramWindow", "Channel lock", nullptr));
        applyButton->setText(QCoreApplication::translate("histogramWindow", "Scale", nullptr));
        openCumHistButton->setText(QCoreApplication::translate("histogramWindow", "Cumulative Histogram", nullptr));
        label_3->setText(QCoreApplication::translate("histogramWindow", "green min:   ", nullptr));
        minGreenLabel->setText(QCoreApplication::translate("histogramWindow", "0", nullptr));
        label_4->setText(QCoreApplication::translate("histogramWindow", "green max:   ", nullptr));
        maxGreenLabel->setText(QCoreApplication::translate("histogramWindow", "255", nullptr));
        label->setText(QCoreApplication::translate("histogramWindow", "intensity min:", nullptr));
        minLabel->setText(QCoreApplication::translate("histogramWindow", "0", nullptr));
        label_2->setText(QCoreApplication::translate("histogramWindow", "intensity max:", nullptr));
        maxLabel->setText(QCoreApplication::translate("histogramWindow", "255", nullptr));
        label_5->setText(QCoreApplication::translate("histogramWindow", "blue min:     ", nullptr));
        minBlueLabel->setText(QCoreApplication::translate("histogramWindow", "0", nullptr));
        label_7->setText(QCoreApplication::translate("histogramWindow", "blue max:     ", nullptr));
        maxBlueLabel->setText(QCoreApplication::translate("histogramWindow", "255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class histogramWindow: public Ui_histogramWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTOGRAMWINDOW_H
