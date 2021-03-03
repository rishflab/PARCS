#pragma once
#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>

class QtSerial : public QObject {
	Q_OBJECT

public:
	explicit QtSerial(QSerialPort *serialPort, QObject* parent = 0);
	~QtSerial();

	bool Init();

	bool Write(char* str);

	void LinkThread(QThread* thread);

private slots:
	void handleReadyRead();
	void handleError(QSerialPort::SerialPortError error);
	void Run();


private:
	QSerialPort* m_serialPort;
	QByteArray  m_readData;
};
