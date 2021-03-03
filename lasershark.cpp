#include "lasershark.hpp"

using namespace cv;
using namespace std;

Lasershark::Lasershark(QObject* parent) :
QObject(parent){
	
}

Lasershark::~Lasershark() {
	
}


void Lasershark::Run() {

	//QSerialPort serialPort;
	//QtPointGrey pgrey;
	//QtSerial serial(&serialPort);	

	//serial.Init();
	//pgrey.InitSingleCamera();

	//Init(&serial, &pgrey);

	FlyCapture2::Error error;

	bool flag = false;
	//serial->WriteSync("beginxxxx");
	char buf[255];

	while (1) {
		
		serial->ClearRX();

		// Check that the trigger is ready
		pgrey->PollForTriggerReady(&pgrey->cam);

	

		// Fire software trigger
		bool retVal = pgrey->FireSoftwareTrigger(&pgrey->cam);
		if (!retVal){
			qDebug() << "Error firing software trigger";	
		}
		error = pgrey->cam.RetrieveBuffer(&rawImg);

		if (error != FlyCapture2::PGRERROR_OK){
			
			qDebug("image buffer not ready");
		}
		else {
		
			error = rawImg.Convert(FlyCapture2::PIXEL_FORMAT_MONO8, &img);
			if (error != FlyCapture2::PGRERROR_OK)
			{
				//PrintError(error);
				qDebug() << "error in raw image conversion";

			}
			else
			{
				cv::Mat mat;
				mat = cv::Mat(img.GetRows(), img.GetCols(), CV_8UC1, img.GetData());
				cv::Mat result = cv::Mat::zeros(cv::Size(4, 1), CV_64F);
				vector<cv::Mat> posVec;
				vector<Point2f> imgVec;
				//read serial message
				imshow("rfewf", mat);
				waitKey(1);

				if (FindTarget(mat, &result, &posVec, &imgVec)) {

					if (serial->ReadSync(buf)) {

						serial->ClearTX();

						Point2f panTilt = ParsePanTiltString(buf);
						
						//qDebug() << panTilt.x;
						//qDebug() << panTilt.y;

						Point2f panTiltRad;
						panTiltRad.x = (300.f*(panTilt.x - 512.f)) / 1024.f;
						panTiltRad.y = (300.f*(panTilt.y - 512.f)) / 1024.f;

						Point3f centre = ComputeTargetCentre(posVec);
						Mat cam = CreateTransformMatrix(0.f, 0.f, 0.f, centre.x, centre.y, centre.z);
						Mat t3 = CreateTransformMatrix(0.f, 0.f, 180.f, 0.f, 0.f, 0.f);
						Mat t2 = CreateTransformMatrix(panTiltRad.y, 0.f, 0.f, 0.f, 0.f, 0.f);
						Mat t1 = CreateTransformMatrix(0.f, panTiltRad.x, 0.f, 0.f, 0.f, 0.f);

						Mat pan = t1*t2*t3*cam;
						
					/*	qDebug() << pan.at<double>(0, 3);
						qDebug() << pan.at<double>(0, 3);
						qDebug() << pan.at<double>(0, 3);*/

						window->AddDataPoint(pan.at<double>(0, 3), pan.at<double>(1, 3), pan.at<double>(2, 3));

						//Rect2f roi = Rect2f(120, 90, 400, 300);

						Rect2f roi = Rect2f(160, 120, 320, 240 );
						Point2i delta = Point2i(32, 32);
						Point2i newPanTilt;

						if (EdgeTrack(imgVec, roi, panTilt, delta, &newPanTilt)) {

							string panStr = to_string(newPanTilt.x);
							string tiltStr = to_string(newPanTilt.y);

							string moveCommand = "s0" + panStr + '0' + tiltStr;
							serial->WriteSync((char*)moveCommand.c_str());
							QThread::msleep(50);
						}
						//QThread::msleep(20);

					}else {
						qDebug() << "could not read serial";
					}

				}

			}
		


		}

	}

}



void Lasershark::LinkThread(QThread* thread) {
	connect(thread, SIGNAL(started()), this, SLOT(Run()));
}

bool Lasershark::LinkUI(MainWindow* _window) {
	window = _window;
	//connect(this, SIGNAL(Lasershark::DataPointReady()), window, SLOT(MainWindow::AddDataPoint()));
	return true;
}

//void Lasershark::DataPointReady() {
//
//}

