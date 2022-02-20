#include "MainWindow.h"
#include<QString>

//���������������
#if _MSC_VER >= 1600 //�汾�Ŵ���1600��
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	mdiArea = new QMdiArea;
	setCentralWidget(mdiArea);
	initUI();
	craetActions();

	win_id = 0; 
	resize(800, 600);
	
	crateOpenGl_GraphicDriver();
}

void MainWindow::initUI()
{
	this->setWindowTitle("KAX_CAD");
}

MDIWindow * MainWindow::activeMDIWindow() const
{
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
		return qobject_cast<MDIWindow *>(activeSubWindow->widget());
	return 0;
}

void MainWindow::craetActions()
{
	//�½���Ŀ
	newProgress = new QAction(QString("�½���Ŀ"), this);
	QIcon icon;
	icon.addFile(":/MainWindow/images/new.png");
	newProgress->setIcon(icon);
	ui.start->addAction(newProgress);
	ui.mainToolBar->addAction(newProgress);
	connect(newProgress, SIGNAL(triggered()), this, SLOT(newWindow()));

	//��������Ŀ
	openProgress = new QAction("��������Ŀ", this);
	QIcon icon2;
	icon2.addFile(":/MainWindow/images/open.png");
	openProgress->setIcon(icon2);
	ui.start->addAction(openProgress);
	ui.mainToolBar->addAction(openProgress);

	//�½�����
	createDemo = new QAction(QString("����demo"), this);
	ui.file->addAction(createDemo);
	ui.mainToolBar->addAction(createDemo);
	connect(createDemo, SIGNAL(triggered()), this, SLOT(createBottle()));

	//��ͼ���Ļ�
	viewCentel = new QAction(QString("��ͼ���Ļ�"), this);
	ui.start->addAction(viewCentel);
	ui.mainToolBar->addAction(viewCentel);
	connect(viewCentel, SIGNAL(triggered()), this, SLOT(ViewCentel()));
	
}

void MainWindow::createBottle()
{
	demo = now_mdi->myView->create_bottle(10, 10, 10);
	Handle(AIS_Shape) aisDemo = new AIS_Shape(demo);
	now_mdi = activeMDIWindow();
	now_mdi->myView->get_H_AISContext()->Display(aisDemo,Standard_True);
	now_mdi->myView->get_view()->FitAll();
}

void MainWindow::ViewCentel()
{
	now_mdi = activeMDIWindow();
	now_mdi->myView->fitALL();
}

void MainWindow::newWindow()
{
	MDIWindow* mdi = new MDIWindow(mdiArea->size(), driver, this);

	mdis[mdi->myView->getWId()] = mdi;
	win_id++;
	now_mdi = mdi;

	mdiArea->addSubWindow(now_mdi);
	now_mdi->show();
	now_mdi->setFocus();

}

Handle(OpenGl_GraphicDriver) MainWindow::getOpenGl_GraphicDriver()
{
	return driver;
}

void MainWindow::crateOpenGl_GraphicDriver()
{
	const Handle(Aspect_DisplayConnection) theDisp = new Aspect_DisplayConnection();
	driver = new OpenGl_GraphicDriver(theDisp);
}