/********************************************************************************
** Form generated from reading UI file 'PeronaMalikDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PERONAMALIKDIALOG_H
#define UI_PERONAMALIKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_peronaMalikDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QPushButton *okButton;
    QLabel *label_4;
    QDoubleSpinBox *timeStepSpinBox;
    QSpinBox *nStepsSpinBox;
    QLabel *label;
    QPushButton *cancelButton;
    QLabel *label_3;
    QDoubleSpinBox *kParamSpinBox;
    QComboBox *schemeComboBox;
    QCheckBox *overrideCheckBox;

    void setupUi(QDialog *peronaMalikDialog)
    {
        if (peronaMalikDialog->objectName().isEmpty())
            peronaMalikDialog->setObjectName(QString::fromUtf8("peronaMalikDialog"));
        peronaMalikDialog->resize(400, 211);
        gridLayout = new QGridLayout(peronaMalikDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(peronaMalikDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        okButton = new QPushButton(peronaMalikDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 5, 0, 1, 1);

        label_4 = new QLabel(peronaMalikDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        timeStepSpinBox = new QDoubleSpinBox(peronaMalikDialog);
        timeStepSpinBox->setObjectName(QString::fromUtf8("timeStepSpinBox"));
        timeStepSpinBox->setMinimum(0.010000000000000);
        timeStepSpinBox->setSingleStep(0.010000000000000);
        timeStepSpinBox->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
        timeStepSpinBox->setValue(0.200000000000000);

        gridLayout->addWidget(timeStepSpinBox, 2, 1, 1, 1);

        nStepsSpinBox = new QSpinBox(peronaMalikDialog);
        nStepsSpinBox->setObjectName(QString::fromUtf8("nStepsSpinBox"));
        nStepsSpinBox->setMinimum(1);
        nStepsSpinBox->setMaximum(200);
        nStepsSpinBox->setValue(10);

        gridLayout->addWidget(nStepsSpinBox, 1, 1, 1, 1);

        label = new QLabel(peronaMalikDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        cancelButton = new QPushButton(peronaMalikDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 5, 1, 1, 1);

        label_3 = new QLabel(peronaMalikDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        kParamSpinBox = new QDoubleSpinBox(peronaMalikDialog);
        kParamSpinBox->setObjectName(QString::fromUtf8("kParamSpinBox"));
        kParamSpinBox->setMinimum(0.000000000000000);
        kParamSpinBox->setMaximum(1.000000000000000);
        kParamSpinBox->setSingleStep(0.001000000000000);
        kParamSpinBox->setValue(0.010000000000000);

        gridLayout->addWidget(kParamSpinBox, 3, 1, 1, 1);

        schemeComboBox = new QComboBox(peronaMalikDialog);
        schemeComboBox->addItem(QString());
        schemeComboBox->addItem(QString());
        schemeComboBox->setObjectName(QString::fromUtf8("schemeComboBox"));

        gridLayout->addWidget(schemeComboBox, 0, 1, 1, 1);

        overrideCheckBox = new QCheckBox(peronaMalikDialog);
        overrideCheckBox->setObjectName(QString::fromUtf8("overrideCheckBox"));

        gridLayout->addWidget(overrideCheckBox, 4, 0, 1, 1);


        retranslateUi(peronaMalikDialog);
        QObject::connect(okButton, SIGNAL(clicked()), peronaMalikDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), peronaMalikDialog, SLOT(reject()));
        QObject::connect(schemeComboBox, SIGNAL(currentIndexChanged(int)), peronaMalikDialog, SLOT(bindTimeStep()));
        QObject::connect(overrideCheckBox, SIGNAL(toggled(bool)), peronaMalikDialog, SLOT(overrideTimeStep()));

        QMetaObject::connectSlotsByName(peronaMalikDialog);
    } // setupUi

    void retranslateUi(QDialog *peronaMalikDialog)
    {
        peronaMalikDialog->setWindowTitle(QCoreApplication::translate("peronaMalikDialog", "Perona-Malik", nullptr));
        label_2->setText(QCoreApplication::translate("peronaMalikDialog", "NSteps:", nullptr));
        okButton->setText(QCoreApplication::translate("peronaMalikDialog", "OK", nullptr));
        label_4->setText(QCoreApplication::translate("peronaMalikDialog", "diffusion parameter (K):", nullptr));
        label->setText(QCoreApplication::translate("peronaMalikDialog", "solver scheme:", nullptr));
        cancelButton->setText(QCoreApplication::translate("peronaMalikDialog", "Cancel", nullptr));
        label_3->setText(QCoreApplication::translate("peronaMalikDialog", "time step:", nullptr));
        schemeComboBox->setItemText(0, QCoreApplication::translate("peronaMalikDialog", "explicit", nullptr));
        schemeComboBox->setItemText(1, QCoreApplication::translate("peronaMalikDialog", "regularized (semi-implicit)", nullptr));

        overrideCheckBox->setText(QCoreApplication::translate("peronaMalikDialog", "override time step", nullptr));
    } // retranslateUi

};

namespace Ui {
    class peronaMalikDialog: public Ui_peronaMalikDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERONAMALIKDIALOG_H
