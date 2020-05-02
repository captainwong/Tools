#include "HashTool.h"
#include <QtWidgets/QApplication>
#include "HashView.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HashView w;
	w.show();
	return a.exec();
}
