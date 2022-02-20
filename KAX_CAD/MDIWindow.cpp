#include "MDIWindow.h"

MDIWindow::MDIWindow(QSize _mdiAreaSize, Handle(OpenGl_GraphicDriver) driver, QMainWindow* parent = nullptr)
	:QMainWindow(parent)
{
	mdiAreaSize = _mdiAreaSize;

	QFrame* vb = new QFrame(this);
	QVBoxLayout* layout = new QVBoxLayout(vb);
	layout->setMargin(0);
	vb->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
	setCentralWidget(vb);

	myView = new OCCView(this, driver);
	layout->addWidget(myView);

	//setMinimumSize(QSize(400, 300));
	resize(sizeHint());
	setFocusPolicy(Qt::StrongFocus);
}

QSize MDIWindow::sizeHint() const
{
	return QSize(400,300);//mdiAreaSize;
}

void MDIWindow::changeEvent(QEvent * event)
{
	myView->get_view()->MustBeResized();//更改窗口大小必须使用
}
