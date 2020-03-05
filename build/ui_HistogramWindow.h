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
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *minLabel;
    QSlider *minSlider;
    QGraphicsView *histogramView;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLabel *maxLabel;
    QSlider *maxSlider;
    QPushButton *applyButton;

    void setupUi(QWidget *histogramWindow)
    {
        if (histogramWindow->objectName().isEmpty())
            histogramWindow->setObjectName(QString::fromUtf8("histogramWindow"));
        histogramWindow->resize(592, 437);
        gridLayout = new QGridLayout(histogramWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(histogramWindow);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        minLabel = new QLabel(histogramWindow);
        minLabel->setObjectName(QString::fromUtf8("minLabel"));

        horizontalLayout->addWidget(minLabel);

        minSlider = new QSlider(histogramWindow);
        minSlider->setObjectName(QString::fromUtf8("minSlider"));
        minSlider->setMaximum(255);
        minSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(minSlider);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        histogramView = new QGraphicsView(histogramWindow);
        histogramView->setObjectName(QString::fromUtf8("histogramView"));

        gridLayout->addWidget(histogramView, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(histogramWindow);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        maxLabel = new QLabel(histogramWindow);
        maxLabel->setObjectName(QString::fromUtf8("maxLabel"));

        horizontalLayout_2->addWidget(maxLabel);

        maxSlider = new QSlider(histogramWindow);
        maxSlider->setObjectName(QString::fromUtf8("maxSlider"));
        maxSlider->setMaximum(255);
        maxSlider->setValue(255);
        maxSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(maxSlider);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        applyButton = new QPushButton(histogramWindow);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        gridLayout->addWidget(applyButton, 3, 0, 1, 1);


        retranslateUi(histogramWindow);
        QObject::connect(maxSlider, SIGNAL(valueChanged(int)), histogramWindow, SLOT(ActionStretch()));
        QObject::connect(minSlider, SIGNAL(valueChanged(int)), histogramWindow, SLOT(ActionStretch()));
        QObject::connect(applyButton, SIGNAL(clicked()), histogramWindow, SLOT(ActionApply()));

        QMetaObject::connectSlotsByName(histogramWindow);
    } // setupUi

    void retranslateUi(QWidget *histogramWindow)
    {
        histogramWindow->setWindowTitle(QCoreApplication::translate("histogramWindow", "Histogram", nullptr));
        label->setText(QCoreApplication::translate("histogramWindow", "Stretch min:", nullptr));
        minLabel->setText(QCoreApplication::translate("histogramWindow", "0", nullptr));
        label_2->setText(QCoreApplication::translate("histogramWindow", "Stretch max:", nullptr));
        maxLabel->setText(QCoreApplication::translate("histogramWindow", "255", nullptr));
        applyButton->setText(QCoreApplication::translate("histogramWindow", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class histogramWindow: public Ui_histogramWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTOGRAMWINDOW_H
