#include "temp_occview.h"
//#include "ui_occview.h"
#include<QMouseEvent>
#include<QWheelEvent>


gp_Pnt p1(10.0,0.0,0.0);
gp_Pnt p2(10.0,10.0,0.0);
gp_Pnt p3(10.0,0.0,10.0);
gp_Vec vec1(p1,p2);
gp_Vec vec2(p1,p3);
gp_Dir dir1(vec1);//单位向量
gp_Dir dir2(vec2);
gp_Ax3 ax3_1(p1,dir1,dir2);//右手坐标系
gp_Pln pln1(ax3_1);
gp_Pln pln2(p1,vec1);//对



OccView::OccView(QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::OccView)
{
    //ui->setupUi(this);
    if(m_context.IsNull())//若交互式上下文为空
    {
        //此对象提供与X server的连接，在Windows和Mac OS中不起作用
        Handle(Aspect_DisplayConnection) m_display_connection=new Aspect_DisplayConnection();
       //创建OpenGl图形驱动
        if(m_graphic_driver.IsNull())
        {
            m_graphic_driver=new OpenGl_GraphicDriver(m_display_connection);
        }
        //获取QWidget的窗口系统标识符
        WId window_handle=(WId)winId();
        // 创建Windows NT 窗口
        Handle(WNT_Window) wind=new WNT_Window((Aspect_Handle)window_handle);
       //创建3D查看器
        m_viewer=new V3d_Viewer(m_graphic_driver);
        //创建视图
        m_view=m_viewer->CreateView();
        m_view->SetWindow(wind);
        //打开窗口
        if(!wind->IsMapped())
        {
            wind->Map();
        }
        //创建交互式上下文
        m_context=new AIS_InteractiveContext(m_viewer);
        //设置查看器的光照
        m_viewer->SetDefaultLights();
        //打开背景灯光
        m_viewer->SetLightOn();
        //设置视图的背景颜色为黑色
        m_view->SetBackgroundColor(Quantity_NOC_BLACK);
        m_view->MustBeResized();;
        //显示直角坐标系，可以配置在窗口显示位置、文字颜色、大小、样式
        m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER,Quantity_NOC_GOLD,0.08,V3d_ZBUFFER);
       //设置交互式上下文的显示模式
        m_context->SetDisplayMode(AIS_Shaded,Standard_True);
        //设置模型高亮风格
        Handle(Prs3d_Drawer) t_hilight_style = m_context->HighlightStyle();
        t_hilight_style->SetMethod(Aspect_TOHM_COLOR);  // 颜色显示方式
        t_hilight_style->SetColor(Quantity_NOC_LIGHTYELLOW);    // 设置高亮颜色
        t_hilight_style->SetDisplayMode(1); // 整体高亮
        t_hilight_style->SetTransparency(0.2f); // 设置透明度

        // 设置选择模型的风格
        Handle(Prs3d_Drawer) t_select_style = m_context->SelectionStyle();  // 获取选择风格
        t_select_style->SetMethod(Aspect_TOHM_COLOR);  // 颜色显示方式
        t_select_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);   // 设置选择后颜色
        t_select_style->SetDisplayMode(1); // 整体高亮
        t_select_style->SetTransparency(0.4f); // 设置透明度
    }
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setBackgroundRole(QPalette::NoRole);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);//设置鼠标跟踪
}

OccView::~OccView()
{
   
}

QPaintEngine *OccView::paintEngine() const
{
	return 0;
}
//
// Handle(AIS_InteractiveContext) OccView::GetInteractiveContext()
//{
//    return m_context;
//
//}
//
// Handle(V3d_View) OccView::GetView()
//{
//    return m_view;
//}
//
void OccView::paintEvent(QPaintEvent *)
{
   m_view->Redraw();
}

