#include "qtpointgrey.hpp"


using namespace FlyCapture2;
using namespace std;

QtPointGrey::QtPointGrey(QObject* parent) :
QObject(parent){
	
}

QtPointGrey::~QtPointGrey() {
	
}

void QtPointGrey::LinkThread(QThread* thread) {
	connect(thread, SIGNAL(started()), this, SLOT(Run()));
}

void QtPointGrey::Run() {

	//InitSingleCamera();
	//FlyCapture2::Error error;

	//while (1) {
	//	error = cam.RetrieveBuffer(&rawImage);
	//	if (error != FlyCapture2::PGRERROR_OK)
	//	{
	//		//PrintError(error);
	//		qDebug("image buffer not ready");
	//		continue;
	//	}
	//	else
	//	{
	//		error = rawImage.Convert(FlyCapture2::PIXEL_FORMAT_MONO8, &img);
	//		if (error != FlyCapture2::PGRERROR_OK)
	//		{
	//			//PrintError(error);
	//			qDebug() << "error in raw image conversion";

	//		}
	//		else
	//		{
	//			cv::Mat mat;
	//			mat = cv::Mat(img.GetRows(), img.GetCols(), CV_8UC1, img.GetData());
	//			imshow("img", mat);
	//			cvWaitKey(1);
	//		}
	//	}
	//}

}

void QtPointGrey::PrintBuildInfo()
{
	FC2Version fc2Version;
	Utilities::GetLibraryVersion(&fc2Version);

	ostringstream version;
	version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
	cout << version.str() << endl;

	ostringstream timeStamp;
	timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
	cout << timeStamp.str() << endl << endl;
}

void QtPointGrey::PrintCameraInfo(CameraInfo* pCamInfo)
{
	cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number -" << pCamInfo->serialNumber << endl;
	cout << "Camera model - " << pCamInfo->modelName << endl;
	cout << "Camera vendor - " << pCamInfo->vendorName << endl;
	cout << "Sensor - " << pCamInfo->sensorInfo << endl;
	cout << "Resolution - " << pCamInfo->sensorResolution << endl;
	cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
	cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl << endl;

}

void QtPointGrey::PrintError(Error error)
{
	error.PrintErrorTrace();
}

int QtPointGrey::InitCameraExTrigger() {
	PrintBuildInfo();

	const int k_numImages = 10;

	Error error;

	//BusManager busMgr;
	unsigned int numCameras;
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	cout << "Number of cameras detected: " << numCameras << endl;

	if (numCameras < 1)
	{
		cout << "Insufficient number of cameras... exiting" << endl;
		return -1;
	}

	//PGRGuid guid;
	error = busMgr.GetCameraFromIndex(0, &guid);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	//Camera cam;

	// Connect to a camera
	error = cam.Connect(&guid);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Power on the camera
	const unsigned int k_cameraPower = 0x610;
	const unsigned int k_powerVal = 0x80000000;
	error = cam.WriteRegister(k_cameraPower, k_powerVal);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	const unsigned int millisecondsToSleep = 100;
	unsigned int regVal = 0;
	unsigned int retries = 10;

	// Wait for camera to complete power-up
	do
	{
		Sleep(millisecondsToSleep);

		error = cam.ReadRegister(k_cameraPower, &regVal);
		if (error == PGRERROR_TIMEOUT)
		{
			// ignore timeout errors, camera may not be responding to
			// register reads during power-up
		}
		else if (error != PGRERROR_OK)
		{
			PrintError(error);
			return -1;
		}

		retries--;
	} while ((regVal & k_powerVal) == 0 && retries > 0);

	// Check for timeout errors after retrying
	if (error == PGRERROR_TIMEOUT)
	{
		PrintError(error);
		return -1;
	}

	// Get the camera information
	CameraInfo camInfo;
	error = cam.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	PrintCameraInfo(&camInfo);


	// Get current trigger settings
	TriggerMode triggerMode;
	error = cam.GetTriggerMode(&triggerMode);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Set camera to trigger mode 0
	triggerMode.onOff = true;
	triggerMode.mode = 0;
	triggerMode.parameter = 0;


	// Triggering the camera externally using source 0.
	triggerMode.source = 3;


	error = cam.SetTriggerMode(&triggerMode);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Poll to ensure camera is ready
	bool retVal = PollForTriggerReady(&cam);
	if (!retVal)
	{
		cout << endl;
		cout << "Error polling for trigger ready!" << endl;
		qDebug() << "Error polling for trigger ready!";
		return -1;
	}

	// Get the camera configuration
	FC2Config config;
	error = cam.GetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Set the grab timeout to 5 seconds
	//config.grabTimeout = 5000;
	config.grabTimeout = 300;
	//config.grabMode =
	// Set the camera configuration
	error = cam.SetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Camera is ready, start capturing images
	error = cam.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}


	//Image image;
	//for (int imageCount = 0; imageCount < k_numImages; imageCount++)
	//{

	//	// Grab image
	//	error = cam.RetrieveBuffer(&image);
	//	if (error != PGRERROR_OK)
	//	{
	//		PrintError(error);
	//		break;
	//	}
	//	cout << "." << endl;
	//}

	//

	return 0;
}

