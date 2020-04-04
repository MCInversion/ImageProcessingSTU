/********************************************************************************
** Form generated from reading UI file 'BlurDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLURDIALOG_H
#define UI_BLURDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_blurDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QHBoxLayout *horizontalLayout;
    QSpinBox *spinBox;
    QLabel *label_2;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboBox;
    QCheckBox *checkBox;

    void setupUi(QDialog *blurDialog)
    {
        if (blurDialog->objectName().isEmpty())
            blurDialog->setObjectName(QString::fromUtf8("blurDialog"));
        blurDialog->resize(476, 120);
        gridLayout = new QGridLayout(blurDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(blurDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        cancelButton = new QPushButton(blurDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 2, 1, 1, 1);

        okButton = new QPushButton(blurDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setFocusPolicy(Qt::StrongFocus);
        okButton->setAutoDefault(false);

        gridLayout->addWidget(okButton, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        spinBox = new QSpinBox(blurDialog);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMaximum(100);
        spinBox->setValue(3);

        horizontalLayout->addWidget(spinBox);

        label_2 = new QLabel(blurDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);


        gridLayout->addLayout(horizontalLayout, 1, 1, 1, 1);

        label_3 = new QLabel(blurDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        comboBox = new QComboBox(blurDialog);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_2->addWidget(comboBox);

        checkBox = new QCheckBox(blurDialog);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setChecked(true);

        horizontalLayout_2->addWidget(checkBox);


        gridLayout->addLayout(horizontalLayout_2, 0, 1, 1, 1);


        retranslateUi(blurDialog);
        QObject::connect(okButton, SIGNAL(clicked()), blurDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), blurDialog, SLOT(reject()));

        okButton->setDefault(true);
        comboBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(blurDialog);
    } // setupUi

    void retranslateUi(QDialog *blurDialog)
    {
        blurDialog->setWindowTitle(QCoreApplication::translate("blurDialog", "Blur", nullptr));
        label->setText(QCoreApplication::translate("blurDialog", "radius:", nullptr));
        cancelButton->setText(QCoreApplication::translate("blurDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("blurDialog", "OK", nullptr));
        label_2->setText(QCoreApplication::translate("blurDialog", "px", nullptr));
        label_3->setText(QCoreApplication::translate("blurDialog", "kernel:", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("blurDialog", "Gaussian", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("blurDialog", "Averaging", nullptr));

        checkBox->setText(QCoreApplication::translate("blurDialog", "print to console", nullptr));
    } // retranslateUi

};

namespace Ui {
    class blurDialog: public Ui_blurDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLURDIALOG_H