void OccView::resizeEvent(QResizeEvent *)
{
   if(!m_view.IsNull())
   {
       m_view->MustBeResized();
   }
}
//
//void OccView::mouseMoveEvent(QMouseEvent *event)
//{
//   /* if (aManipulator->HasActiveMode())
//    {
//        anXPix=event->pos().x();
//        anYPix=event->pos().y();
//        aManipulator->Transform(anXPix, anYPix, m_view); // 应用鼠标从起始位置开始移动而产生的变换
//        m_view->Redraw();
//    }*/
//    switch (m_curremt_mode) {
//    case CurrentAction_Panning:
//        m_view->Pan(event->pos().x()-m_x,event->pos().y()-m_y);
//        m_x=event->pos().x();
//        m_y=event->pos().y();
//        break;
//    case CurrentAction_Rotation:
//        //m_view->StartRotation(event->pos().x(),event->pos().y());移动灵敏性特别高
//       // m_view->Rotate(event->pos().x(),event->pos().y());
//        m_view->Rotation(event->pos().x(),event->pos().y());
//        break;
//    default:
//        break;
//    }
//     m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
//}
//
//void OccView::mouseReleaseEvent(QMouseEvent* )
//{
//    m_curremt_mode=CurrentAction_Nothing;
//   // aManipulator->StopTransform(Standard_True);	// 重置起始变换参数（函数参数为 Standard_False 则撤销本次的变换）
//    //aManipulator->DeactivateCurrentMode();//停止模型操纵器
//   // aManipulator->Detach();//将模型操纵器与模型分离
//}
//
//void OccView::wheelEvent(QWheelEvent *event)
//{
//    m_view->StartZoomAtPoint(event->pos().x(),event->pos().y());
//    m_view->ZoomAtPoint(0,0,event->angleDelta().y(),0);
//}
//
////void OccView::mousePressEvent(QMouseEvent *event)
////{
////    // 激活操纵器
////   /* aManipulator->SetModeActivationOnDetection(Standard_True);
////    m_context->UpdateCurrentViewer();
////    if (aManipulator->HasActiveMode())
////    {
////
////        anXPix=event->pos().x();
////        anYPix=event->pos().y();
////        aManipulator->StartTransform(anXPix, anYPix, m_view);	// 初始化转换，记录起始位置
////    }*/
////
////
////
////    if(event->buttons()&Qt::LeftButton&&event->buttons()&Qt::RightButton)//event->buttons()实时更新按钮并判断；如果不加会导致左键之后，不管再按哪个键一直是平移
////    {
////        m_curremt_mode=CurrentAction_Panning;
////        m_x=event->pos().x();
////        m_y=event->pos().y();
////    }else if(event->buttons()&Qt::MidButton)
////    {
////        m_curremt_mode=CurrentAction_Rotation;
////       // m_view->StartRotation(event->pos().x(),event->pos().y());
////    }else if(event->buttons()&Qt::LeftButton)
////    {
////        polygon_end=true;
////        // 点击前，将鼠标位置传递到交互环境
////        m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
////        // 鼠标左键：选择模型
////        AIS_StatusOfPick t_pick_status = AIS_SOP_NothingSelected;
////        //if(qApp->keyboardModifiers()==Qt::ControlModifier)
////        //{
////        //    t_pick_status = m_context->ShiftSelect(true);   // 多选
////        //}
////        //else
////        //{
////        //    t_pick_status = m_context->Select(true);        // 单选
////        //   //myObeject=this->getshape(m_context);
////        //   // m_shapeArray.push_back(myObeject);
////
////        //  // TopoDS_Shape abc=this->getshape(m_context);一运行就卡掉，选择一个物体后就卡
////        //   //emit selectshape(abc);
////        //}
////
////
////    }
////    else if(event->buttons()&Qt::RightButton)
////    {
////         //aManipulator->DeactivateCurrentMode();//停止模型操纵器
////       // QVector<gp_Pnt> pntArry;
////        switch (m_current_type) {
////        case LINE:
////            a++;
////            if(a==1)  p1=this->ConvertClickToPoint(event->pos().x(),event->pos().y(),m_view);
////            if(a==2){
////                p2=this->ConvertClickToPoint(event->pos().x(),event->pos().y(),m_view);
////                emit drawline(p1,p2);
////                a=0;
////            }
////            break;
////        case CIRCLE:
////            a++;
////            if(a==1)  p1=this->ConvertClickToPoint(event->pos().x(),event->pos().y(),m_view);
////            if(a==2){
////                p2=this->ConvertClickToPoint(event->pos().x(),event->pos().y(),m_view);
////                emit drawcircle(p1,p2);
////                a=0;
////            }
////            break;
////        case POLYGON://先右键画点，然后左键使得为true，然后再右键发送信号
////            if(polygon_end==false)
////            {
////                gp_Pnt p=this->ConvertClickToPoint(event->pos().x(),event->pos().y(),m_view);
////                pntArry.push_back(p);
////            }
////            else {
////                emit drawPolygon(pntArry);
////                pntArry.clear();
////                polygon_end=false;
////            }
////          default:
////            break;
////        }
////
////    }
////}
////TopoDS_Shape OccView::getshape(const opencascade::handle<AIS_InteractiveContext> context)
////{
////    TopoDS_Shape abc = m_context->DetectedShape();
////                  TopAbs_ShapeEnum bba = abc.ShapeType();
////                      qDebug() << bba;
////    return abc;
////}
//gp_Pnt OccView::ConvertClickToPoint(Standard_Real x, Standard_Real y, opencascade::handle<V3d_View> aView)
//{
//    gp_Pln PlaneOfTheView = pln2;  //注意agpPlane是全局变量，创建一个平面，
//    //用于将鼠标点投射在此面上，这是将鼠标点投射在选定面上的
//    //基础
//    Standard_Real X,Y,Z;
//    aView->Convert(int(x),int(y),X,Y,Z);  //将鼠标点坐标转换为OCC三维坐标
//    gp_Pnt ConvertedPoint(X,Y,Z);
//    gp_Pnt2d ConvertedPointOnPlane = ProjLib::Project(PlaneOfTheView,ConvertedPoint);
//
//    gp_Pnt ResultPoint = ElSLib::Value(ConvertedPointOnPlane.X(),
//                                       ConvertedPointOnPlane.Y(),
//                                       PlaneOfTheView);
//
//    return ResultPoint;     //将鼠标点二维坐标转换为基于OCC三维坐标选定面上的坐标（OCC三维坐标）
//}
