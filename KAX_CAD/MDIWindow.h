#pragma once
#include <QtWidgets>
#include "OCCView.h"

#include <QLayout>
class MDIWindow :
	public QMainWindow
{
	Q_OBJECT

private:
	QSize mdiAreaSize;
public:
	MDIWindow(QSize _mdiAreaSize, Handle(OpenGl_GraphicDriver) driver, QMainWindow *parent);

	//OCCView* myView;
	OCCView* myView;

	//重载QMainWindow中的szieHint()才能使resize()有效，不知为啥？
	virtual QSize              sizeHint() const;

protected:
	//当点击子窗口最大化按钮时触发，解决直接点击最大化按钮，视图view只能部分使用的问题
	void changeEvent(QEvent * event);
};