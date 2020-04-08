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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_heatEquationDialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QLabel *label;
    QComboBox *schemeComboBox;

    void setupUi(QDialog *heatEquationDialog)
    {
        if (heatEquationDialog->objectName().isEmpty())
            heatEquationDialog->setObjectName(QString::fromUtf8("heatEquationDialog"));
        heatEquationDialog->resize(400, 106);
        gridLayout = new QGridLayout(heatEquationDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cancelButton = new QPushButton(heatEquationDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 1, 1, 1, 1);

        okButton = new QPushButton(heatEquationDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 1, 0, 1, 1);

        label = new QLabel(heatEquationDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        schemeComboBox = new QComboBox(heatEquationDialog);
        schemeComboBox->addItem(QString());
        schemeComboBox->addItem(QString());
        schemeComboBox->setObjectName(QString::fromUtf8("schemeComboBox"));

        gridLayout->addWidget(schemeComboBox, 0, 1, 1, 1);


        retranslateUi(heatEquationDialog);
        QObject::connect(okButton, SIGNAL(clicked()), heatEquationDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), heatEquationDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(heatEquationDialog);
    } // setupUi

    void retranslateUi(QDialog *heatEquationDialog)
    {
        heatEquationDialog->setWindowTitle(QCoreApplication::translate("heatEquationDialog", "Heat Equation", nullptr));
        cancelButton->setText(QCoreApplication::translate("heatEquationDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("heatEquationDialog", "OK", nullptr));
        label->setText(QCoreApplication::translate("heatEquationDialog", "solver scheme:", nullptr));
        schemeComboBox->setItemText(0, QCoreApplication::translate("heatEquationDialog", "explicit", nullptr));
        schemeComboBox->setItemText(1, QCoreApplication::translate("heatEquationDialog", "implicit", nullptr));

    } // retranslateUi

};

namespace Ui {
    class heatEquationDialog: public Ui_heatEquationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEATEQUATIONDIALOG_H
