#pragma once
#include <QObject>
#include <QThread>
#include <QDebug>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class MyThread : public QThread {
	Q_OBJECT

public:
	MyThread();
	~MyThread();

	void run();

private:
	
};
