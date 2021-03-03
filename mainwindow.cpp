#include "mainwindow.hpp"
#include <QtCore/qmath.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>
#include <QtCore/QDebug>

using namespace QtDataVisualization;

const int numberOfItems = 3600;
const float curveDivider = 3.0f;
const int lowerNumberOfItems = 900;
const float lowerCurveDivider = 0.75f;

MainWindow::MainWindow(QWidget *parent): //:
	QWidget(parent)//,
{
	m_graph = new QtDataVisualization::Q3DScatter();
	QWidget *container = QWidget::createWindowContainer(m_graph);

	
	QSize screenSize = m_graph->screen()->size();
	container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
	container->setMaximumSize(screenSize);
	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	container->setFocusPolicy(Qt::StrongFocus);
	




	//QWidget *widget = new QWidget;
	QHBoxLayout *hLayout = new QHBoxLayout(this);
	QVBoxLayout *vLayout = new QVBoxLayout();
	hLayout->addWidget(container, 1);
	hLayout->addLayout(vLayout);
	this->setWindowTitle(QStringLiteral("qwdqd"));


	QPushButton *labelButton = new QPushButton(this);
	labelButton->setText(QStringLiteral("Change Label Style"));
	vLayout->addWidget(labelButton, 0, Qt::AlignTop);

	m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
	QFont font = m_graph->activeTheme()->font();
	font.setPointSize(m_fontSize);
	m_graph->activeTheme()->setFont(font);
	m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
	m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

	QScatterDataProxy *proxy = new QScatterDataProxy;
	QScatter3DSeries *series = new QScatter3DSeries(proxy);
	series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
	series->setMeshSmooth(m_smooth);
	m_graph->addSeries(series);

	QObject::connect(labelButton, &QPushButton::clicked, this, &MainWindow::TestButton);

	



	ToggleItemCount();

}

void MainWindow::TestButton() {
	qDebug() << "label button pressed";
}

MainWindow::~MainWindow() {
	//delete ui;
}

void MainWindow::AddDataPoint(float x, float y, float z)
{

	ptrToDataArray->setPosition(QVector3D(x, y, z));
	ptrToDataArray++;


	m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
}


void MainWindow::ToggleItemCount()
{
	if (m_itemCount == numberOfItems) {
		m_itemCount = lowerNumberOfItems;
		m_curveDivider = lowerCurveDivider;
	}
	else {
		m_itemCount = numberOfItems;
		m_curveDivider = curveDivider;
	}

	dataArray = new QScatterDataArray;

	

	dataArray->resize(m_itemCount);

	ptrToDataArray = &dataArray->first();


	m_graph->axisX()->setTitle("X");
	m_graph->axisY()->setTitle("Y");
	m_graph->axisZ()->setTitle("Z");
	m_graph->axisX()->setRange(-2000.0f, 2000.0f);
	m_graph->axisY()->setRange(-2000.0f, 2000.0f);
	m_graph->axisZ()->setRange(0.0f, 4000.0f);

	//m_graph->seriesList().at(0)->dataProxy()->resetArray(0);
	//AddData(1.0f, 1.0f, 1.0f);
	//AddData(2.0f, 3.0f, 4.0f);

	//QValue3DAxis xAxis, yAxis, zAxis;
	//xAxis.autoAdjustRangeChanged(false);
	//yAxis.autoAdjustRangeChanged(false);
	//zAxis.autoAdjustRangeChanged(false);
	//xAxis.setRange(-5000.0f, 5000.0f);
	//yAxis.setRange(-5000.0f, 5000.0f);
	//zAxis.setRange(-5000.0f, 5000.0f);
	//m_graph->setAxisX(&xAxis);
	//m_graph->setAxisX(&yAxis);
	//m_graph->setAxisX(&zAxis);

}

//QVector3D MainWindow::RandVector()
//{
//	return QVector3D(
//		(float)(rand() % 100) / 2.0f - (float)(rand() % 100) / 2.0f,
//		(float)(rand() % 100) / 100.0f - (float)(rand() % 100) / 100.0f,
//		(float)(rand() % 100) / 2.0f - (float)(rand() % 100) / 2.0f);
//}
