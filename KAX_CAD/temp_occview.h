#ifndef OCCVIEW_H
#define OCCVIEW_H
//#include<QGLWidget>
#include <QWidget>
#include<QDebug>
#include<QVector>
#include<AIS_InteractiveContext.hxx>
#include<OpenGl_GraphicDriver.hxx>
#include<V3d_View.hxx>
#include<V3d_Viewer.hxx>
#include<WNT_Window.hxx>
#include<Quantity_NameOfColor.hxx>
#include<gp_Pnt2d.hxx>
//#include"makebottledialog.h"
#include<QpointF>
#include<TopoDS_Shape.hxx>
#include<ProjLib.hxx>
#include<ElSLib.hxx>
#include <AIS_Manipulator.hxx>
#include<TopoDS_Face.hxx>
#include<BRepBuilderAPI_MakeFace.hxx>
#include<AIS_Shape.hxx>
namespace Ui {
class OccView;
}
enum drawtype{
  LINE,
 CIRCLE,
    POLYGON

};
class OccView : public QWidget
{
    Q_OBJECT

public:
    //explicit 
	OccView(QWidget *parent = 0);
    ~OccView();
 Handle(V3d_View) m_view;
 //drawtype m_current_type;
private:
    //Ui::OccView *ui;
private:
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer) m_viewer;
    Handle(Graphic3d_GraphicDriver) m_graphic_driver;
public:
    QPaintEngine *paintEngine() const;
    //Handle(AIS_InteractiveContext) GetInteractiveContext();
    //Handle(V3d_View) GetView();
    //TopoDS_Shape getshape(const Handle(AIS_InteractiveContext) context);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    //void wheelEvent(QWheelEvent *event);
    //void mousePressEvent(QMouseEvent *event);

protected:
    enum CurrentAction{
        CurrentAction_Nothing,//
        CurrentAction_Panning,//平移，按住鼠标左键和右键并拖动进行平移操作
        CurrentAction_Zooming,//缩放，滚动滚轮进行缩放
        CurrentAction_Rotation//旋转，按住鼠标中间并拖动进行旋转
    };

private:
    Standard_Integer m_x;
    Standard_Integer m_y;
    CurrentAction m_curremt_mode;
    Standard_Integer anXPix;
     Standard_Integer anYPix;
signals:
    //void drawline(gp_Pnt p1,gp_Pnt p2);
    //void drawcircle(gp_Pnt p1,gp_Pnt p2);
    //void drawPolygon(QVector<gp_Pnt>vector);
    //void selectshape(TopoDS_Shape shape);
private:
    int a=0;
    gp_Pnt p1;
    gp_Pnt p2;
    //gp_Pnt ConvertClickToPoint(Standard_Real x, Standard_Real y, Handle(V3d_View) aView);
    TopoDS_Shape myObeject;
    QVector<TopoDS_Shape> m_shapeArray;
    bool polygon_end=false;//判断绘制结束的标志，绘制结束才会发送信号
    QVector<gp_Pnt> pntArry;
};

#endif // OCCVIEW_H
