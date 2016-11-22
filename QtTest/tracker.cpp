#include "tracker.hpp"

Tracker::Tracker(QObject * parent) : QObject(parent) {
	
}

Tracker::~Tracker() {
	
}

#define HORZ_RES 640
#define VERT_RES 480
#define TRACK_COEF 1

using namespace std;
using namespace arma;
using namespace cv;

/*a1,a2,a3 are camera offset params*/
Tracker::Tracker(float cx, float cy, float w, float h, float _a1, float _a2, float _a3,
	float dpan, float dtilt) {
	this->rect = ComputeBounds(cx, cy, w, h);
	this->a1 = _a1;
	this->a2 = _a2;
	this->a3 = _a3;
	this->deltapan = dpan;
	this->deltatilt = dtilt;
}

//takes in image centre computes bounds rect based on width and height 
Rect2f Tracker::ComputeBounds(float cx, float cy, float width, float height) {

	Rect2f _rect;
	_rect.x = cx - width / 2;
	_rect.y = cy - width / 2;
	_rect.width = width;
	_rect.height = height;
	this->rect = _rect;
	return _rect;
}

//checks in corners found are inside bounding rect
bool Tracker::CheckCornerInBounds(vector<Point2f> corners, Rect2f _rect) {

	float sumx = 0;
	float sumy = 0;

	for (int i = 1; i < corners.size(); i++) {
		sumx += corners.at(i).x;
		sumy += corners.at(i).y;
	}

	float xmean = mean(sumx);
	float ymean = mean(sumy);


	if (xmean < _rect.x || xmean > _rect.x + _rect.width ||
		ymean < _rect.y || ymean > _rect.y + _rect.height) {
		return false;
	}

	return true;
}

Point3f Tracker::ComputeTargetCenter(vector<cv::Mat> objpoints) {
	float sumx = 0;
	float sumy = 0;
	float sumz = 0;

	for (int i = 0; i < objpoints.size(); i++) {

		sumx += objpoints.at(i).at<double>(0, 0);
		sumy += objpoints.at(i).at<double>(1, 0);
		sumz += objpoints.at(i).at<double>(2, 0);
	}

	float xmean = sumx / objpoints.size();
	float ymean = sumy / objpoints.size();
	float zmean = sumz / objpoints.size();


	return Point3f(xmean, ymean, zmean);
}

//predicts new world position based on latest world position
Point3f Tracker::PredictLocationVelocity() {
	Point3f newLoc;

	return newLoc;

}

bool Tracker::MovetoPanTilt(int _pan, int _tilt, Serial serial) {

	char* panstr;
	char* tiltstr;

	//char msg[] = "s06120512\0";

	_itoa(_pan, panstr, 10);

	_itoa(_tilt, tiltstr, 10);

	//string msg2 = "s0" + panstr + "0" + tiltstr + "\0";

	char* msg = new char[strlen("s0") + strlen(panstr) + strlen("0") + strlen(tiltstr) + strlen("\0")];

	sprintf(msg, "%s%s%s%s%s", "s0", panstr, "0", tiltstr, "\0");

	return serial.WriteToCom(msg);
}

void Tracker::PredictLocationEdge(Point2f target, float newpan, float newtilt, float pan, float tilt) {

	float xmax = this->rect.x + this->rect.width;
	float xmin = this->rect.x;
	float ymin = this->rect.y;
	float ymax = this->rect.y + this->rect.height;

	Point3f newLoc;

	if (target.x > xmax) {
		if (target.y < ymin) {
			//upright
			newpan = this->deltapan + pan;
			newtilt = this->deltatilt + tilt;
		}
		else if (target.y > ymax) {
			//downright
			newpan = this->deltapan + pan;
			newtilt = this->deltatilt + tilt;
		}
		else {
			//right
			newpan = this->deltapan + pan;
			newtilt = this->deltatilt + tilt;
		}
	}

	else if (target.x < xmin) {
		if (target.y < ymin) {
			//upleft
			newpan = this->deltapan + pan;
			newtilt = this->deltatilt + tilt;
		}
		else if (target.y > ymax) {
			//downleft
			newpan = this->deltapan + pan;
			newtilt = this->deltatilt + tilt;
		}
		else {
			//left
			newpan = this->deltapan + pan;
			newtilt = this->deltatilt + tilt;
		}
	}

	//return newLoc;
}

