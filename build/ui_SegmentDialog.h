/********************************************************************************
** Form generated from reading UI file 'SegmentDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGMENTDIALOG_H
#define UI_SEGMENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_segmentDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_4;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLabel *label_6;
    QLabel *label_3;
    QDoubleSpinBox *diffParamSpinBox;
    QCheckBox *addIsoDataCheckBox;
    QHBoxLayout *horizontalLayout;
    QSpinBox *uMinSpinBox;
    QSpinBox *uMaxSpinBox;
    QSpinBox *epsilonSpinBox;
    QSpinBox *nStepsSpinBox;
    QLabel *label_5;
    QDoubleSpinBox *timeStepSpinBox;
    QLabel *label;

    void setupUi(QDialog *segmentDialog)
    {
        if (segmentDialog->objectName().isEmpty())
            segmentDialog->setObjectName(QString::fromUtf8("segmentDialog"));
        segmentDialog->resize(400, 266);
        gridLayout = new QGridLayout(segmentDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(segmentDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_4 = new QLabel(segmentDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        okButton = new QPushButton(segmentDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 6, 0, 1, 1);

        cancelButton = new QPushButton(segmentDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 6, 1, 1, 1);

        label_6 = new QLabel(segmentDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 5, 1, 1, 1);

        label_3 = new QLabel(segmentDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        diffParamSpinBox = new QDoubleSpinBox(segmentDialog);
        diffParamSpinBox->setObjectName(QString::fromUtf8("diffParamSpinBox"));
        diffParamSpinBox->setMaximum(10000.000000000000000);
        diffParamSpinBox->setSingleStep(0.010000000000000);
        diffParamSpinBox->setValue(5000.000000000000000);

        gridLayout->addWidget(diffParamSpinBox, 3, 1, 1, 1);

        addIsoDataCheckBox = new QCheckBox(segmentDialog);
        addIsoDataCheckBox->setObjectName(QString::fromUtf8("addIsoDataCheckBox"));
        addIsoDataCheckBox->setChecked(true);

        gridLayout->addWidget(addIsoDataCheckBox, 5, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        uMinSpinBox = new QSpinBox(segmentDialog);
        uMinSpinBox->setObjectName(QString::fromUtf8("uMinSpinBox"));
        uMinSpinBox->setMinimum(-255);
        uMinSpinBox->setMaximum(0);
        uMinSpinBox->setValue(-2);

        horizontalLayout->addWidget(uMinSpinBox);

        uMaxSpinBox = new QSpinBox(segmentDialog);
        uMaxSpinBox->setObjectName(QString::fromUtf8("uMaxSpinBox"));
        uMaxSpinBox->setMinimum(1);
        uMaxSpinBox->setMaximum(255);
        uMaxSpinBox->setValue(2);

        horizontalLayout->addWidget(uMaxSpinBox);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        epsilonSpinBox = new QSpinBox(segmentDialog);
        epsilonSpinBox->setObjectName(QString::fromUtf8("epsilonSpinBox"));
        epsilonSpinBox->setMinimum(0);
        epsilonSpinBox->setMaximum(12);
        epsilonSpinBox->setValue(8);

        gridLayout->addWidget(epsilonSpinBox, 4, 1, 1, 1);

        nStepsSpinBox = new QSpinBox(segmentDialog);
        nStepsSpinBox->setObjectName(QString::fromUtf8("nStepsSpinBox"));
        nStepsSpinBox->setMinimum(1);
        nStepsSpinBox->setMaximum(200);
        nStepsSpinBox->setValue(60);

        gridLayout->addWidget(nStepsSpinBox, 1, 1, 1, 1);

        label_5 = new QLabel(segmentDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        timeStepSpinBox = new QDoubleSpinBox(segmentDialog);
        timeStepSpinBox->setObjectName(QString::fromUtf8("timeStepSpinBox"));
        timeStepSpinBox->setMinimum(0.010000000000000);
        timeStepSpinBox->setMaximum(100.000000000000000);
        timeStepSpinBox->setSingleStep(0.010000000000000);
        timeStepSpinBox->setValue(2.000000000000000);

        gridLayout->addWidget(timeStepSpinBox, 2, 1, 1, 1);

        label = new QLabel(segmentDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);


        retranslateUi(segmentDialog);
        QObject::connect(okButton, SIGNAL(clicked()), segmentDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), segmentDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(segmentDialog);
    } // setupUi

    void retranslateUi(QDialog *segmentDialog)
    {
        segmentDialog->setWindowTitle(QCoreApplication::translate("segmentDialog", "Segment (SUBSURF)", nullptr));
        label_2->setText(QCoreApplication::translate("segmentDialog", "time step:", nullptr));
        label_4->setText(QCoreApplication::translate("segmentDialog", "regularization parameter (epsilon): 1E-", nullptr));
        okButton->setText(QCoreApplication::translate("segmentDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("segmentDialog", "Cancel", nullptr));
        label_6->setText(QString());
        label_3->setText(QCoreApplication::translate("segmentDialog", "diffusion parameter (K):", nullptr));
        addIsoDataCheckBox->setText(QCoreApplication::translate("segmentDialog", "add isodata image to list", nullptr));
        label_5->setText(QCoreApplication::translate("segmentDialog", "piecewise segmentation function range:", nullptr));
        label->setText(QCoreApplication::translate("segmentDialog", "NSteps:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class segmentDialog: public Ui_segmentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGMENTDIALOG_H
