#include "MenuGenerator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MenuGenerator w;
	w.show();
	return a.exec();
}
