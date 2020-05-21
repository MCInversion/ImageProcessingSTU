/********************************************************************************
** Form generated from reading UI file 'CurvatureFilterDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CURVATUREFILTERDIALOG_H
#define UI_CURVATUREFILTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_curvatureFilterDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *labelEpsilon;
    QLabel *labelNSteps;
    QSpinBox *nStepsSpinBox;
    QLabel *labelTimeStep;
    QSpinBox *epsilonSpinBox;
    QLabel *labelFlowType;
    QPushButton *okButton;
    QDoubleSpinBox *diffParamSpinBox;
    QPushButton *cancelButton;
    QComboBox *flowTypeComboBox;
    QLabel *labelDiffParam;
    QDoubleSpinBox *timeStepSpinBox;

    void setupUi(QDialog *curvatureFilterDialog)
    {
        if (curvatureFilterDialog->objectName().isEmpty())
            curvatureFilterDialog->setObjectName(QString::fromUtf8("curvatureFilterDialog"));
        curvatureFilterDialog->resize(490, 211);
        gridLayout = new QGridLayout(curvatureFilterDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelEpsilon = new QLabel(curvatureFilterDialog);
        labelEpsilon->setObjectName(QString::fromUtf8("labelEpsilon"));

        gridLayout->addWidget(labelEpsilon, 4, 0, 1, 1);

        labelNSteps = new QLabel(curvatureFilterDialog);
        labelNSteps->setObjectName(QString::fromUtf8("labelNSteps"));

        gridLayout->addWidget(labelNSteps, 1, 0, 1, 1);

        nStepsSpinBox = new QSpinBox(curvatureFilterDialog);
        nStepsSpinBox->setObjectName(QString::fromUtf8("nStepsSpinBox"));
        nStepsSpinBox->setMinimum(1);
        nStepsSpinBox->setMaximum(200);
        nStepsSpinBox->setSingleStep(1);
        nStepsSpinBox->setValue(10);

        gridLayout->addWidget(nStepsSpinBox, 1, 1, 1, 1);

        labelTimeStep = new QLabel(curvatureFilterDialog);
        labelTimeStep->setObjectName(QString::fromUtf8("labelTimeStep"));

        gridLayout->addWidget(labelTimeStep, 2, 0, 1, 1);

        epsilonSpinBox = new QSpinBox(curvatureFilterDialog);
        epsilonSpinBox->setObjectName(QString::fromUtf8("epsilonSpinBox"));
        epsilonSpinBox->setMaximum(12);
        epsilonSpinBox->setValue(3);

        gridLayout->addWidget(epsilonSpinBox, 4, 1, 1, 1);

        labelFlowType = new QLabel(curvatureFilterDialog);
        labelFlowType->setObjectName(QString::fromUtf8("labelFlowType"));

        gridLayout->addWidget(labelFlowType, 0, 0, 1, 1);

        okButton = new QPushButton(curvatureFilterDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 5, 0, 1, 1);

        diffParamSpinBox = new QDoubleSpinBox(curvatureFilterDialog);
        diffParamSpinBox->setObjectName(QString::fromUtf8("diffParamSpinBox"));
        diffParamSpinBox->setMinimum(0.000000000000000);
        diffParamSpinBox->setMaximum(100.000000000000000);
        diffParamSpinBox->setSingleStep(0.010000000000000);
        diffParamSpinBox->setValue(0.010000000000000);

        gridLayout->addWidget(diffParamSpinBox, 3, 1, 1, 1);

        cancelButton = new QPushButton(curvatureFilterDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 5, 1, 1, 1);

        flowTypeComboBox = new QComboBox(curvatureFilterDialog);
        flowTypeComboBox->addItem(QString());
        flowTypeComboBox->addItem(QString());
        flowTypeComboBox->setObjectName(QString::fromUtf8("flowTypeComboBox"));
        flowTypeComboBox->setMinimumSize(QSize(250, 0));

        gridLayout->addWidget(flowTypeComboBox, 0, 1, 1, 1);

        labelDiffParam = new QLabel(curvatureFilterDialog);
        labelDiffParam->setObjectName(QString::fromUtf8("labelDiffParam"));

        gridLayout->addWidget(labelDiffParam, 3, 0, 1, 1);

        timeStepSpinBox = new QDoubleSpinBox(curvatureFilterDialog);
        timeStepSpinBox->setObjectName(QString::fromUtf8("timeStepSpinBox"));
        timeStepSpinBox->setMinimum(0.010000000000000);
        timeStepSpinBox->setMaximum(100.000000000000000);
        timeStepSpinBox->setSingleStep(0.010000000000000);
        timeStepSpinBox->setValue(0.200000000000000);

        gridLayout->addWidget(timeStepSpinBox, 2, 1, 1, 1);


        retranslateUi(curvatureFilterDialog);
        QObject::connect(okButton, SIGNAL(clicked()), curvatureFilterDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), curvatureFilterDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(curvatureFilterDialog);
    } // setupUi

    void retranslateUi(QDialog *curvatureFilterDialog)
    {
        curvatureFilterDialog->setWindowTitle(QCoreApplication::translate("curvatureFilterDialog", "Curvature Filter", nullptr));
        labelEpsilon->setText(QCoreApplication::translate("curvatureFilterDialog", "regularization param (epsilon): 1E-", nullptr));
        labelNSteps->setText(QCoreApplication::translate("curvatureFilterDialog", "NSteps:", nullptr));
        labelTimeStep->setText(QCoreApplication::translate("curvatureFilterDialog", "time step:", nullptr));
        labelFlowType->setText(QCoreApplication::translate("curvatureFilterDialog", "flow type:", nullptr));
        okButton->setText(QCoreApplication::translate("curvatureFilterDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("curvatureFilterDialog", "Cancel", nullptr));
        flowTypeComboBox->setItemText(0, QCoreApplication::translate("curvatureFilterDialog", "Mean Curvature Flow (MCF)", nullptr));
        flowTypeComboBox->setItemText(1, QCoreApplication::translate("curvatureFilterDialog", "Geodesic Mean Curvature Flow (GMCF)", nullptr));

        labelDiffParam->setText(QCoreApplication::translate("curvatureFilterDialog", "diffusion parameter (K):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class curvatureFilterDialog: public Ui_curvatureFilterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CURVATUREFILTERDIALOG_H
