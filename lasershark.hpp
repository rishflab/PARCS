#pragma once
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QElapsedTimer>
#include <iostream>
#include "FlyCapture2.h"
#include "qtpointgrey.hpp"
#include "serial.hpp"
#include "mainwindow.hpp"

using namespace std;
using namespace cv;
class Lasershark : public QObject {
	Q_OBJECT

public:
	explicit Lasershark(QObject* parent = 0);
	~Lasershark();

	bool Init(Serial* serial, QtPointGrey* pgrey);
	bool FindTarget(cv::Mat frame, cv::Mat* result, vector<Mat>* posVec, vector<Point2f>* imgVec);
	void LinkThread(QThread* thread);
	bool LinkUI(MainWindow* _window);
	Point3d ComputeTargetCentre(vector<Mat> pos3dvec);
	Point2d ComputeImgCentre(vector<Point2f> pos2dvec);
	//Point2i EdgeTrack(vector<Point2f> corners, Rect2f rect, Point2f _oldPanTilt, Point2i delta);
	bool EdgeTrack(vector<Point2f> corners, Rect2f rect, Point2f _oldPanTilt, Point2i delta, Point2i* newPanTilt);
	Mat CreateTransformMatrix(double alpha, double beta, double gamma, double dx, double dy, double dz);
	Point3d ApplyForwardKinematics(Point3f target, double pan, double tilt);
	Point2d ParsePanTiltString(char* buf);

signals:
	void DataPointReady();

public slots :

	void ProcessImage();
	void Run();


private:

	cv::Mat cameraMatrix2;
	cv::Mat distCoeffs2;

	cv::Mat pos3d;

	std::vector<cv::Point3f> objectPoints;

	int horzCorners = 3;
	int vertCorners = 3;
	float squaresize = 53.82f;
	//float squaresize = 49.20f;

	bool chessboardFound = true;
	cv::Mat rvec2 = cv::Mat::ones(3,1, CV_64F);
	cv::Mat tvec2 = cv::Mat::ones(3,1, CV_64F);

	std::vector<cv::Point2f> imagePoints2;

	std::vector<cv::Mat> pos3dvec;

	cv::Mat rmat;

	cv::Mat rt = cv::Mat::eye(4, 4, CV_64F);

	cv::Mat model = cv::Mat::ones(4, 1, CV_64F);

	FlyCapture2::Error error;

	FlyCapture2::Image img;
	FlyCapture2::Image rawImg;

	QElapsedTimer myTimer;

	Serial* serial;
	
	QtPointGrey* pgrey;

	MainWindow* window;
};
