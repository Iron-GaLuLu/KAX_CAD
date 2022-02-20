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

	//����QMainWindow�е�szieHint()����ʹresize()��Ч����֪Ϊɶ��
	virtual QSize              sizeHint() const;

protected:
	//������Ӵ�����󻯰�ťʱ���������ֱ�ӵ����󻯰�ť����ͼviewֻ�ܲ���ʹ�õ�����
	void changeEvent(QEvent * event);
};