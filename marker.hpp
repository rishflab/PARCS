#pragma once
#include <QObject>
#include <opencv2\core.hpp>
#include <opencv2\features2d.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv\cv.h>
#include "opencv2/highgui.hpp"
#include <vector>


using namespace cv;
using namespace std;

struct MarkerParameters
{
	MarkerParameters()
	{
		thresholdParameter = 7.0;
		minPixels = 100;
		maxPixels = 560;
	}
	float thresholdParameter;
	int   minPixels;    //minumum pixel numbers of perimeter
	int   maxPixels;    //minumum pixel numbers of perimeter
};


class Marker  {
public:
	//Marker(QObject * parent = Q_NULLPTR);
	Marker();
	~Marker();

	void getCenterPoints(void);
	bool getSubArea(int dilateLength, int w, int h);
	//addOffset is used in marker tracking, in sub images
	void addOffset(int x, int y);
	bool operator<(const Marker &other)const;

	int id;   //0-1023
	Point2f center;
	vector<Point2f> corners;
	CvRect subArea;
	float perimeter;

	//detect marker in an image
	static bool detect(const Mat &image, const int markerNumber, vector<Marker> &outMarkers,
		const MarkerParameters &criteria);


	//tracking marker position in consequence frames
	static bool track(const Mat &image, const int markerNumber, vector<Marker> &inoutMarkers,
		const MarkerParameters &perimeter);

	//create marker image, id >=0 && id < 1024
	static cv::Mat createMarkerImage(int id, int size);



private:
	void getPerimeter(void);


	
};
