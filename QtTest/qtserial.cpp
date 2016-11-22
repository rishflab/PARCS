#include "qtserial.hpp"

QtSerial::QtSerial(QSerialPort *serialPort, QObject* parent)
	:QObject(parent)
	, m_serialPort(serialPort) {

	

}

bool QtSerial::Init() {

	//m_serialPort = qSerialPort;
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

		//connect(m_serialPort, &QSerialPort::readyRead, this, &QtSerial::handleReadyRead);
		connect(m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
			this, &QtSerial::handleError);

		return true;
	}
}

void QtSerial::LinkThread(QThread* thread) {
	connect(thread, SIGNAL(started()), this, SLOT(Run()));
}

void QtSerial::Run() {

	//QSerialPort s;
	//m_serialPort = &s;
	//m_serialPort->setPortName("com3");
	//m_serialPort->setBaudRate(m_serialPort->Baud115200);
	//if (!m_serialPort->open(QIODevice::ReadWrite)) {
	//	qDebug() << "would not open serial device";
	//	//return false;
	//}
	//else {
	//	m_serialPort->write("ewfwef");
	//	m_serialPort->write("ewfwef");
	//	m_serialPort->write("ewfwef");
	//	m_serialPort->write("ewfwef");
	//	m_serialPort->write("ewfwef");

	//	//connect(m_serialPort, &QSerialPort::readyRead, this, &QtSerial::handleReadyRead);
	//	connect(m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
	//		this, &QtSerial::handleError);

	//	//return true;
	//}

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
	//m_readData.append(m_serialPort->readAll());

	QByteArray serialData = m_serialPort->readAll();
	QString temp = QString::fromStdString(serialData.toStdString());
	qDebug() << temp;
	//if (!m_timer.isActive())
	//	m_timer.start(1000);
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
		//QCoreApplication::exit(1);
	}
}