bool Lasershark::Init(Serial* _serial, QtPointGrey* _pgrey) {
	serial = _serial;
	pgrey = _pgrey;

	myTimer.start();

	for (int i = 0; i < vertCorners; i++) {
		for (int j = 0; j < horzCorners; j++) {

			objectPoints.push_back(Point3f(float(j *squaresize), float(i*squaresize), 0.0f));
		}
	}

	cv::FileStorage fsread("calibdata.yml", cv::FileStorage::READ);
	fsread["cameraMatrix"] >> cameraMatrix2;
	//qDebug() << cameraMatrix2;
	fsread["distCoeffs"] >> distCoeffs2;
	//qDebug() << distCoeffs2;
	fsread.release();

	


	//connect(pgrey, SIGNAL(ImageCaptured()), this, SLOT(ProcessImage()));
	return true;
}

void Lasershark::ProcessImage() {
	qDebug() << "entered process image function";

}


bool Lasershark::FindTarget(cv::Mat frame, cv::Mat* result, vector<Mat>* posVec, vector<Point2f>* imgVec) {

	if (!cv::findChessboardCorners(frame, cv::Size(horzCorners, vertCorners), imagePoints2, 
		//CALIB_CB_ADAPTIVE_THRESH |CALIB_CB_NORMALIZE_IMAGE|  CALIB_CB_FAST_CHECK)) {
		CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK)) {
		///*CALIB_CB_ADAPTIVE_THRESH |CALIB_CB_NORMALIZE_IMAGE|*/  CALIB_CB_FAST_CHECK)) {
		qDebug() << "could not find chessboard";
		return false;
	}
	else {
		if (!cv::solvePnP(objectPoints, imagePoints2, cameraMatrix2, distCoeffs2, rvec2, tvec2, false, 0)) {
			qDebug() << "could not find pnp solution";
			return false;
		}
		else {
			
			*imgVec = imagePoints2;
			//convert rotations into rotation matrix
			cv::Rodrigues(rvec2, rmat);

			//create 4x4 transform matrix from 3x3 rotation and 3x1 translation matrix
			for (int i = 0; i < rmat.rows; i++) {
				for (int j = 0; j < rmat.rows; j++) {
					rmat.row(i).col(j).copyTo(rt.row(i).col(j));
				}
			}
			tvec2.row(0).col(0).copyTo(rt.row(0).col(3));
			tvec2.row(1).col(0).copyTo(rt.row(1).col(3));
			tvec2.row(2).col(0).copyTo(rt.row(2).col(3));

			
			//compute and store real world 3d positions in a new vector
			pos3dvec.clear();

			for (int i = 0; i < objectPoints.size(); i++) {
				model.row(0).col(0).setTo(objectPoints.at(i).x);
				model.row(1).col(0).setTo(objectPoints.at(i).y);
				model.row(2).col(0).setTo(objectPoints.at(i).z);
				//cout << model << endl;
				pos3d = rt*model;
				pos3dvec.push_back(pos3d);
				//cout << pos3d << endl;
			}
			*posVec = pos3dvec;


			
			return true;
			
		}
	}

	return false;
}




