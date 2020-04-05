#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets>

#include "../build/ui_MultiBlurDialog.h"

class MultiBlurDialog :	public QDialog
{
	Q_OBJECT

public:
	MultiBlurDialog(QWidget* parent = Q_NULLPTR) : QDialog(parent), multiBlurDialogUi(new Ui::multiBlurDIalog)
	{
		multiBlurDialogUi->setupUi(this);
	}
	int getSteps() { return multiBlurDialogUi->spinBox->value(); }
private:
	Ui::multiBlurDIalog* multiBlurDialogUi;
};

