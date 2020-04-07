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
	int getSteps() { return multiBlurDialogUi->stepsSpinBox->value(); }
	int getRadius() { return multiBlurDialogUi->radiusSpinBox->value(); }
private:
	Ui::multiBlurDIalog* multiBlurDialogUi;
};

