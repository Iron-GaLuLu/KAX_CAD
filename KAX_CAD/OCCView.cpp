#include "OCCView.h"
#include <TCollection_AsciiString.hxx>
#include <iostream>
using namespace std;
OCCView::OCCView(QWidget *parent, Handle(OpenGl_GraphicDriver) driver)
{
	//OCC���helloworld
	TCollection_AsciiString asHelloWorld("Hello World!");
	TCollection_AsciiString asHelloOCC("Hello Open CASCADE!");
	cout << asHelloWorld << endl;
    cout << asHelloOCC << endl;

	H_viewer = new V3d_Viewer(driver);//�������洴����ͼ����
	H_AISContext = new AIS_InteractiveContext(H_viewer);//������ͼ���󴴽�����ʽ������
	view = new V3d_View(H_viewer);// ������ͼ���󴴽���ͼ

	window_handle = (WId)winId();//��ȡQWidget�Ĵ���ϵͳ��ʶ��
	std::cout << window_handle << std::endl;
	Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)window_handle);// ����Windows NT ����
	view->SetWindow(wind);//������ͼ��wind

	//���ò鿴���Ĺ���
	H_viewer->SetDefaultLights();
	//�򿪱����ƹ�
	H_viewer->SetLightOn();
	//������ͼ�ı�����ɫΪ��ɫ
	view->SetBackgroundColor(Quantity_NOC_BLACK);
	//���Ĵ��ڴ�Сʱ����ʹ��
	view->MustBeResized();

	//��ʾֱ������ϵ�����������ڴ�����ʾλ�á�������ɫ����С����ʽ
	//view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

	//���ý���ʽ�����ĵ���ʾģʽ
	H_AISContext->SetDisplayMode(AIS_Shaded, Standard_True);

	Handle(StdSelect_ShapeTypeFilter) solidFilter = new  StdSelect_ShapeTypeFilter(TopAbs_SOLID);
	H_AISContext->AddFilter(solidFilter);//��SOLIDѡ������Ϊ����������Բ�������
	Handle(StdSelect_ShapeTypeFilter) faceFilter = new  StdSelect_ShapeTypeFilter(TopAbs_FACE);
	H_AISContext->AddFilter(faceFilter);
	Handle(StdSelect_ShapeTypeFilter) edgeFilter = new  StdSelect_ShapeTypeFilter(TopAbs_EDGE);
	H_AISContext->AddFilter(edgeFilter);
	Handle(StdSelect_ShapeTypeFilter) vertexFilter = new  StdSelect_ShapeTypeFilter(TopAbs_VERTEX);
	H_AISContext->AddFilter(vertexFilter);
	Handle(StdSelect_ShapeTypeFilter) shellFilter = new  StdSelect_ShapeTypeFilter(TopAbs_SHELL);
	H_AISContext->AddFilter(shellFilter);
	Handle(StdSelect_ShapeTypeFilter) wireFilter = new  StdSelect_ShapeTypeFilter(TopAbs_WIRE);
	H_AISContext->AddFilter(wireFilter);

	//����ģ�͸������
	Handle(Prs3d_Drawer) t_hilight_style = H_AISContext->HighlightStyle();
	t_hilight_style->SetMethod(Aspect_TOHM_COLOR);  // ��ɫ��ʾ��ʽ
	t_hilight_style->SetColor(Quantity_NOC_LIGHTYELLOW);    // ���ø�����ɫ
	t_hilight_style->SetDisplayMode(1); // �������
	t_hilight_style->SetTransparency(0.2f); // ����͸����

	// ����ѡ��ģ�͵ķ��
	Handle(Prs3d_Drawer) t_select_style = H_AISContext->SelectionStyle();  // ��ȡѡ����
	t_select_style->SetMethod(Aspect_TOHM_COLOR);  // ��ɫ��ʾ��ʽ
	t_select_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);   // ����ѡ�����ɫ
	t_select_style->SetDisplayMode(1); // �������
	t_select_style->SetTransparency(0.4f); // ����͸����

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setBackgroundRole(QPalette::NoRole);
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);//����������

	CrateReferencePlane();
}


QPaintEngine *OCCView::paintEngine() const
{
	return 0;
}

void OCCView::paintEvent(QPaintEvent *)
{
	//std::cout << "firstChange" << "\n";
	view->Redraw();//��ʾ��ͼ����ʹ��redraw
}

