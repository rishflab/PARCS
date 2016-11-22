/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_helloClass
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *helloClass)
    {
        if (helloClass->objectName().isEmpty())
            helloClass->setObjectName(QStringLiteral("helloClass"));
        helloClass->resize(600, 400);
        centralWidget = new QWidget(helloClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(60, 80, 160, 141));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(240, 170, 70, 17));
        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(260, 240, 70, 17));
        helloClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(helloClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        helloClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(helloClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        helloClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(helloClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        helloClass->setStatusBar(statusBar);

        retranslateUi(helloClass);

        QMetaObject::connectSlotsByName(helloClass);
    } // setupUi

    void retranslateUi(QMainWindow *helloClass)
    {
        helloClass->setWindowTitle(QApplication::translate("helloClass", "hello", 0));
        checkBox->setText(QApplication::translate("helloClass", "CheckBox", 0));
        checkBox_2->setText(QApplication::translate("helloClass", "CheckBox", 0));
    } // retranslateUi

};

namespace Ui {
    class helloClass: public Ui_helloClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
