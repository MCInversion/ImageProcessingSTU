/********************************************************************************
** Form generated from reading UI file 'MultiBlurDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTIBLURDIALOG_H
#define UI_MULTIBLURDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_multiBlurDIalog
{
public:
    QGridLayout *gridLayout;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QSpinBox *spinBox;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *multiBlurDIalog)
    {
        if (multiBlurDIalog->objectName().isEmpty())
            multiBlurDIalog->setObjectName(QString::fromUtf8("multiBlurDIalog"));
        multiBlurDIalog->resize(400, 109);
        gridLayout = new QGridLayout(multiBlurDIalog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cancelButton = new QPushButton(multiBlurDIalog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 2, 1, 1, 1);

        okButton = new QPushButton(multiBlurDIalog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 2, 0, 1, 1);

        spinBox = new QSpinBox(multiBlurDIalog);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(1);
        spinBox->setValue(5);

        gridLayout->addWidget(spinBox, 1, 1, 1, 1);

        label = new QLabel(multiBlurDIalog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(multiBlurDIalog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 2);


        retranslateUi(multiBlurDIalog);
        QObject::connect(okButton, SIGNAL(clicked()), multiBlurDIalog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), multiBlurDIalog, SLOT(reject()));

        QMetaObject::connectSlotsByName(multiBlurDIalog);
    } // setupUi

    void retranslateUi(QDialog *multiBlurDIalog)
    {
        multiBlurDIalog->setWindowTitle(QCoreApplication::translate("multiBlurDIalog", "Multi-Blur", nullptr));
        cancelButton->setText(QCoreApplication::translate("multiBlurDIalog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("multiBlurDIalog", "OK", nullptr));
        label->setText(QCoreApplication::translate("multiBlurDIalog", "steps:", nullptr));
        label_2->setText(QCoreApplication::translate("multiBlurDIalog", "> generates a sequence of progressively blurred images", nullptr));
    } // retranslateUi

};

namespace Ui {
    class multiBlurDIalog: public Ui_multiBlurDIalog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTIBLURDIALOG_H