void OCCView::mousePressEvent(QMouseEvent * event)
{
	//���Ҽ�������ƽ�ƣ�event->buttons()��ǰ��갴���ˣ����ɿ���ȡ�����жϣ�������һֱƽ��״̬
	if (event->buttons()&Qt::LeftButton && event->buttons()&Qt::RightButton) {
		//std::cout << "press_left_ringht" << "\n";
		nowMode = CurrentAction_Panning;
		now_x = event->pos().x();
		now_y = event->pos().y();
	}
	//����м�����������ƶ�������ת
	else if (event->buttons()&Qt::LeftButton && event->buttons()&Qt::MidButton) {
		//std::cout << "press_whell" << "\n";
		nowMode = CurrentAction_Rotation;

		//����StartRoation����תʱ�ᷢ������
		view->StartRotation(event->pos().x(), event->pos().y());
	}
	//��������м������ƶ���������
	else if (event->button()&Qt::MidButton) {
		//std::cout << "press_whell" << "\n";
		nowMode = CurrentAction_Zooming;
		now_x = event->pos().x();
		now_y = event->pos().y();
	}
	
	//���������±�ʾѡ��ͼ��
	else if (event->buttons()&Qt::LeftButton) {
		//std::cout << "press_left" << "\n";
		//�����λ�ô��ݵ���������
		H_AISContext->MoveTo(event->pos().x(), event->pos().y(), view, Standard_True);
		AIS_StatusOfPick  pickStatus = AIS_SOP_NothingSelected;
		if (qApp->keyboardModifiers() == Qt::ControlModifier) {
			pickStatus = H_AISContext->ShiftSelect(true);
		}
		else {
			pickStatus = H_AISContext->Select(true);
		}
	}
	else if (event->buttons()&Qt::RightButton) {

	}
}

void OCCView::mouseMoveEvent(QMouseEvent * event)
{
	//std::cout << nowMode << "\n";
	if (nowMode == CurrentAction_Panning) {
		//std::cout << "panning" << "\n";
		view->Pan(event->pos().x() - now_x, now_y - event->pos().y());
		now_x = event->pos().x();
		now_y = event->pos().y();
	}
	else if (nowMode == CurrentAction_Rotation) {
		//std::cout << "rotation" << "\n";
		view->Rotation(event->pos().x(), event->pos().y());
		//std::cout << "(" << event->pos().x() << "," << event->pos().y() << ")\n";
		//Redraw���Ӻ���Ҳ����
		view->Redraw();
	}
	else if (nowMode == CurrentAction_Zooming) {
		//std::cout << "zooming" << "\n";
		view->Zoom(now_x, now_y, event->pos().x(), event->pos().y());
		//std::cout << "now��(" << now_x << ',' << now_y << ")\n"
		//	<< "event��(" << event->pos().x() << ',' << event->pos().y() << ")\n";
		now_x = event->pos().x();
		now_y = event->pos().y();
		
	}
	//�����λ�ô��ݵ���������
	H_AISContext->MoveTo(event->pos().x(), event->pos().y(), view, Standard_True);
}

void OCCView::mouseWhellEvent(QMouseEvent * event)
{
	//std::cout << "whell" << "\n";
	view->StartZoomAtPoint(event->pos().x(), event->pos().y());
	view->ZoomAtPoint(0, 0, event->pos().x(), event->pos().y());
}

void OCCView::mouseReleaseEvent(QMouseEvent * event)
{
	//std::cout << "release" << "\n";
	nowMode = CurrentAction_Nothing;
}

