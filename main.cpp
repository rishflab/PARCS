#include "qtserial.hpp"
#include "qtpointgrey.hpp"
#include "lasershark.hpp"
#include "mainwindow.hpp"
#include "serial.hpp"
#include <QTextStream>
#include <QCoreApplication>
#include <QApplication>
#include <QFile>
#include <QStringList>
#include <QThread>

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	

	MainWindow w;

	QSerialPort serialPort;

	Serial serial;
	if (!serial.InitSerial("COM3", 115200)) {
		qDebug() << "Serial failed to open";
		app.exit();
	}

	QtPointGrey pgrey;

	if (!pgrey.InitCameraSoftwareTrigger()) {
		qDebug() << "Camera failed to start";
		app.exit();
	}

	QThread* thread = new QThread();
	
	Lasershark lshark;
	lshark.Init(&serial, &pgrey);
	lshark.LinkUI(&w);

	lshark.LinkThread(thread);
	lshark.moveToThread(thread);

	thread->start();

	w.show();

	app.exec();
}