Mat Lasershark::CreateTransformMatrix(double alpha, double beta, double gamma, double dx, double dy, double dz)
{
	//alpha = (alpha - 90.)*CV_PI / 180.;
	//beta = (beta - 90.)*CV_PI / 180.;
	//gamma = (gamma - 90.)*CV_PI / 180.;
	alpha = alpha*CV_PI / 180.;
	beta = beta*CV_PI / 180.;
	gamma = gamma*CV_PI / 180.; 
	// get width and height for ease of use in matrices
	//double w = (double)input.cols;
	//double h = (double)input.rows;
	// Projection 2D -> 3D matrix
	//Mat A1 = (Mat_<double>(4, 3) <<
	//	1, 0, -w / 2,
	//	0, 1, -h / 2,
	//	0, 0, 0,
	//	0, 0, 1);
	// Rotation matrices around the X, Y, and Z axis
	Mat RX = (Mat_<double>(4, 4) <<
		1, 0, 0, 0,
		0, cos(alpha), -sin(alpha), 0,
		0, sin(alpha), cos(alpha), 0,
		0, 0, 0, 1);
	Mat RY = (Mat_<double>(4, 4) <<
		cos(beta), 0, sin(beta), 0,
		0, 1, 0, 0,
		-sin(beta), 0, cos(beta), 0,
		0, 0, 0, 1);
	Mat RZ = (Mat_<double>(4, 4) <<
		cos(gamma), -sin(gamma), 0, 0,
		sin(gamma), cos(gamma), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	// Composed rotation matrix with (RX, RY, RZ)
	Mat R = RX * RY * RZ;
	// Translation matrix
	Mat T = (Mat_<double>(4, 4) <<
		1, 0, 0, dx,
		0, 1, 0, dy,
		0, 0, 1, dz,
		0, 0, 0, 1);
	// 3D -> 2D matrix
	//Mat A2 = (Mat_<double>(3, 4) <<
	//	f, 0, w / 2, 0,
	//	0, f, h / 2, 0,
	//	0, 0, 1, 0);
	// Final transformation matrix
	//Mat trans = A2 * (T * (R * A1));
	// Apply matrix transformation
	//warpPerspective(input, output, trans, input.size(), INTER_LANCZOS4);

	return T*R;
}



Point3d Lasershark::ComputeTargetCentre(vector<cv::Mat> targetPoints) {
	
	Point3d centre;

	for (int i = 0; i < targetPoints.size(); i++) {
		
		centre.x += targetPoints.at(i).at<double>(0, 0);
		centre.y += targetPoints.at(i).at<double>(1, 0);
		centre.z += targetPoints.at(i).at<double>(2, 0);
		
		//cout << pos3d << endl;
	}

	centre.x = centre.x / targetPoints.size();
	centre.y = centre.y / targetPoints.size();
	centre.z = centre.z / targetPoints.size();

	return centre;
}

//finds centre of target in Image plane
Point2d Lasershark::ComputeImgCentre(vector<Point2f> imgPoints) {
	
	Point2d centre;

	for (int i = 0; i < imgPoints.size(); i++) {

		centre.x += imgPoints.at(i).x;
		centre.y += imgPoints.at(i).y;
		//cout << pos3d << endl;
	}

	centre.x = centre.x / imgPoints.size();
	centre.y = centre.y / imgPoints.size();


	return centre;
}

bool Lasershark::EdgeTrack(vector<Point2f> corners, Rect2f rect, Point2f _oldPanTilt, Point2i delta, Point2i* newPanTilt) {

	Point2d centre;
	//Point2i newPanTilt;
	Point2i oldPanTilt;

	bool movePan = true;
	bool moveTilt = true;

	int servoMax = 960;
	int servoMin = 64;

	oldPanTilt.x = (int)_oldPanTilt.x;
	oldPanTilt.y = (int)_oldPanTilt.y;

	for (int i = 0; i < corners.size(); i++) {

		centre.x += corners.at(i).x;
		centre.y += corners.at(i).y;
		//cout << pos3d << endl;
	}

	centre.x = centre.x / corners.size();
	centre.y = centre.y / corners.size();

	float xmax = rect.x + rect.width;
	float xmin = rect.x;
	float ymin = rect.y;
	float ymax = rect.y + rect.height;

	if (centre.x > xmax) {
		if ((oldPanTilt.x - delta.x) > servoMin){
			newPanTilt->x = oldPanTilt.x - delta.x;
		} else {
			movePan = false;
		}
	}	else if (centre.x < xmin) {
		if ((oldPanTilt.x + delta.x) < servoMax) {
			newPanTilt->x = oldPanTilt.x + delta.x;
		} else {
			movePan = false;
		}
	}	else {
		newPanTilt->x = oldPanTilt.x;
		movePan = false;
	}

	if (centre.y > ymax) {
		if ((oldPanTilt.y + delta.y) < servoMax) {
			newPanTilt->y = oldPanTilt.y + delta.y;
		} else {
			moveTilt = false;
		}
	}
	else if (centre.y < ymin) {
		if ((oldPanTilt.y - delta.y) > servoMin) {
			newPanTilt->y = oldPanTilt.y - delta.y;
		} else {
			moveTilt = false;
		}
	}
	else {
		newPanTilt->y = oldPanTilt.y;
		moveTilt = false;
	}

	return movePan | moveTilt;

}


//gets pan and tilt values from a string and puts them into
//an opencv 2d point
Point2d Lasershark::ParsePanTiltString(char* buf) {

	String str = buf;
	Point2d  panTilt;
	int panStart = 0;
	int tiltStart = 0;

	for (int i = 0; i < str.length(); i++) {
		if (str[i] == 'p') {
			panStart = i;
		}

		else if (str[i] == 't') {
			tiltStart = i;
		}
	}

	string lel = str.substr(panStart + 1, tiltStart - panStart - 1);

	panTilt.x = stoi(str.substr(panStart + 1, tiltStart - panStart - 1));
	panTilt.y = stoi(str.substr(tiltStart + 1, str.length() - tiltStart - 1));
	

	return panTilt;
}


Point3d Lasershark::ApplyForwardKinematics(Point3f target, double pan, double tilt) {


	double ay = 50.0f;
	double bx = 25.0f;
	double by = 50.0f;
	double bz = 10.0f;

	//Point3f pos3d;
	Point3d pos3d;
	//cv::Mat t0 = cv::Mat::eye(4, 4, CV_64F);
	//pan
	Mat t1 = CreateTransformMatrix(0.0f, pan, 0.0f, 0.0f, 50.0f, 0.0f);
	//tilt
	Mat t2 = CreateTransformMatrix(tilt, 0.0f, 0.0f, 25.0f, 50.0f, 10.0f);
	//camera
	Mat t3 = CreateTransformMatrix(0.0f, 0.0f, 0.0f, target.x, target.y, target.z);

	Mat p = t3*t2*t1;

	pos3d.x = p.at<double>(0, 3);
	pos3d.y = p.at<double>(1, 3);
	pos3d.z = p.at<double>(2, 3);

	return pos3d;
}
//
//void Lasershark::RunExTrigger() {
//
//	//QSerialPort serialPort;
//	//QtPointGrey pgrey;
//	//QtSerial serial(&serialPort);	
//
//	//serial.Init();
//	//pgrey.InitSingleCamera();
//
//	//Init(&serial, &pgrey);
//
//	FlyCapture2::Error error;
//
//	bool flag = false;
//	//serial->WriteSync("beginxxxx");
//	char buf[255];
//
//	while (1) {
//		//serial->WriteSync("beginxxxx");
//		//serial->WriteSync("beginxxxx");*/
//		//serial->WriteSync("beginxxxx");
//		//QThread::msleep(5);
//		//qDebug() << myTimer.restart();
//		//QThread::msleep(20);
//
//
//
//		if (flag == false) {
//			serial->WriteSync("beginxxxx");
//		}
//
//
//
//
//		error = pgrey->cam.RetrieveBuffer(&rawImg);
//		//serial->WriteSync("finishxxx");
//		//qDebug() << myTimer.elapsed();
//
//		if (error != FlyCapture2::PGRERROR_OK)
//		{
//			//PrintError(error);
//			serial->WriteSync("finishxxx");
//			flag = false;
//			qDebug("image buffer not ready");
//			QThread::msleep(20);
//
//
//			//continue;
//		}
//		else
//		{
//			serial->WriteSync("finishxxx");
//
//			//QThread::msleep(5);
//			//qDebug() << "img captured";
//			error = rawImg.Convert(FlyCapture2::PIXEL_FORMAT_MONO8, &img);
//			if (error != FlyCapture2::PGRERROR_OK)
//			{
//				//PrintError(error);
//				qDebug() << "error in raw image conversion";
//
//			}
//			else
//			{
//				cv::Mat mat;
//				mat = cv::Mat(img.GetRows(), img.GetCols(), CV_8UC1, img.GetData());
//				cv::Mat result = cv::Mat::zeros(cv::Size(4, 1), CV_64F);
//				vector<cv::Mat> posVec;
//				vector<Point2f> imgVec;
//				//read serial message
//
//				if (FindTarget(mat, &result, &posVec, &imgVec)) {
//
//					if (serial->ReadSync(buf)) {
//
//						Point2f panTilt = ParsePanTiltString(buf);
//						//Point3f pos3d = ApplyForwardKinematics(centre, panTilt.x, panTilt.y);
//						Point2f panTiltRad;
//						panTiltRad.x = (300.f*(panTilt.x - 512.f)) / 1024.f;
//						panTiltRad.y = (300.f*(panTilt.y - 512.f)) / 1024.f;
//
//						Point3f centre = ComputeTargetCentre(posVec);
//						Mat cam = CreateTransformMatrix(0.f, 0.f, 0.f, centre.x, centre.y, centre.z);
//						Mat t3 = CreateTransformMatrix(0.f, 0.f, 180.f, 0.f, 0.f, 0.f);
//						Mat t2 = CreateTransformMatrix(panTilt.y, 0.f, 0.f, 0.f, 0.f, 0.f);
//						Mat t1 = CreateTransformMatrix(0.f, panTilt.x, 0.f, 0.f, 0.f, 0.f);
//
//						/*qDebug() << "cam:";
//						qDebug() << cam.at<double>(0, 3);
//						qDebug() << cam.at<double>(1, 3);
//						qDebug() << cam.at<double>(2, 3);
//
//						Mat world = t3*cam;
//
//						qDebug() << "world";
//						qDebug() << world.at<double>(0, 3);
//						qDebug() << world.at<double>(1, 3);
//						qDebug() << world.at<double>(2, 3);
//
//						Mat tilt = t2*t3*cam;
//
//						qDebug() << "tilt";
//						qDebug() << tilt.at<double>(0, 3);
//						qDebug() << tilt.at<double>(1, 3);
//						qDebug() << tilt.at<double>(2, 3);
//
//						Mat pan = t1*t2*t3*cam;
//
//						qDebug() << "pan";
//						qDebug() << pan.at<double>(0, 3);
//						qDebug() << pan.at<double>(1, 3);
//						qDebug() << pan.at<double>(2, 3);*/
//
//						Mat pan = t1*t2*t3*cam;
//
//						window->AddDataPoint(pan.at<double>(0, 3), pan.at<double>(0, 3), pan.at<double>(0, 3));
//
//						Rect2f roi = Rect2f(120, 90, 400, 300);
//
//
//						Point2i delta = Point2i(50, 50);
//						Point2i  newPanTilt = EdgeTrack(imgVec, roi, panTilt, delta);
//						string panStr = to_string(newPanTilt.x);
//						string tiltStr = to_string(newPanTilt.y);
//
//						//sprintf(panStr, "%d", newPanTilt.x);
//						//sprintf(tiltStr, "%d", newPanTilt.y);
//
//						string moveCommand = "s0" + panStr + '0' + tiltStr;
//						serial->WriteSync((char*)moveCommand.c_str());
//						QThread::msleep(20);
//						flag = false;
//						//QThread::msleep(30);
//
//					}
//
//				}
//
//			}
//			//QThread::msleep(5);
//
//
//		}
//
//	}
//
//
//}










//void Lasershark::Run() {

//pgrey.InitSingleCamera();
//QtSerial serial;

//for (int i = 0; i < vertCorners; i++) {
//	for (int j = 0; j < horzCorners; j++) {

//		objectPoints.push_back(Point3f(float(j *squaresize), float(i*squaresize), 0.0f));
//	}
//}

//cv::FileStorage fsread("calibdata.yml", cv::FileStorage::READ);
//fsread["cameraMatrix"] >> cameraMatrix2;
////qDebug() << cameraMatrix2;
//fsread["distCoeffs"] >> distCoeffs2;
////qDebug() << distCoeffs2;
//fsread.release();



//while (1) {

//	//retrieve image if available
//	error = pgrey.cam.RetrieveBuffer(&rawimg);
//	if (error != FlyCapture2::PGRERROR_OK)
//	{
//		//PrintError(error);
//		qDebug("image buffer not ready");
//		continue;
//	}
//	else {

//		//let arbotix know that serial is finished
//		//serial.Write("finishxxx");


//		// Convert the raw image
//		error = rawimg.Convert(FlyCapture2::PIXEL_FORMAT_MONO8, &img);
//		if (error != FlyCapture2::PGRERROR_OK)
//		{
//			//PrintError(error);
//			qDebug() << "error in raw image conversion";


//		}
//		else {
//			cv::Mat mat;
//			mat = cv::Mat(img.GetRows(), img.GetCols(), CV_8UC1, img.GetData());
//			cv::Mat result;

//			//if the targets position is found
//			if (FindTarget(mat, result)) {
//				//render the position on the UI

//			}

//			//tell arbotix to new capture cycle;
//			//serial.Write("startxxxx");

//		}
//	}



//}
//}

//
////checks if chessboard corners found are inside bounding rect
//bool Lasershark::CheckCornerInBounds(vector<Point2f> corners, Rect2f _rect) {
//
//	//float sumx = 0;
//	//float sumy = 0;
//
//	//for (int i = 1; i < corners.size(); i++) {
//	//	sumx += corners.at(i).x;
//	//	sumy += corners.at(i).y;
//	//}
//
//	//float xmean = mean(sumx);
//	//float ymean = mean(sumy);
//
//	Point2d centre;
//
//	for (int i = 0; i < corners.size(); i++) {
//
//		centre.x += corners.at(i).x;
//		centre.y += corners.at(i).y;
//		//cout << pos3d << endl;
//	}
//
//	centre.x = centre.x / corners.size();
//	centre.y = centre.y / corners.size();
//
//	if (centre.x < _rect.x || centre.x > _rect.x + _rect.width ||
//		centre.y < _rect.y || centre.y > _rect.y + _rect.height) {
//		return false;
//	}
//
//	return true;
//}
