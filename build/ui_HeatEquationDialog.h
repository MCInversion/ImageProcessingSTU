/********************************************************************************
** Form generated from reading UI file 'HeatEquationDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEATEQUATIONDIALOG_H
#define UI_HEATEQUATIONDIALOG_H

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

class Ui_heatEquationDialog
{
public:
    QGridLayout *gridLayout;
    QDoubleSpinBox *timeStepSpinBox;
    QLabel *label_3;
    QPushButton *cancelButton;
    QLabel *label;
    QLabel *label_2;
    QComboBox *schemeComboBox;
    QCheckBox *overrideCheckBox;
    QPushButton *okButton;
    QLabel *label_4;
    QSpinBox *nStepsSpinBox;

    void setupUi(QDialog *heatEquationDialog)
    {
        if (heatEquationDialog->objectName().isEmpty())
            heatEquationDialog->setObjectName(QString::fromUtf8("heatEquationDialog"));
        heatEquationDialog->resize(400, 168);
        gridLayout = new QGridLayout(heatEquationDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        timeStepSpinBox = new QDoubleSpinBox(heatEquationDialog);
        timeStepSpinBox->setObjectName(QString::fromUtf8("timeStepSpinBox"));
        timeStepSpinBox->setMinimum(0.010000000000000);
        timeStepSpinBox->setMaximum(0.250000000000000);
        timeStepSpinBox->setSingleStep(0.010000000000000);
        timeStepSpinBox->setValue(0.100000000000000);

        gridLayout->addWidget(timeStepSpinBox, 2, 1, 1, 1);

        label_3 = new QLabel(heatEquationDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 1, 1, 1);

        cancelButton = new QPushButton(heatEquationDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 4, 1, 1, 1);

        label = new QLabel(heatEquationDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(heatEquationDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        schemeComboBox = new QComboBox(heatEquationDialog);
        schemeComboBox->addItem(QString());
        schemeComboBox->addItem(QString());
        schemeComboBox->setObjectName(QString::fromUtf8("schemeComboBox"));

        gridLayout->addWidget(schemeComboBox, 0, 1, 1, 1);

        overrideCheckBox = new QCheckBox(heatEquationDialog);
        overrideCheckBox->setObjectName(QString::fromUtf8("overrideCheckBox"));

        gridLayout->addWidget(overrideCheckBox, 3, 0, 1, 1);

        okButton = new QPushButton(heatEquationDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 4, 0, 1, 1);

        label_4 = new QLabel(heatEquationDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        nStepsSpinBox = new QSpinBox(heatEquationDialog);
        nStepsSpinBox->setObjectName(QString::fromUtf8("nStepsSpinBox"));
        nStepsSpinBox->setMinimum(1);
        nStepsSpinBox->setMaximum(200);
        nStepsSpinBox->setValue(5);

        gridLayout->addWidget(nStepsSpinBox, 1, 1, 1, 1);


        retranslateUi(heatEquationDialog);
        QObject::connect(okButton, SIGNAL(clicked()), heatEquationDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), heatEquationDialog, SLOT(reject()));
        QObject::connect(schemeComboBox, SIGNAL(currentIndexChanged(int)), heatEquationDialog, SLOT(bindTimeStep()));
        QObject::connect(overrideCheckBox, SIGNAL(toggled(bool)), heatEquationDialog, SLOT(overrideTimeStep()));

        QMetaObject::connectSlotsByName(heatEquationDialog);
    } // setupUi

    void retranslateUi(QDialog *heatEquationDialog)
    {
        heatEquationDialog->setWindowTitle(QCoreApplication::translate("heatEquationDialog", "Heat Equation", nullptr));
        label_3->setText(QString());
        cancelButton->setText(QCoreApplication::translate("heatEquationDialog", "Cancel", nullptr));
        label->setText(QCoreApplication::translate("heatEquationDialog", "solver scheme:", nullptr));
        label_2->setText(QCoreApplication::translate("heatEquationDialog", "time step:", nullptr));
        schemeComboBox->setItemText(0, QCoreApplication::translate("heatEquationDialog", "explicit", nullptr));
        schemeComboBox->setItemText(1, QCoreApplication::translate("heatEquationDialog", "implicit", nullptr));

        overrideCheckBox->setText(QCoreApplication::translate("heatEquationDialog", "override time step", nullptr));
        okButton->setText(QCoreApplication::translate("heatEquationDialog", "OK", nullptr));
        label_4->setText(QCoreApplication::translate("heatEquationDialog", "NSteps:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class heatEquationDialog: public Ui_heatEquationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEATEQUATIONDIALOG_H
