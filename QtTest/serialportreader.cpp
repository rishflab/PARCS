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
	//m_readData.append(m_serialPort->readAll());

	QByteArray serialData =  m_serialPort->readAll();
	QString temp = QString::fromStdString(serialData.toStdString());
	qDebug() << temp;
	//if (!m_timer.isActive())
	//	m_timer.start(1000);
	//Write("lol");
}

bool SerialPortReader::Write(char* str) {
	
	m_serialPort->write(str);
	return true;
}

