#pragma once
#include <QObject>
#include <QThread>
#include <QDebug>
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
//need to define NOMINMAX before windows.h include to prevent q3dscatter macro errors
#define NOMINMAX 
#include <Windows.h>

#define BUFFERSIZE 255

class Serial : public QObject {
	Q_OBJECT

public:
	Serial(QObject * parent = Q_NULLPTR);
	~Serial();

	HANDLE hCom;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[BUFFERSIZE] = { 0 };

	BOOL dingdong = TRUE;
	BOOL fWaitingOnRead = FALSE;
	DWORD dwRead;
	OVERLAPPED osReader = { 0 };

	OVERLAPPED osWrite = { 0 };
	DWORD dwWritten;
	DWORD dwRes;
	BOOL fRes;

	char rdBuf[BUFFERSIZE];


	char command[10];

	//char lpBuf[7];

	bool InitSerial(char* comport, int baudrate);

	bool ClearTX();
	bool ClearRX();
	
	bool ReadSync(char* buf);

	bool WriteSync(char* buf);

	std::string HandleASuccessfulRead();

	bool CFind(char* bigstring, char* substring);

	bool Read(char** buf);

	BOOL WriteToCom(char* lpBuf);

	BOOL Close();

	DCB dcb = { 0 };

	void LinkThread(QThread* thread);

public slots:

	void Run();


private:
	
};
