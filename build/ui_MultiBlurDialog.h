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
    QSpinBox *stepsSpinBox;
    QLabel *label_2;
    QPushButton *okButton;
    QLabel *label;
    QLabel *label_3;
    QSpinBox *radiusSpinBox;

    void setupUi(QDialog *multiBlurDIalog)
    {
        if (multiBlurDIalog->objectName().isEmpty())
            multiBlurDIalog->setObjectName(QString::fromUtf8("multiBlurDIalog"));
        multiBlurDIalog->resize(345, 135);
        gridLayout = new QGridLayout(multiBlurDIalog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cancelButton = new QPushButton(multiBlurDIalog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 3, 1, 1, 1);

        stepsSpinBox = new QSpinBox(multiBlurDIalog);
        stepsSpinBox->setObjectName(QString::fromUtf8("stepsSpinBox"));
        stepsSpinBox->setMinimum(1);
        stepsSpinBox->setValue(5);

        gridLayout->addWidget(stepsSpinBox, 1, 1, 1, 1);

        label_2 = new QLabel(multiBlurDIalog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 2);

        okButton = new QPushButton(multiBlurDIalog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 3, 0, 1, 1);

        label = new QLabel(multiBlurDIalog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_3 = new QLabel(multiBlurDIalog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        radiusSpinBox = new QSpinBox(multiBlurDIalog);
        radiusSpinBox->setObjectName(QString::fromUtf8("radiusSpinBox"));
        radiusSpinBox->setMinimum(1);
        radiusSpinBox->setValue(3);

        gridLayout->addWidget(radiusSpinBox, 2, 1, 1, 1);


        retranslateUi(multiBlurDIalog);
        QObject::connect(okButton, SIGNAL(clicked()), multiBlurDIalog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), multiBlurDIalog, SLOT(reject()));

        QMetaObject::connectSlotsByName(multiBlurDIalog);
    } // setupUi

    void retranslateUi(QDialog *multiBlurDIalog)
    {
        multiBlurDIalog->setWindowTitle(QCoreApplication::translate("multiBlurDIalog", "Multi-Blur", nullptr));
        cancelButton->setText(QCoreApplication::translate("multiBlurDIalog", "Cancel", nullptr));
        label_2->setText(QCoreApplication::translate("multiBlurDIalog", "> generates a sequence of progressively blurred images", nullptr));
        okButton->setText(QCoreApplication::translate("multiBlurDIalog", "OK", nullptr));
        label->setText(QCoreApplication::translate("multiBlurDIalog", "steps:", nullptr));
        label_3->setText(QCoreApplication::translate("multiBlurDIalog", "radius:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class multiBlurDIalog: public Ui_multiBlurDIalog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTIBLURDIALOG_H
