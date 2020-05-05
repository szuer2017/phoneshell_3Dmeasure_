#include "pclviewer.h"
#include <QtWidgets/QApplication.h>
#include <QMainWindow>

int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	PCLViewer w;
	w.setWindowTitle("3DCloud_Measure");
	std::cout << " ----Welcome to Use 3D  PointCloud  Geometry Measurement Tool ---" << endl;
	
	w.show();
	return a.exec();
}
