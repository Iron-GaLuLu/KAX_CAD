#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "MDIWindow.h"
#include <vector>

#include "temp_occview.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	Handle(OpenGl_GraphicDriver) getOpenGl_GraphicDriver();

private:
	QMdiArea *mdiArea;

	int win_id;
	std::map<WId,MDIWindow*> mdis;
	MDIWindow* now_mdi;

    Ui::MainWindowClass ui;

	QAction *newProgress;	
	QAction *openProgress;
	QAction *newProfile;
	QAction *openProfile;
	QAction *createDemo;
	QAction *viewCentel;

	TopoDS_Shape demo;

	Handle(OpenGl_GraphicDriver) driver;
	void crateOpenGl_GraphicDriver();
	
	void initUI();

	MDIWindow* activeMDIWindow() const;//返回当前的活动窗口

private slots:
	void newWindow();
	void craetActions();
	void createBottle();
	void ViewCentel();

protected:
	//void mousePressEvent(QMouseEvent* event);
	
	
};
