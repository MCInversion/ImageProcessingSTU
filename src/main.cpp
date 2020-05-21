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

// - Bernsen threshold
// - Isodata threshold
// - image sequence (slider)
// - enable/disable slider upon changing tabs
// - unify dialog classes in a single header
// - Explicit scheme solver for heat eqn
// - Implicit SOR solver for heat eqn
// - mean value test for heat eqn
//
// - Perona-Malik basic
// - Perona-Malik regularized
// - use smoothed gradients for s = |grad Gsigma * u| into g(s) = 1 / (1 + K s^2)
// - check if all image dimensions are correct

// WIP:
//

// TODO:
//
//
// - UndoRedo (?)
// - background processing thread (major problem with viewerWidget - the processing thread of the image processor cannot write to it)
// - active img <--> widgets communication/update

// issues:
// - problems with freeing QImage memory as well
// - cannot delete QPainter ptr, why?
// - repeated multi-channel stretching results in large variation in histogram values (possibly a feature)
// - the ViewerWidget implementation makes it very inconvenient to perform any background computation in a separate thread

// Solved issues:
// - unifying dialog classes into a single header still requires a .cpp file for auto MOC
// - Q_OBJECT in MirrorExtendDialog/HistogramWindow cannot be present (lnk 2001 error otherwise)
//	 both issues => regenerate build via CMake (Q_OBJECT macro necessary for signals & slots inside new QWindow)
// - after multiband histogram scaling mirror extend switches colors
//   => (kernel mask with integral > 1)
// - blur makes the picture darker
//   => (kernel mask with integral < 1)
// - asymmetrical gaussian kernel

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