bool Tracker::MoveToLocation(float x, float y, float z) {


	return true;
}






ikvals Tracker::IkCalc(float x, float y, float z) {

	ikvals ret;
	ret.d1 = sqrt(x*x + y*y - this->a1*this->a1);
	ret.pan = atan2(y, x) - atan2(this->a1, ret.d1);
	ret.d = sqrt(ret.d1*ret.d1 + z*z);
	ret.d2 = sqrt(ret.d*ret.d - this->a2*this->a2);
	ret.tilt = atan2(z, ret.d1) - atan2(this->a2, ret.d2);
	ret.l = ret.d2 - a3;
	return ret;
}

mat Tracker::Calculate3DPosFK(ikvals ik, float x, float y, float z) {


	mat r0 = MakeZRotationMatrix(ik.pan);
	//cout << "r0" << endl << r0 << endl;

	mat t1 = MakeTranslateMatrix(0, this->a1, 0);
	//cout << "t1" << endl << t1 << endl;

	mat r3 = MakeYRotationMatrix(-ik.tilt);
	//cout << "r3" << endl << r3 << endl;

	mat t3 = MakeTranslateMatrix(0.0f, 0.0f, this->a2);
	//cout << "t3" << endl << t3 << endl;

	mat r4 = MakeZRotationMatrix(((-datum::pi) / 2.0f));
	//cout << "r4" << endl << r4 << endl;

	mat t4 = MakeTranslateMatrix(0.0f, ik.d2, 0);
	//cout << "t4" << endl << t4 << endl;
	

	mat p0;

	p0 << 0 << endr
		<< 0 << endr
		<< 0 << endr
		<< 1 << endr;

	mat p1 = r0* t1 * p0;
	//cout << "p1" << endl << p1 << endl;

	mat p3 = r0 * r3 * t3 * p0;
	//cout << "p3" << endl << p3 + p1 << endl;

	mat p4 = r0* r3* r4 * t4 * p0;
	cout << "p4" << endl << p4 + p3 + p1 << endl;

	//calculation for laser
	//return (p4 + p3 + p1);

	//calculation for camera
	return (p3 + p1);
}



bool Tracker::SimpleTrack(cv::Point2i oldPos, cv::Point2i newPos) {

	int deltaX = oldPos.x - (HORZ_RES / 2);
	int deltaY = oldPos.y - (VERT_RES / 2);


	newPos.x = TRACK_COEF * deltaX;
	newPos.y = TRACK_COEF * deltaY;

	return true;
}

mat Tracker::MakeTranslateMatrix(float x, float y, float z) {

	mat r;

	r << 1.0f << 0.0f << 0.0f << x << endr
		<< 0.0f << 1.0f << 0.0f << y << endr
		<< 0.0f << 0.0f << 1.0f << z << endr
		<< 0.0f << 0.0f << 0.0f << 1.0f << endr;

	return r;
}

mat Tracker::MakeXRotationMatrix(float rx) {

	mat r;

	r << 1.0f << 0.0f << 0.0f << 0.0f << endr
		<< 0.0f << cos(rx) << -sin(rx) << 0.0f << endr
		<< 0.0f << sin(rx) << cos(rx) << 0.0f << endr
		<< 0.0f << 0.0f << 0.0f << 1.0f << endr;


	return r;
}

mat Tracker::MakeYRotationMatrix(float ry) {

	mat r;

	r << cos(ry) << 0.0f << sin(ry) << 0.0f << endr
		<< 0.0f << 1.0f << 0.0f << 0.0f << endr
		<< -sin(ry) << 0.0f << cos(ry) << 0.0f << endr
		<< 0.0f << 0.0f << 0.0f << 1.0f << endr;


	return r;
}



mat Tracker::MakeZRotationMatrix(float rz) {



	mat r;

	r << cos(rz) << -sin(rz) << 0.0f << 0.0f << endr
		<< sin(rz) << cos(rz) << 0.0f << 0.0f << endr
		<< 0.0f << 0.0f << 1.0f << 0.0f << endr
		<< 0.0f << 0.0f << 0.0f << 1.0f << endr;


	return r;
}