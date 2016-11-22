#pragma once
#include <QObject>

#include <stdio.h>
#include <iostream>
#include "opencv2\opencv.hpp"
#include <math.h>
#include <string>
#include "armadillo"
#include "serial.hpp"

struct ikvals {
	float l;
	float d;
	float d1;
	float d2;
	float pan;
	float tilt;
};

class Tracker : public QObject {
	Q_OBJECT

public:
	Tracker(QObject * parent = Q_NULLPTR);
	~Tracker();


	Tracker(float cx, float cy, float _w, float _h, float _a1, float _a2, float _a3,
		float deltapan, float deltatilt);
	bool SimpleTrack(cv::Point2i oldPos, cv::Point2i newPos);
	arma::mat Calculate3DPosFK(ikvals ik, float x, float y, float z);

	arma::mat MakeTranslateMatrix(float x, float y, float z);
	arma::mat MakeXRotationMatrix(float rx);
	arma::mat MakeYRotationMatrix(float ry);
	arma::mat MakeZRotationMatrix(float rz);
	void PredictLocationEdge(cv::Point2f target, float newpan, float newtilt, float pan, float tilt);
	cv::Point3f ComputeTargetCenter(std::vector<cv::Mat> objpoints);

	//vector<Point3f> LocationBuffer;
	cv::Rect2f rect;
	float deltapan, deltatilt;
	float a1, a2, a3;
	//float pan, tilt;
	ikvals IkCalc(float x, float y, float  z);
	cv::Rect2f ComputeBounds(float cx, float cy, float width, float height);
	bool CheckCornerInBounds(std::vector<cv::Point2f> corners, cv::Rect2f rect);
	cv::Point3f PredictLocationVelocity();

	bool MoveToLocation(float x, float y, float  z);
	bool MovetoPanTilt(int pan, int tilt, Serial serial);



private:
	
};
