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
#include "../build/ui_PeronaMalikDialog.h"

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
	HeatEquationDialog(QWidget* parent = Q_NULLPTR) : QDialog(parent), heatEquationDialogUi(new Ui::heatEquationDialog)
	{
		heatEquationDialogUi->setupUi(this);
	}
	int getNSteps() { return heatEquationDialogUi->nStepsSpinBox->value(); }
	double getTimeStep() { return heatEquationDialogUi->timeStepSpinBox->value(); }
	int getScheme() { return heatEquationDialogUi->schemeComboBox->currentIndex(); }
private:
	Ui::heatEquationDialog* heatEquationDialogUi;
private slots:
	void bindTimeStep() {
		bool explicitScheme = heatEquationDialogUi->schemeComboBox->currentIndex() == 0;
		bool overrideTimeStep = heatEquationDialogUi->overrideCheckBox->isChecked();
		double tauMax = (!explicitScheme || overrideTimeStep ? 99. : 0.25);
		double tau = heatEquationDialogUi->timeStepSpinBox->value();
		heatEquationDialogUi->timeStepSpinBox->setRange(0.01, tauMax);
		heatEquationDialogUi->timeStepSpinBox->setValue(std::fminf(tau, tauMax));
	}
	void overrideTimeStep() {
		bindTimeStep();
	}
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

class PeronaMalikDialog : public QDialog
{
	Q_OBJECT

public:
	PeronaMalikDialog(QWidget* parent = Q_NULLPTR) : QDialog(parent), peronaMalikDialogUi(new Ui::peronaMalikDialog)
	{
		peronaMalikDialogUi->setupUi(this);
	};
	int getNSteps() { return peronaMalikDialogUi->nStepsSpinBox->value(); }
	double getTimeStep() { return peronaMalikDialogUi->timeStepSpinBox->value(); }
	int getScheme() { return peronaMalikDialogUi->schemeComboBox->currentIndex(); }
private:
	Ui::peronaMalikDialog* peronaMalikDialogUi;
private slots:
	void bindTimeStep() {
		bool explicitScheme = peronaMalikDialogUi->schemeComboBox->currentIndex() == 0;
		bool overrideTimeStep = peronaMalikDialogUi->overrideCheckBox->isChecked();
		double tauMax = (!explicitScheme || overrideTimeStep ? 99. : 0.25);
		double tau = peronaMalikDialogUi->timeStepSpinBox->value();
		peronaMalikDialogUi->timeStepSpinBox->setRange(0.01, tauMax);
		peronaMalikDialogUi->timeStepSpinBox->setValue(std::fminf(tau, tauMax));
	}
	void overrideTimeStep() {
		bindTimeStep();
	}
};

#endif