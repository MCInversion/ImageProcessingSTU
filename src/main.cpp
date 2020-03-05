#include "ImageViewer.h"
#include <QtWidgets/QApplication>

// DONE:
// - Extend image bounds by mirroring for masks (3x3 or higher) with adjustable extent (n)

// TODO:
// - Histogram window
// - Full Scale Histogram Stretch
// - Normalized Cumulative histogram window

// Unexplainable issues:
// - Q_OBJECT in MirrorExtendDialog cannot be present (lnk 2001 error otherwise)
// - cannot delete QImage ptr, why?

int main(int argc, char* argv[])
{
	QLocale::setDefault(QLocale::c());

	QCoreApplication::setOrganizationName("MPM");
	QCoreApplication::setApplicationName("ImageViewer");

	QApplication a(argc, argv);
	ImageViewer w;
	w.show();
	return a.exec();
}