#include "serialportreader.hpp"

#include <QCoreApplication>

QT_USE_NAMESPACE

SerialPortReader::SerialPortReader()
{
	connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortReader::handleReadyRead);
	
}

SerialPortReader::~SerialPortReader()
{
}

void SerialPortReader::handleReadyRead()
{

	QByteArray serialData =  m_serialPort->readAll();
	QString temp = QString::fromStdString(serialData.toStdString());
	qDebug() << temp;
}

bool SerialPortReader::Write(char* str) {
	
	m_serialPort->write(str);
	return true;
}

