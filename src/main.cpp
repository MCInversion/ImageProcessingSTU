#include "ImageViewer.h"
#include <QtWidgets/QApplication>

// DONE:
// - Extend image bounds by mirroring for masks (3x3 or higher) with adjustable extent (n)
// - Histogram window

// TODO:
// - Full Scale Histogram Stretch
// - Normalized Cumulative histogram window
// - filter with an NxN mask (arbitrary normalized matrix kernel)
// - gaussian filter

// Unexplainable issues:
// - cannot delete QImage ptr, why?

// Solved issues:
// - Q_OBJECT in MirrorExtendDialog/HistogramWindow cannot be present (lnk 2001 error otherwise)
// both issues => regenerate build via CMake (Q_OBJECT macro necessary for signals & slots inside new QWindow)

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