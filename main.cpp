#include "mainwidget.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWidget w;
	w.show();

	w.move((QApplication::desktop()->width() - w.width()) / 2, (QApplication::desktop()->height() - w.height()) / 2);

	return a.exec();
}