TopoDS_Shape OCCView::create_bottle(const Standard_Real myWidth = 10, const Standard_Real myHeight = 10, const Standard_Real myThickness = 10)
{
	// ����������֧�ֵ�
	gp_Pnt aPnt1(-myWidth / 2., 0, 0);
	gp_Pnt aPnt2(-myWidth / 2., -myThickness / 4., 0);
	gp_Pnt aPnt3(0, -myThickness / 2., 0);
	gp_Pnt aPnt4(myWidth / 2., -myThickness / 4., 0);
	gp_Pnt aPnt5(myWidth / 2., 0, 0);
	// ���������弸��ͼ��
	Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2, aPnt3, aPnt4);
	Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
	Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt4, aPnt5);
	// �������������˽ṹ
	TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
	TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
	TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);
	TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);
	// �������������
	gp_Ax1 xAxis = gp::OX();    // ���X��
	gp_Trsf aTrsf;
	aTrsf.SetMirror(xAxis);
	BRepBuilderAPI_Transform aBRepTrsf(aWire, aTrsf);
	TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
	TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);
	BRepBuilderAPI_MakeWire mkWire;
	mkWire.Add(aWire);
	mkWire.Add(aMirroredWire);
	TopoDS_Wire myWireProfile = mkWire.Wire();
	// ƿ����������
	TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);
	gp_Vec aPrismVec(0, 0, myHeight);
	TopoDS_Shape myBody = BRepPrimAPI_MakePrism(myFaceProfile, aPrismVec);
	// ƿ������
	BRepFilletAPI_MakeFillet mkFillet(myBody);
	TopExp_Explorer anEdgeExplorer(myBody, TopAbs_EDGE);
	while (anEdgeExplorer.More()) {
		TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
		//Add edge to fillet algorithm
		mkFillet.Add(myThickness / 12., anEdge);
		anEdgeExplorer.Next();
	}
	myBody = mkFillet.Shape();
	// ƿ�����ƿ��
	gp_Pnt neckLocation(0, 0, myHeight);
	gp_Dir neckAxis = gp::DZ();
	gp_Ax2 neckAx2(neckLocation, neckAxis);
	Standard_Real myNeckRadius = myThickness / 4.;
	Standard_Real myNeckHeight = myHeight / 10.;
	BRepPrimAPI_MakeCylinder MKCylinder(neckAx2, myNeckRadius, myNeckHeight);
	TopoDS_Shape myNeck = MKCylinder.Shape();
	myBody = BRepAlgoAPI_Fuse(myBody, myNeck);
	// ƿ�������пյ�ʵ��
	TopoDS_Face   faceToRemove;
	Standard_Real zMax = -1;
	for (TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE); aFaceExplorer.More(); aFaceExplorer.Next()) {
		TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
		// Check if <aFace> is the top face of the bottle's neck
		Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
		if (aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
			Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
			gp_Pnt aPnt = aPlane->Location();
			Standard_Real aZ = aPnt.Z();
			if (aZ > zMax) {
				zMax = aZ;
				faceToRemove = aFace;
			}
		}
	}
	TopTools_ListOfShape facesToRemove;
	facesToRemove.Append(faceToRemove);
	BRepOffsetAPI_MakeThickSolid BodyMaker;
	BodyMaker.MakeThickSolidByJoin(myBody, facesToRemove, -myThickness / 50, 1.e-3);
	myBody = BodyMaker.Shape();
	// ���ƣ���������
	Handle(Geom_CylindricalSurface) aCyl1 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 0.99);
	Handle(Geom_CylindricalSurface) aCyl2 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 1.05);
	// ���ƣ������ά����
	gp_Pnt2d aPnt(2. * M_PI, myNeckHeight / 2.);
	gp_Dir2d aDir(2. * M_PI, myNeckHeight / 4.);
	gp_Ax2d anAx2d(aPnt, aDir);
	Standard_Real aMajor = 2. * M_PI;
	Standard_Real aMinor = myNeckHeight / 10;
	Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor);
	Handle(Geom2d_Ellipse) anEllipse2 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor / 4);
	Handle(Geom2d_TrimmedCurve) anArc1 = new Geom2d_TrimmedCurve(anEllipse1, 0, M_PI);
	Handle(Geom2d_TrimmedCurve) anArc2 = new Geom2d_TrimmedCurve(anEllipse2, 0, M_PI);
	gp_Pnt2d anEllipsePnt1 = anEllipse1->Value(0);
	gp_Pnt2d anEllipsePnt2 = anEllipse1->Value(M_PI);
	Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1, anEllipsePnt2);
	// ���ƣ������ߺ���
	TopoDS_Edge anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(anArc1, aCyl1);
	TopoDS_Edge anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment, aCyl1);
	TopoDS_Edge anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(anArc2, aCyl2);
	TopoDS_Edge anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(aSegment, aCyl2);
	TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
	TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);
	BRepLib::BuildCurves3d(threadingWire1);
	BRepLib::BuildCurves3d(threadingWire2);
	// ��������
	BRepOffsetAPI_ThruSections aTool(Standard_True);
	aTool.AddWire(threadingWire1);
	aTool.AddWire(threadingWire2);
	aTool.CheckCompatibility(Standard_False);
	TopoDS_Shape myThreading = aTool.Shape();
	// ����ģ��
	TopoDS_Compound aRes;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound(aRes);
	aBuilder.Add(aRes, myBody);
	aBuilder.Add(aRes, myThreading);
	return aRes;
}

