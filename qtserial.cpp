#include "qtserial.hpp"

QtSerial::QtSerial(QSerialPort *serialPort, QObject* parent)
	:QObject(parent)
	, m_serialPort(serialPort) {

	

}

bool QtSerial::Init() {

	m_serialPort->setPortName("com3");
	m_serialPort->setBaudRate(m_serialPort->Baud115200);
	if (!m_serialPort->open(QIODevice::ReadWrite)) {
		qDebug() << "would not open serial device";
		return false;
	}
	else {
		m_serialPort->write("ewfwef");
		m_serialPort->write("ewfwef");
		m_serialPort->write("ewfwef");
		m_serialPort->write("ewfwef");
		m_serialPort->write("ewfwef");


		connect(m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
			this, &QtSerial::handleError);

		return true;
	}
}

void QtSerial::LinkThread(QThread* thread) {
	connect(thread, SIGNAL(started()), this, SLOT(Run()));
}

void QtSerial::Run() {

	while (1) {
		QByteArray serialData = m_serialPort->readAll();
		QString temp = QString::fromStdString(serialData.toStdString());

		if (temp == "helloresponse") {
			//temp == "";
			qDebug() << temp;
			m_serialPort->write("Wefwef");
			//Write("ewfwef");
		}
	}
}

QtSerial::~QtSerial()
{
}

void QtSerial::handleReadyRead()
{

	QByteArray serialData = m_serialPort->readAll();
	QString temp = QString::fromStdString(serialData.toStdString());
	qDebug() << temp;
	Write("lol");
}

bool QtSerial::Write(char* str) {

	m_serialPort->write(str);
	return true;
}

void QtSerial::handleError(QSerialPort::SerialPortError serialPortError)
{
	if (serialPortError == QSerialPort::ReadError) {
		qDebug() << "An I/O error occurred while reading the data from port %1, error: %2";
	}
}
