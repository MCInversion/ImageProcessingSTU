#include "ImageViewer.h"
#include <QtWidgets/QApplication>

// DONE:
// - Extend image bounds by mirroring for masks (3x3 or higher) with adjustable extent (n)
// - Histogram window
// - Full Scale Histogram Stretch
// - Window resize event for histogram
// - Multiple channel histogram bounds
// - Normalized Cumulative histogram 
// - applying extend on stretched image breaks it down (bad data ptr)
// - add gaussian kernel matrix function paremetrized by t = sigma^2 / 2 (dialog with spin box)
// - ImageProcessor: filter with an NxN mask (arbitrary normalized matrix kernel)

// WIP:
// - asymmetrical gaussian kernel

// TODO:
// - UndoRedo (?)
// - background processing thread

// issues:
// - cannot delete QImage ptr, why?
// - repeated multi-channel stretching results in large variation in histogram values (possibly a feature)

// Solved issues:
// - Q_OBJECT in MirrorExtendDialog/HistogramWindow cannot be present (lnk 2001 error otherwise)
//	 both issues => regenerate build via CMake (Q_OBJECT macro necessary for signals & slots inside new QWindow)
// - after multiband histogram scaling mirror extend switches colors
//   => (kernel mask with integral > 1)
// - blur makes the picture darker
//   => (kernel mask with integral < 1)

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