Handle(AIS_InteractiveContext) OCCView::get_H_AISContext()
{
	return H_AISContext;
}

Handle(V3d_View) OCCView::get_view()
{
	return view;
}

void OCCView::fitALL()
{
	view->FitAll();
	view->ZFitAll();
	view->Redraw();
}

WId OCCView::getWId()
{
	//return WId();
	return window_handle;
}

void OCCView::makeEdge(gp_Pnt & Pnt1, gp_Pnt & Pnt2, Quantity_NameOfColor & Color1)
{
	//    gp_Pnt Pnt3 = gp_Pnt(0.0,0.0,10.0);
	//    gp_Pnt Pnt4 = gp_Pnt(0.0,0.0,-10.0);
	TopoDS_Shape aTopoEdge = BRepBuilderAPI_MakeEdge(Pnt1, Pnt2).Shape();
	//TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();
	Handle(AIS_Shape) anAisEdge = new AIS_Shape(aTopoEdge);
	anAisEdge->SetColor(Color1);
	//anAisEdge->SetTransparency(0.1);
	//anAisBox->SetColor(Quantity_NOC_AZURE);

	//myOccView->getContext()->Display(anAisEdge, Standard_True);
	H_AISContext->Display(anAisEdge, 1, -1, Standard_True, AIS_DS_None);

}