int QtPointGrey::InitCameraSoftwareTrigger() {
	PrintBuildInfo();


	Error error;

	unsigned int numCameras;
	
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	cout << "Number of cameras detected: " << numCameras << endl;

	if (numCameras < 1)
	{
		cout << "Insufficient number of cameras... exiting" << endl;
		return -1;
	}


	error = busMgr.GetCameraFromIndex(0, &guid);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}


	// Connect to a camera
	error = cam.Connect(&guid);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Power on the camera
	const unsigned int k_cameraPower = 0x610;
	const unsigned int k_powerVal = 0x80000000;
	error = cam.WriteRegister(k_cameraPower, k_powerVal);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	const unsigned int millisecondsToSleep = 100;
	unsigned int regVal = 0;
	unsigned int retries = 10;

	// Wait for camera to complete power-up
	do
	{

		Sleep(millisecondsToSleep);

		error = cam.ReadRegister(k_cameraPower, &regVal);
		if (error == PGRERROR_TIMEOUT)
		{
			// ignore timeout errors, camera may not be responding to
			// register reads during power-up
		}
		else if (error != PGRERROR_OK)
		{
			PrintError(error);
			return -1;
		}

		retries--;
	} while ((regVal & k_powerVal) == 0 && retries > 0);

	// Check for timeout errors after retrying
	if (error == PGRERROR_TIMEOUT)
	{
		PrintError(error);
		return -1;
	}

	// Get the camera information
	CameraInfo camInfo;
	error = cam.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	PrintCameraInfo(&camInfo);


	// Check for external trigger support
	TriggerModeInfo triggerModeInfo;
	error = cam.GetTriggerModeInfo(&triggerModeInfo);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	if (triggerModeInfo.present != true)
	{
		cout << "Camera does not support external trigger! Exiting..." << endl;
		return -1;
	}


	// Get current strobe settings
	StrobeControl strobeControl;
	error = cam.GetStrobe(&strobeControl);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	//Set camera to strobe low on GPIO3
	strobeControl.onOff = true;
	strobeControl.source = 3;
	strobeControl.polarity = 0;
	strobeControl.delay = 0.f;
	strobeControl.duration = 10.f;

	error = cam.SetStrobe(&strobeControl);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}



	// Get current trigger settings
	TriggerMode triggerMode;
	error = cam.GetTriggerMode(&triggerMode);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Set camera to trigger mode 0
	triggerMode.onOff = true;
	triggerMode.mode = 0;
	triggerMode.parameter = 0;

	// A source of 7 means software trigger
	triggerMode.source = 7;


	error = cam.SetTriggerMode(&triggerMode);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Poll to ensure camera is ready
	bool retVal = PollForTriggerReady(&cam);
	if (!retVal)
	{
		cout << endl;
		cout << "Error polling for trigger ready!" << endl;
		return -1;
	}

	// Get the camera configuration
	FC2Config config;
	error = cam.GetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Set the grab timeout to 5 seconds
	config.grabTimeout = 2000;

	// Set the camera configuration
	error = cam.SetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Camera is ready, start capturing images
	error = cam.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}


	if (!CheckSoftwareTriggerPresence(&cam))
	{
		qDebug() << "SOFT_ASYNC_TRIGGER not implemented on this camera!  Stopping application";
		cout << "SOFT_ASYNC_TRIGGER not implemented on this camera!  Stopping application" << endl;
		return -1;
	}

	return 0;
}

bool QtPointGrey::FireSoftwareTrigger(Camera* pCam)
{
	const unsigned int k_softwareTrigger = 0x62C;
	const unsigned int k_fireVal = 0x80000000;
	Error error;

	error = pCam->WriteRegister(k_softwareTrigger, k_fireVal);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return false;
	}

	return true;
}


