#pragma once
#include <QtWidgets/QDialog>
#include <QtWidgets>

#include "../build/ui_MirrorExtendDialog.h"

class MirrorExtendDialog :	public QDialog
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

