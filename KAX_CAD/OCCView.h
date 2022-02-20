#pragma once
#include <QtWidgets>
#include <WNT_Window.hxx>

#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <Geom_Plane.hxx>
#include <BRepLib.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <StdSelect_ShapeTypeFilter.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <AIS_ViewCube.hxx>


class OCCView
	: public QWidget
{
	Q_OBJECT

public:
	OCCView(QWidget *parent, Handle(OpenGl_GraphicDriver) driver);
	
public:
	
	Handle(OpenGl_GraphicDriver) getOpenGl_GraphicDriver();
	//QWideget 继承自QPaintEngine paintEngine()是纯虚函数，要想实现自定义的绘制功能必须实现paintEngine()
	virtual QPaintEngine *paintEngine() const;

	TopoDS_Shape create_bottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);
	Handle(AIS_InteractiveContext) get_H_AISContext();
	Handle(V3d_View) get_view();
	void fitALL();

	WId getWId();

	void makeEdge(gp_Pnt &Pnt1, gp_Pnt &Pnt2, Quantity_NameOfColor &Color1);

private:
	
	Handle(V3d_Viewer) H_viewer;
	Handle(AIS_InteractiveContext) H_AISContext;
	Handle(V3d_View) H_view;
	Handle(V3d_View) view;
	WId window_handle;

	void CrateReferencePlane();

protected:
	enum CurrenAction {
		CurrentAction_Nothing,
		CurrentAction_Panning,//平移
		CurrentAction_Zooming,//缩放
		CurrentAction_Rotation//旋转
	};
	CurrenAction nowMode;
	Standard_Integer now_x;
	Standard_Integer now_y;

	void paintEvent(QPaintEvent *event);//第一次绘制和更改窗口位置后会触发此事件
	void resizeEvent(QResizeEvent *event);//更改窗口大小时会触发，paintEvent也实现了同样的功能，注释后也不影响

	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseWhellEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private slots:
	void showAll(void);
};

