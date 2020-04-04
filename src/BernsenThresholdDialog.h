#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets>

#include "../build/ui_BernsenThresholdDialog.h"

class BernsenThresholdDialog: public QDialog
{
	Q_OBJECT
public:
	BernsenThresholdDialog(QWidget* parent = Q_NULLPTR) : QDialog(parent), bernsenThresholdDialogUi(new Ui::bernsenThresholdDialog)
	{
		bernsenThresholdDialogUi->setupUi(this);
	}
	int getBackgroundType() { return bernsenThresholdDialogUi->bgTypeComboBox->currentIndex(); }
	int getRadius() { return bernsenThresholdDialogUi->radiusSpinBox->value(); }
	int getMinContrast() { return bernsenThresholdDialogUi->minContrastSpinBox->value(); }
	bool printMaskToConsole() { return bernsenThresholdDialogUi->printToConsoleCheckBox->isChecked(); }
	bool convertToGrayscale() { return bernsenThresholdDialogUi->convertToGSCheckBox->isChecked(); }
private:
	Ui::bernsenThresholdDialog* bernsenThresholdDialogUi;
};

