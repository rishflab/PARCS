#pragma once
#include <QObject>
#include "opencv2\opencv.hpp"
#include "marker.hpp"

using namespace cv;
using namespace std;



class Chessboard : public QObject {
	Q_OBJECT

public:
	Chessboard(QObject * parent = Q_NULLPTR);
	~Chessboard();

	void Init(const CvSize &size);


	/************************************************************************/
	/*
	corner detection function
	input:
	isTracking:    tracking marker position in continues frames
	DistThreshold: maxi distortion value in pixel,
	for HD camera, is in [2, 5], for web camera it may larger than 5
	output:
	pts2d:         chessboard corner position
	*/
	/************************************************************************/

	bool findCorners(const Mat &image, bool isTracking, vector<Point2f> &outCorners,
		int distortionThreshold = 5);

private:

	CvSize patternSize;				  //chessboard size
	vector<Point2f> boardCorners;     //physical position of corners in 2d
	vector<Point2f> markerCenters;    //physical position of marker centers
	vector<Marker> preMarkers;     //previous frame marker position
	vector<Marker> curMarkers;     //current frame marker position

									  /************************************************************************/
									  /*
									  imageSize: such as 1920 * 1080
									  boradSize: such as 14 * 10
									  thMax: threshold of maximum perimeter of candidate marker
									  thMin: threshold of minimum perimeter of candidate marker
									  */
									  /************************************************************************/
	static void GetThreshold(const CvSize &imageSize, const CvSize &boardSize, int &thMax, int &thMin);

	//find homography of 4 correspondings without scaling
	static Mat findHomography4Pts(const vector<Point2f> &src, const vector<Point2f> &dst);

	
};

