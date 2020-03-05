/********************************************************************************
** Form generated from reading UI file 'MirrorExtendDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIRROREXTENDDIALOG_H
#define UI_MIRROREXTENDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_DialogMirrorExtend
{
public:
    QGridLayout *gridLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpinBox *spinBoxExtendPixels;
    QLabel *label;

    void setupUi(QDialog *DialogMirrorExtend)
    {
        if (DialogMirrorExtend->objectName().isEmpty())
            DialogMirrorExtend->setObjectName(QString::fromUtf8("DialogMirrorExtend"));
        DialogMirrorExtend->resize(400, 103);
        gridLayout = new QGridLayout(DialogMirrorExtend);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        okButton = new QPushButton(DialogMirrorExtend);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 1, 0, 1, 1);

        cancelButton = new QPushButton(DialogMirrorExtend);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 1, 1, 1, 1);

        spinBoxExtendPixels = new QSpinBox(DialogMirrorExtend);
        spinBoxExtendPixels->setObjectName(QString::fromUtf8("spinBoxExtendPixels"));
        spinBoxExtendPixels->setMaximum(500);
        spinBoxExtendPixels->setValue(30);

        gridLayout->addWidget(spinBoxExtendPixels, 0, 0, 1, 1);

        label = new QLabel(DialogMirrorExtend);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);


        retranslateUi(DialogMirrorExtend);
        QObject::connect(okButton, SIGNAL(clicked()), DialogMirrorExtend, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), DialogMirrorExtend, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogMirrorExtend);
    } // setupUi

    void retranslateUi(QDialog *DialogMirrorExtend)
    {
        DialogMirrorExtend->setWindowTitle(QCoreApplication::translate("DialogMirrorExtend", "Mirror Extend", nullptr));
        okButton->setText(QCoreApplication::translate("DialogMirrorExtend", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("DialogMirrorExtend", "Cancel", nullptr));
        label->setText(QCoreApplication::translate("DialogMirrorExtend", "px", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogMirrorExtend: public Ui_DialogMirrorExtend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIRROREXTENDDIALOG_H
