#pragma once
#include <QObject>
#include <QThread>
#include <QDebug>

#include "FlyCapture2.h"
#include "opencv2\opencv.hpp"
#include <QThread>
#include <iostream>
//need to define NOMINMAX before windows.h include to prevent q3dscatter macro errors
#define NOMINMAX 
#include <Windows.h>

using namespace FlyCapture2;

class QtPointGrey : public QObject {
	Q_OBJECT

public:
	explicit QtPointGrey(QObject* parent = 0);
	~QtPointGrey();

	//void run();

	//QtSerial serial;

	void LinkThread(QThread* thread);
	
	FlyCapture2::Image flycap_img;
	FlyCapture2::Image img;

	static void OnImageGrabbed(FlyCapture2::Image* pImage, const void* pCallbackData);

	void PrintBuildInfo();
	void PrintCameraInfo(FlyCapture2::CameraInfo* pCamInfo);
	void PrintError(FlyCapture2::Error error);
	bool RunSingleCamera(cv::Mat* mat2);
	int InitSingleCamera();
	int InitCameraExTrigger();
	int InitCameraSoftwareTrigger();
	bool PollForTriggerReady(Camera* pCam);
	bool CheckSoftwareTriggerPresence(Camera* pCam);
	
	bool FireSoftwareTrigger(Camera* pCam);

	cv::Mat PointGreyImageToMat(FlyCapture2::Image convertedImage);
	bool CloseSingleCamera();
	FlyCapture2::Camera cam;
	FlyCapture2::Image rawImage;

	cv::Mat* cvMat;

public slots :
	void Run();

signals:
	//static void ImageCaptured();

private:

	FlyCapture2::PGRGuid guid;
	FlyCapture2::PGRGuid guidTemp;
	FlyCapture2::BusManager busMgr;
	
};
