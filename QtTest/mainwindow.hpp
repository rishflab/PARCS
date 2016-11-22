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


	//void AddChangeStyle(int);
	//void SetSmoothDots(int smooth);
	//void ChangePresetCamera();
	//void ChangeLabelStyle();
	//void ChangeFont(const QFont &font);
	//void ShadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq);
	//void ChangeShadowQuality(int quality);
	//void SetBackgroundEnabled(int enabled);
	//void SetGridEnabled(int enabled);


private:

	QScatterDataArray *dataArray ;
	QScatterDataItem *ptrToDataArray;
	//QVector3D RandVector();
	Q3DScatter *m_graph;
	int m_fontSize;
	QAbstract3DSeries::Mesh m_style;
	bool m_smooth;
	int m_itemCount;
	float m_curveDivider;


	//public Q_SLOTS:
	//void ChangeStyle(int style);
	//void ChangeTheme(int theme);
	//void changeShadowQuality(int quality);
	//void shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality shadowQuality);

Q_SIGNALS:
	//void backgroundEnabledChanged(bool enabled);
	//void gridEnabledChanged(bool enabled);
	//void shadowQualityChanged(int quality);
	//void fontChanged(QFont font);
	
};
