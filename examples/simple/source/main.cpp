#include <QApplication>

#include <QMainWindow>
#include <Widget.h>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QMainWindow w;
	w.setCentralWidget(new Widget(&w));
	w.show();

	return app.exec();
}
