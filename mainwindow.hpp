#pragma once
#include <QMainWindow>
#include <QObject>
#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtGui/QFont>
#include <QWidget>

using namespace QtDataVisualization;

class MainWindow : public QWidget {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

	void TestButton();

	void AddData();
	void AddDataPoint(float x, float y, float z);
	void ToggleItemCount();


private:

	QScatterDataArray *dataArray ;
	QScatterDataItem *ptrToDataArray;
	Q3DScatter *m_graph;
	int m_fontSize;
	QAbstract3DSeries::Mesh m_style;
	bool m_smooth;
	int m_itemCount;
	float m_curveDivider;

Q_SIGNALS:

	
};
