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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HistogramWindow
{
public:
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *horizontalSlider;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSlider *horizontalSlider_2;

    void setupUi(QWidget *HistogramWindow)
    {
        if (HistogramWindow->objectName().isEmpty())
            HistogramWindow->setObjectName(QString::fromUtf8("HistogramWindow"));
        HistogramWindow->resize(476, 354);
        verticalLayout = new QVBoxLayout(HistogramWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new QGraphicsView(HistogramWindow);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout->addWidget(graphicsView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(HistogramWindow);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSlider = new QSlider(HistogramWindow);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMaximum(255);
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(HistogramWindow);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSlider_2 = new QSlider(HistogramWindow);
        horizontalSlider_2->setObjectName(QString::fromUtf8("horizontalSlider_2"));
        horizontalSlider_2->setMaximum(255);
        horizontalSlider_2->setValue(255);
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(horizontalSlider_2);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(HistogramWindow);

        QMetaObject::connectSlotsByName(HistogramWindow);
    } // setupUi

    void retranslateUi(QWidget *HistogramWindow)
    {
        HistogramWindow->setWindowTitle(QCoreApplication::translate("HistogramWindow", "Histogram", nullptr));
        label->setText(QCoreApplication::translate("HistogramWindow", "Stretch min:", nullptr));
        label_2->setText(QCoreApplication::translate("HistogramWindow", "Stretch max:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HistogramWindow: public Ui_HistogramWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTOGRAMWINDOW_H
