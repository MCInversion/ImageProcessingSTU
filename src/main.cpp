#include "ImageViewer.h"
#include <QtWidgets/QApplication>

// DONE:
// - Extend image bounds by mirroring for masks (3x3 or higher) with adjustable extent (n)
// - Histogram window
// - Full Scale Histogram Stretch
// - Window resize event for histogram
// - Multiple channel histogram bounds
// - Normalized Cumulative histogram 

// WIP:
// - applying extend on stretched image breaks it down (bad resolution)

// TODO:
// - ImageProcessor: filter with an NxN mask (arbitrary normalized matrix kernel)
// - gaussian filter
// - UndoRedo (?)

// issues:
// - cannot delete QImage ptr, why?
// - repeated multi-channel stretching results in large variation in histogram values

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