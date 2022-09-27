#include "MainWindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QThread>

const unsigned int SLEEP_MS = 2500;


int main(int argc, char** argv) {

	QApplication app(argc, argv);

	QSplashScreen splash(QPixmap(":icons/splash.jpg"), Qt::WindowStaysOnTopHint);
	splash.show();
	
	QThread::msleep(SLEEP_MS);

	(new trexp::MainWindow)->showMaximized();

	splash.close();

	return app.exec();

}