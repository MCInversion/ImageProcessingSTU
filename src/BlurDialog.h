#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets>

#include "../build/ui_BlurDialog.h"

class BlurDialog : public QDialog
{
	Q_OBJECT

public:
	BlurDialog(QWidget* parent = Q_NULLPTR) : QDialog(parent), blurDialogUi(new Ui::blurDialog)
	{
		blurDialogUi->setupUi(this);
	}
	int getRadius() { return blurDialogUi->spinBox->value(); }
	int getKernelType() { return blurDialogUi->comboBox->currentIndex(); }
	bool printKernelToConsole() { return blurDialogUi->checkBox->isChecked(); }
private:
	Ui::blurDialog* blurDialogUi;
};

