/********************************************************************************
** Form generated from reading UI file 'NewImgDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWIMGDIALOG_H
#define UI_NEWIMGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_DialogNewImg
{
public:
    QGridLayout *gridLayout;
    QLabel *label_3;
    QSpinBox *spinBoxNewImgWidth;
    QLabel *label_2;
    QLabel *label_5;
    QSpinBox *spinBoxNewImgHeight;
    QPushButton *pushButtonNewImgOk;
    QLabel *label;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QPushButton *pushButtonNewImgCancel;

    void setupUi(QDialog *DialogNewImg)
    {
        if (DialogNewImg->objectName().isEmpty())
            DialogNewImg->setObjectName(QString::fromUtf8("DialogNewImg"));
        DialogNewImg->resize(250, 150);
        DialogNewImg->setMinimumSize(QSize(250, 150));
        DialogNewImg->setMaximumSize(QSize(250, 150));
        gridLayout = new QGridLayout(DialogNewImg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(DialogNewImg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        spinBoxNewImgWidth = new QSpinBox(DialogNewImg);
        spinBoxNewImgWidth->setObjectName(QString::fromUtf8("spinBoxNewImgWidth"));
        spinBoxNewImgWidth->setMinimum(1);
        spinBoxNewImgWidth->setMaximum(5000);
        spinBoxNewImgWidth->setValue(500);

        gridLayout->addWidget(spinBoxNewImgWidth, 1, 1, 1, 2);

        label_2 = new QLabel(DialogNewImg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_5 = new QLabel(DialogNewImg);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 2, 3, 1, 1);

        spinBoxNewImgHeight = new QSpinBox(DialogNewImg);
        spinBoxNewImgHeight->setObjectName(QString::fromUtf8("spinBoxNewImgHeight"));
        spinBoxNewImgHeight->setMinimum(1);
        spinBoxNewImgHeight->setMaximum(5000);
        spinBoxNewImgHeight->setValue(500);

        gridLayout->addWidget(spinBoxNewImgHeight, 2, 1, 1, 2);

        pushButtonNewImgOk = new QPushButton(DialogNewImg);
        pushButtonNewImgOk->setObjectName(QString::fromUtf8("pushButtonNewImgOk"));

        gridLayout->addWidget(pushButtonNewImgOk, 5, 1, 1, 3);

        label = new QLabel(DialogNewImg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(50, 20));
        label->setMaximumSize(QSize(50, 20));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(DialogNewImg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setScaledContents(false);

        gridLayout->addWidget(label_4, 1, 3, 1, 1);

        lineEdit = new QLineEdit(DialogNewImg);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 3);

        pushButtonNewImgCancel = new QPushButton(DialogNewImg);
        pushButtonNewImgCancel->setObjectName(QString::fromUtf8("pushButtonNewImgCancel"));

        gridLayout->addWidget(pushButtonNewImgCancel, 6, 1, 1, 3);


        retranslateUi(DialogNewImg);
        QObject::connect(pushButtonNewImgOk, SIGNAL(clicked()), DialogNewImg, SLOT(accept()));
        QObject::connect(pushButtonNewImgCancel, SIGNAL(clicked()), DialogNewImg, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogNewImg);
    } // setupUi

    void retranslateUi(QDialog *DialogNewImg)
    {
        DialogNewImg->setWindowTitle(QCoreApplication::translate("DialogNewImg", "New image", nullptr));
        label_3->setText(QCoreApplication::translate("DialogNewImg", "Heigth:", nullptr));
        label_2->setText(QCoreApplication::translate("DialogNewImg", "Width:", nullptr));
        label_5->setText(QCoreApplication::translate("DialogNewImg", "px", nullptr));
        pushButtonNewImgOk->setText(QCoreApplication::translate("DialogNewImg", "Ok", nullptr));
        label->setText(QCoreApplication::translate("DialogNewImg", "Name:", nullptr));
        label_4->setText(QCoreApplication::translate("DialogNewImg", "px", nullptr));
        lineEdit->setText(QCoreApplication::translate("DialogNewImg", "New image", nullptr));
        pushButtonNewImgCancel->setText(QCoreApplication::translate("DialogNewImg", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogNewImg: public Ui_DialogNewImg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWIMGDIALOG_H
