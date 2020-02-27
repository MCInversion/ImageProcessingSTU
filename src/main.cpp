#include "ImageViewer.h"
#include <QtWidgets/QApplication>

// TODO:
// - Extend image bounds by mirroring for masks (3x3 or higher) with adjustable extent (n)
// - Histogram window
// - Full Scale Histogram Stretch
// - Normalized Cumulative histogram window

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