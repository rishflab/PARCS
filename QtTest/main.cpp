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
	//QtSerial serial(&serialPort);
	Serial serial;
	if (!serial.InitSerial("COM3", 115200)) {
		qDebug() << "Serial failed to open";
		app.exit();
	}

	QtPointGrey pgrey;
	
	//pgrey.InitSingleCamera();
	if (!pgrey.InitCameraSoftwareTrigger()) {
		qDebug() << "Camera failed to start";
		app.exit();
	}

	//QtPointGrey pgrey;
	QThread* thread = new QThread();
	
	Lasershark lshark;
	lshark.Init(&serial, &pgrey);
	lshark.LinkUI(&w);
	
	//pgrey.LinkThread(thread);
	//pgrey.moveToThread(thread);

	//serial.LinkThread(thread);
	//serial.moveToThread(thread);

	lshark.LinkThread(thread);
	lshark.moveToThread(thread);



	thread->start();

	////QtPointGrey pgrey;
	////pgrey.InitSingleCamera();

	////QtSerial serial;
	////serial.Init();
	////	
	////Lasershark lshark;
	////lshark.Init(&serial, &pgrey);

	//
	w.show();

	app.exec();
}