int QtPointGrey::InitSingleCamera() {
	PrintBuildInfo();

	Error error;

	unsigned int numCameras;
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	cout << "Number of cameras detected: " << numCameras << endl;

	for (unsigned int i = 0; i < numCameras; i++)
	{
		error = busMgr.GetCameraFromIndex(i, &guidTemp);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return -1;
		}
		else {
			guid = guidTemp;
		}
	}

	// Connect to a camera
	error = cam.Connect(&guid);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Get the camera information
	CameraInfo camInfo;
	error = cam.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	PrintCameraInfo(&camInfo);

	
	// Start capturing images

	//error = cam.SetCallback(OnImageGrabbed, cvMat);

	//if (error != PGRERROR_OK)
	//{
	//	PrintError(error);
	//	return -1;
	//}

	error = cam.StartCapture();


	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	return 1;

	//if (!pgrey.InitSingleCamera()) {
		//		return 0;
		//	}
		//			
		//	FlyCapture2::Error error = pgrey.cam.StartCapture(OnImageGrabbed);
		//
		//	if (error != PGRERROR_OK)  {
		//		cout << "start capture failed" << endl;
		//		return 0;

		//	}
}

cv::Mat QtPointGrey::PointGreyImageToMat(Image convertedImage) {
	
	cv::Mat mat;
	//unsigned int rowBytes = (double)convertedImage.GetReceivedDataSize() / (double)convertedImage.GetRows();
	mat = cv::Mat(convertedImage.GetRows(), convertedImage.GetCols(), CV_8UC1, convertedImage.GetData());
	return mat;
}

bool QtPointGrey::CloseSingleCamera() {
	// Stop capturing images
	Error error;
	error = cam.StopCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return false;
	}

	// Disconnect the camera
	error = cam.Disconnect();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return false;
	}

	return true;
}

bool QtPointGrey::RunSingleCamera(cv::Mat* mat2) {

	Error error;

	//cv::Mat mat;

	//for (;;) {
	// Retrieve an image
	error = cam.RetrieveBuffer(&rawImage);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		//return false;
	}

	// Create a converted image
	Image convertedImage;

	// Convert the raw image
	error = rawImage.Convert(PIXEL_FORMAT_MONO8, &convertedImage);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return false;
	}

	//unsigned int rowBytes = (double)convertedImage.GetReceivedDataSize() / (double)convertedImage.GetRows();
	//mat = cv::Mat(convertedImage.GetRows(), convertedImage.GetCols(), CV_8UC1, convertedImage.GetData());
	*mat2 = cv::Mat(convertedImage.GetRows(), convertedImage.GetCols(), CV_8UC1, convertedImage.GetData());


	//mat = pgrey.RunSingleCamera();
	//cv::imshow("DSfwef", *mat2);

	//if (cv::waitKey(1) == 27) {

	//	//CloseSingleCamera();
	//	
	//	return false;
	//}

	//}

	return true;
	//return 0;
}


//void OnImageGrabbed(FlyCapture2::Image* pImage, const void* pCallbackData);
//
//void QtPointGrey::OnImageGrabbed(Image* pImage, const void* pCallbackData)
//{
//	qDebug() << "camera callback triggered";
//	//cout << pImage->GetTimeStamp().microSeconds << endl;
//	//elapsed = milliseconds_now() - start;
//	//cout << elapsed << endl;
//
//	FlyCapture2::Error error;
//
//	FlyCapture2::Image img;
//	// Convert the raw image
//	error = pImage->Convert(PIXEL_FORMAT_MONO8, &img);
//	if (error != PGRERROR_OK)
//	{
//		//PrintError(error);
//		qDebug() << "error in image capture";
//
//
//	}
//	else {
//		//cv::Mat mat = QtPointGreyPointGreyImageToMat(&img);
//		
//		//unsigned int rowBytes = (double)convertedImage.GetReceivedDataSize() / (double)convertedImage.GetRows();
//		cv::Mat* mat  = new cv::Mat(img.GetRows(), img.GetCols(), CV_8UC1, img.GetData());
//		pCallbackData = &mat;
//
//		//emit(ImageCaptured());
//		//cv::imshow("cam", mat);
//		//cv::waitKey(1);
//
//		//qDebug() << "image captured";
//	}
//	//imageIsFresh = true;
//
//}

bool QtPointGrey::PollForTriggerReady(Camera* pCam)
{
	const unsigned int k_softwareTrigger = 0x62C;
	Error error;
	unsigned int regVal = 0;

	do
	{
		error = pCam->ReadRegister(k_softwareTrigger, &regVal);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return false;
		}

	} while ((regVal >> 31) != 0);

	return true;
}

bool QtPointGrey::CheckSoftwareTriggerPresence(Camera* pCam)
{
	const unsigned int k_triggerInq = 0x530;

	Error error;
	unsigned int regVal = 0;

	error = pCam->ReadRegister(k_triggerInq, &regVal);

	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return false;
	}

	if ((regVal & 0x10000) != 0x10000)
	{
		return false;
	}

	return true;
}