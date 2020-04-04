/********************************************************************************
** Form generated from reading UI file 'BernsenThresholdDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BERNSENTHRESHOLDDIALOG_H
#define UI_BERNSENTHRESHOLDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_bernsenThresholdDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *minContrastLabel;
    QHBoxLayout *radiusLayout;
    QSpinBox *radiusSpinBox;
    QLabel *pxLabel;
    QHBoxLayout *bgTypeLayout;
    QComboBox *bgTypeComboBox;
    QLabel *bgTypeLabel;
    QLabel *radiusLabel;
    QDialogButtonBox *buttonBox;
    QHBoxLayout *minContrastLayout;
    QSpinBox *minContrastSpinBox;
    QCheckBox *printToConsoleCheckBox;
    QLabel *label;
    QCheckBox *convertToGSCheckBox;

    void setupUi(QDialog *bernsenThresholdDialog)
    {
        if (bernsenThresholdDialog->objectName().isEmpty())
            bernsenThresholdDialog->setObjectName(QString::fromUtf8("bernsenThresholdDialog"));
        bernsenThresholdDialog->resize(328, 201);
        gridLayout = new QGridLayout(bernsenThresholdDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        minContrastLabel = new QLabel(bernsenThresholdDialog);
        minContrastLabel->setObjectName(QString::fromUtf8("minContrastLabel"));

        gridLayout->addWidget(minContrastLabel, 2, 1, 1, 1);

        radiusLayout = new QHBoxLayout();
        radiusLayout->setObjectName(QString::fromUtf8("radiusLayout"));
        radiusSpinBox = new QSpinBox(bernsenThresholdDialog);
        radiusSpinBox->setObjectName(QString::fromUtf8("radiusSpinBox"));
        radiusSpinBox->setMinimumSize(QSize(0, 0));
        radiusSpinBox->setMinimum(1);
        radiusSpinBox->setValue(5);

        radiusLayout->addWidget(radiusSpinBox);

        pxLabel = new QLabel(bernsenThresholdDialog);
        pxLabel->setObjectName(QString::fromUtf8("pxLabel"));

        radiusLayout->addWidget(pxLabel);


        gridLayout->addLayout(radiusLayout, 1, 3, 1, 1);

        bgTypeLayout = new QHBoxLayout();
        bgTypeLayout->setObjectName(QString::fromUtf8("bgTypeLayout"));
        bgTypeComboBox = new QComboBox(bernsenThresholdDialog);
        bgTypeComboBox->addItem(QString());
        bgTypeComboBox->addItem(QString());
        bgTypeComboBox->setObjectName(QString::fromUtf8("bgTypeComboBox"));

        bgTypeLayout->addWidget(bgTypeComboBox);


        gridLayout->addLayout(bgTypeLayout, 0, 3, 1, 1);

        bgTypeLabel = new QLabel(bernsenThresholdDialog);
        bgTypeLabel->setObjectName(QString::fromUtf8("bgTypeLabel"));

        gridLayout->addWidget(bgTypeLabel, 0, 1, 1, 1);

        radiusLabel = new QLabel(bernsenThresholdDialog);
        radiusLabel->setObjectName(QString::fromUtf8("radiusLabel"));

        gridLayout->addWidget(radiusLabel, 1, 1, 1, 1);

        buttonBox = new QDialogButtonBox(bernsenThresholdDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 5, 1, 1, 1);

        minContrastLayout = new QHBoxLayout();
        minContrastLayout->setObjectName(QString::fromUtf8("minContrastLayout"));
        minContrastSpinBox = new QSpinBox(bernsenThresholdDialog);
        minContrastSpinBox->setObjectName(QString::fromUtf8("minContrastSpinBox"));
        minContrastSpinBox->setMaximum(254);
        minContrastSpinBox->setValue(15);

        minContrastLayout->addWidget(minContrastSpinBox);


        gridLayout->addLayout(minContrastLayout, 2, 3, 1, 1);

        printToConsoleCheckBox = new QCheckBox(bernsenThresholdDialog);
        printToConsoleCheckBox->setObjectName(QString::fromUtf8("printToConsoleCheckBox"));
        printToConsoleCheckBox->setChecked(true);

        gridLayout->addWidget(printToConsoleCheckBox, 3, 1, 1, 1);

        label = new QLabel(bernsenThresholdDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 3, 3, 1, 1);

        convertToGSCheckBox = new QCheckBox(bernsenThresholdDialog);
        convertToGSCheckBox->setObjectName(QString::fromUtf8("convertToGSCheckBox"));

        gridLayout->addWidget(convertToGSCheckBox, 4, 1, 1, 1);


        retranslateUi(bernsenThresholdDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), bernsenThresholdDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), bernsenThresholdDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(bernsenThresholdDialog);
    } // setupUi

    void retranslateUi(QDialog *bernsenThresholdDialog)
    {
        bernsenThresholdDialog->setWindowTitle(QCoreApplication::translate("bernsenThresholdDialog", "Bernsen Threshold", nullptr));
        minContrastLabel->setText(QCoreApplication::translate("bernsenThresholdDialog", "min contrast (I_max - I_min):", nullptr));
        pxLabel->setText(QCoreApplication::translate("bernsenThresholdDialog", "px", nullptr));
        bgTypeComboBox->setItemText(0, QCoreApplication::translate("bernsenThresholdDialog", "dark", nullptr));
        bgTypeComboBox->setItemText(1, QCoreApplication::translate("bernsenThresholdDialog", "bright", nullptr));

        bgTypeLabel->setText(QCoreApplication::translate("bernsenThresholdDialog", "background type:", nullptr));
        radiusLabel->setText(QCoreApplication::translate("bernsenThresholdDialog", "radius:", nullptr));
        printToConsoleCheckBox->setText(QCoreApplication::translate("bernsenThresholdDialog", "print mask to console", nullptr));
        label->setText(QString());
        convertToGSCheckBox->setText(QCoreApplication::translate("bernsenThresholdDialog", "Convert RGB to grayscale", nullptr));
    } // retranslateUi

};

namespace Ui {
    class bernsenThresholdDialog: public Ui_bernsenThresholdDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BERNSENTHRESHOLDDIALOG_H