void OCCView::CrateReferencePlane()
{
	//create x Axis
	gp_Pnt Pntx0 = gp_Pnt(-30.0, 0.0, 0.0);
	gp_Pnt Pntx1 = gp_Pnt(30.0, 0.0, 0.0);
	Quantity_NameOfColor Color2 = Quantity_NOC_ANTIQUEWHITE3;
	makeEdge(Pntx0, Pntx1, Color2);
	gp_Ax2 anAxisX;
	anAxisX.SetLocation(gp_Pnt(30.0, 0.0, 0.0));
	gp_Dir x = gp_Dir();
	anAxisX.SetDirection(x);
	TopoDS_Shape aTopoConeX = BRepPrimAPI_MakeCone(anAxisX, 1.0, 0.0, 2.0).Shape();//����Բ׶��
	aTopoConeX.Orientation(TopAbs_INTERNAL);//����Բ׶�巽��
	Handle(AIS_Shape) anAisConeX = new AIS_Shape(aTopoConeX);//AIS_SHAPE���ڽ�����shape
	anAisConeX->SetColor(Color2);
	H_AISContext->Display(anAisConeX, 1, -1, Standard_True, AIS_DS_None);

	//create y Axis
	gp_Pnt Pnty0 = gp_Pnt(0.0, -30.0, 0.0);
	gp_Pnt Pnty1 = gp_Pnt(0.0, 30.0, 0.0);
	Quantity_NameOfColor Color3 = Quantity_NOC_ANTIQUEWHITE3;
	makeEdge(Pnty0, Pnty1, Color3);
	gp_Ax2 anAxisY;
	anAxisY.SetLocation(gp_Pnt(0.0, 30.0, 0.0));
	gp_Dir y = gp_Dir(gp::DY());
	anAxisY.SetDirection(y);
	TopoDS_Shape aTopoConeY = BRepPrimAPI_MakeCone(anAxisY, 1.0, 0.0, 2.0).Shape();
	aTopoConeY.Orientation(TopAbs_INTERNAL);
	Handle(AIS_Shape) anAisConeY = new AIS_Shape(aTopoConeY);
	anAisConeY->SetColor(Color3);
	H_AISContext->Display(anAisConeY, 1, -1, Standard_True, AIS_DS_None);

	//create z Axis
	gp_Pnt Pntz0 = gp_Pnt(0.0, 0.0, -30.0);
	gp_Pnt Pntz1 = gp_Pnt(0.0, 0.0, 30.0);
	Quantity_NameOfColor Color4 = Quantity_NOC_ANTIQUEWHITE3;
	makeEdge(Pntz0, Pntz1, Color4);
	gp_Ax2 anAxisZ;
	anAxisZ.SetLocation(gp_Pnt(0.0, 0.0, 30.0));
	gp_Dir z = gp_Dir(gp::DZ());;
	anAxisZ.SetDirection(z);
	TopoDS_Shape aTopoConeZ = BRepPrimAPI_MakeCone(anAxisZ, 1.0, 0.0, 2.0).Shape();
	aTopoConeZ.Orientation(TopAbs_INTERNAL);
	Handle(AIS_Shape) anAisConeZ = new AIS_Shape(aTopoConeZ);
	anAisConeZ->SetColor(Color4);
	H_AISContext->Display(anAisConeZ, 1, -1, Standard_True, AIS_DS_None);

	//Set Face,����߱����,XOY��
	TopoDS_Edge anEdgeXOY = BRepBuilderAPI_MakeEdge(gp_Pnt(-20.0, -20.0, 0.0), gp_Pnt(20.0, -20.0, 0.0));
	TopoDS_Shape aPrismEdgeXOY = BRepPrimAPI_MakePrism(anEdgeXOY, gp_Vec(0.0, 40.0, 0.0));
	Handle(AIS_Shape) anAisPrismEdgeXOY = new AIS_Shape(aPrismEdgeXOY);
	anAisPrismEdgeXOY->SetColor(Quantity_NOC_BLUE1);
	H_AISContext->SetTransparency(anAisPrismEdgeXOY, 0.6, Standard_True);//����͸������
	H_AISContext->Display(anAisPrismEdgeXOY, 1, -1, Standard_True, AIS_DS_None);

	//Set Face,����߱����,XOZ��
	TopoDS_Edge anEdgeXOZ = BRepBuilderAPI_MakeEdge(gp_Pnt(-20.0, 0.0, -20.0), gp_Pnt(20.0, 0.0, -20.0));
	TopoDS_Shape aPrismEdgeXOZ = BRepPrimAPI_MakePrism(anEdgeXOZ, gp_Vec(0.0, 0.0, 40.0));
	Handle(AIS_Shape) anAisPrismEdgeXOZ = new AIS_Shape(aPrismEdgeXOZ);
	anAisPrismEdgeXOZ->SetColor(Quantity_NOC_LIGHTSEAGREEN);
	H_AISContext->SetTransparency(anAisPrismEdgeXOZ, 0.6, Standard_True);
	H_AISContext->Display(anAisPrismEdgeXOZ, 1, -1, Standard_True, AIS_DS_None);

	//Set Face,����߱����,YOZ��
	TopoDS_Edge anEdgeYOZ = BRepBuilderAPI_MakeEdge(gp_Pnt(0.0, -20.0, -20.0), gp_Pnt(0.0, 20.0, -20.0));
	TopoDS_Shape aPrismEdgeYOZ = BRepPrimAPI_MakePrism(anEdgeYOZ, gp_Vec(0.0, 0.0, 40.0));
	Handle(AIS_Shape) anAisPrismEdgeYOZ = new AIS_Shape(aPrismEdgeYOZ);
	anAisPrismEdgeYOZ->SetColor(Quantity_NOC_VIOLET);
	H_AISContext->SetTransparency(anAisPrismEdgeYOZ, 0.6, Standard_True);
	H_AISContext->Display(anAisPrismEdgeYOZ, 1, -1, Standard_True, AIS_DS_None);


	//Set up ViewCube
	Handle(AIS_ViewCube) myViewCube = new AIS_ViewCube();
	myViewCube->SetBoxColor(Quantity_NOC_GRAY75);
	//myViewCube->SetFixedAnimationLoop(false);
	myViewCube->SetSize(55);
	myViewCube->SetFontHeight(12);
	myViewCube->SetAxesLabels("", "", "");
	/*myViewCube->SetTransformPersistence(
		new Graphic3d_TransformPers(
			Graphic3d_TMF_TriedronPers,
			toOccCorner(m_viewTrihedronCorner),
			Graphic3d_Vec2i(85, 85)));*/
			//myContext.addObject(myViewCube);
	H_AISContext->Display(myViewCube, Standard_True);
	//myOccView->getContext()->Display(myViewCube,Standard_True);
	//myOccView->TriedronErase();
	///////

	//fitAll
	fitALL();

	/*Handle(Aspect_Grid) GridXOY = new Aspect_Grid(0.0, 0.0, 45, Quantity_NOC_GRAY50, Quantity_NOC_GRAY70);
	myOccView->getContext()->Display(GridXOY, Standard_True);*/

}

void OCCView::resizeEvent(QResizeEvent *)
{
	if (!view.IsNull())
	{
		view->MustBeResized();//���Ĵ��ڴ�С����ʹ��
	}
}

void OCCView::showAll(void) 
{
	H_AISContext->DisplayAll(Standard_True);
}

