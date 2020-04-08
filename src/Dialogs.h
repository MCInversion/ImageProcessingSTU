#ifndef DIALOGS_H_
#define DIALOGS_H_

#include <QtWidgets/QDialog>
#include <QtWidgets>

#include "../build/ui_BernsenThresholdDialog.h"
#include "../build/ui_BlurDialog.h"
#include "../build/ui_HeatEquationDialog.h"
#include "../build/ui_MirrorExtendDialog.h"
#include "../build/ui_MultiBlurDialog.h"
#include "../build/ui_NewImgDialog.h"

class BernsenThresholdDialog : public QDialog
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


class HeatEquationDialog : public QDialog
{
	Q_OBJECT
public:
	HeatEquationDialog(QWidget* parent = Q_NULLPTR) : QDialog(parent)
	{
		heatEquationDialogUi->setupUi(this);
	}
private:
	Ui::heatEquationDialog* heatEquationDialogUi;
};


class MirrorExtendDialog : public QDialog
{
	Q_OBJECT

public:
	MirrorExtendDialog(QWidget* parent = Q_NULLPTR) : QDialog(parent), mirrorExtendUi(new Ui::DialogMirrorExtend)
	{
		mirrorExtendUi->setupUi(this);
	};
	int getPixels() { return mirrorExtendUi->spinBoxExtendPixels->value(); }
private:
	Ui::DialogMirrorExtend* mirrorExtendUi;
};


class MultiBlurDialog : public QDialog
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

class NewImageDialog : public QDialog
{
	Q_OBJECT

public:
	NewImageDialog(QWidget* parent = Q_NULLPTR) : QDialog(parent), newImgUi(new Ui::DialogNewImg)
	{
		newImgUi->setupUi(this);
	};
	int getWidth() { return newImgUi->spinBoxNewImgWidth->value(); }
	int getHeight() { return newImgUi->spinBoxNewImgHeight->value(); }
	QString getName() { return newImgUi->lineEdit->text(); }
private:
	Ui::DialogNewImg* newImgUi;
};

#endif