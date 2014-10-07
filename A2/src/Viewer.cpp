#include <QtWidgets>
#include <QtOpenGL>
#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Viewer.hpp"
// #include "draw.hpp"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

using namespace std;

Viewer::Viewer(const QGLFormat& format, QStatusBar* messageBar, QStatusBar* fieldBar, QWidget *parent) 
    : QGLWidget(format, parent) 
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    , mVertexBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else 
    , mVertexBufferObject(QGLBuffer::VertexBuffer)
#endif
{   

    m_messageBar = messageBar;
    m_messageBar->setMaximumHeight(22);
    m_fieldBar = fieldBar;
    m_fieldBar->setMaximumHeight(22);
    windowSize = 300;
    m_parent = parent;
    m_cubeModel << QVector4D(1,-1,1,1) << QVector4D(-1,-1,1,1)
                << QVector4D(-1,-1,1,1)  << QVector4D(-1,1,1,1) 
                << QVector4D(-1,1,1,1)   << QVector4D(1,1,1,1) 
                << QVector4D(1,1,1,1)    << QVector4D(1,-1,1,1) // Front
                << QVector4D(1,-1,-1,1) <<  QVector4D(-1,-1,-1,1)
                << QVector4D(-1,-1,-1,1)  << QVector4D(-1,1,-1,1) 
                << QVector4D(-1,1,-1,1)   << QVector4D(1,1,-1,1) 
                << QVector4D(1,1,-1,1)    << QVector4D(1,-1,-1,1) // Back
                << QVector4D(1,-1,-1,1) << QVector4D(1,-1,1,1)
                << QVector4D(-1,-1,-1,1) << QVector4D(-1,-1,1,1)
                << QVector4D(-1,1,-1,1) << QVector4D(-1,1,1,1)
                << QVector4D(1,1,-1,1) <<  QVector4D(1,1,1,1);

    m_gnomonModel << QVector4D(0,0,0,1) << QVector4D(0,0.5,0,1)
                  << QVector4D(0,0,0,1) << QVector4D(-0.5,0,0,1)
                  << QVector4D(0,0,0,1) << QVector4D(0,0,-0.5,1); 

    m_gnomonWorld << QVector4D(0,0,0,1) << QVector4D(0,0.5,0,1)
                  << QVector4D(0,0,0,1) << QVector4D(-0.5,0,0,1)
                  << QVector4D(0,0,0,1) << QVector4D(0,0,-0.5,1);



    colors << QColor("blue") << QColor("blue")
           << QColor("green") << QColor("green")
           << QColor("purple") << QColor("purple");
    reset_view();
}

Viewer::~Viewer() {
    // Nothing to do here right now.
}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 300);
}

void Viewer::set_perspective(double fov, double aspect,
                             double near, double far)
{
    m_nearPlaneModel.clear();
    m_nearPlaneModel << QVector4D(1,-1,nearFieldPlane,1) << QVector4D(-1,-1,nearFieldPlane,1)
                    << QVector4D(-1,-1,nearFieldPlane,1) << QVector4D(-1,1,nearFieldPlane,1)
                    << QVector4D(-1,1,nearFieldPlane,1) << QVector4D(1,1,nearFieldPlane,1)
                    << QVector4D(1,1,nearFieldPlane,1) << QVector4D(1,-1,nearFieldPlane,1);
    m_farPlaneModel.clear();
    m_farPlaneModel << QVector4D(1,-1,farfieldPlane,1) << QVector4D(-1,-1,farfieldPlane,1)
                    << QVector4D(-1,-1,farfieldPlane,1) << QVector4D(-1,1,farfieldPlane,1)
                    << QVector4D(-1,1,farfieldPlane,1) << QVector4D(1,1,farfieldPlane,1)
                    << QVector4D(1,1,farfieldPlane,1) << QVector4D(1,-1,farfieldPlane,1);     

    m_projection.setRow(0, QVector4D( cot(fov/2)/aspect , 0 , 0 , 0 ));
    m_projection.setRow(1, QVector4D( 0 , cot(fov/2) , 0 , 0 ));
    m_projection.setRow(2, QVector4D( 0 , 0 , -(far+near)/(far-near) , (-2*far*near)/(far-near) ));
    m_projection.setRow(3, QVector4D( 0 , 0 , -1 , 0 ));
}

void Viewer::reset_view()
{
    // Fill me in!
    m_fov = 0.52359;
    nearFieldPlane = 3.8;
    farfieldPlane = 15;
    setFieldBar("");

    set_perspective(m_fov,1, nearFieldPlane, farfieldPlane);

    m_RotateTranslateModel.setToIdentity();
    m_scaleModel.setToIdentity();

    m_view.setToIdentity();
    m_view.translate(0,0,5);
    setRotateModel();

    vp_x = -0.9485   ;
    vp_y = 0.9485  ;
    vp_width  = 1.89;
    vp_height = -1.89;
 
     m_viewportModel.clear();
    m_viewportModel << QVector4D(vp_x,vp_y,1,1)       << QVector4D(vp_x+ vp_width,vp_y,1,1)
                    << QVector4D(vp_x + vp_width,vp_y,1,1) << QVector4D(vp_x+ vp_width,vp_y+ vp_height,1,1)
                    << QVector4D(vp_x + vp_width,vp_y+vp_height,1,1) << QVector4D(vp_x, vp_y+ vp_height,1,1)
                    << QVector4D(vp_x, vp_y+vp_height,1,1)      << QVector4D(vp_x,vp_y,1,1);  
    update();


}

void Viewer::initializeGL() {
    // Do some OpenGL setup
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glClearColor(0.7, 0.7, 0.7, 0.0);
    
    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        std::cerr << "Cannot load vertex shader." << std::endl;
        return;
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        std::cerr << "Cannot load fragment shader." << std::endl;
        return;
    }

    if ( !mProgram.link() ) {
        std::cerr << "Cannot link shaders." << std::endl;
        return;
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
#else 

    /*
     * if qt version is less than 5.1, use the following commented code
     * instead of QOpenGLVertexVufferObject. Also use QGLBuffer instead of 
     * QOpenGLBuffer.
     */
    uint vao;
     
    typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
    typedef void (APIENTRY *_glBindVertexArray) (GLuint);
     
    _glGenVertexArrays glGenVertexArrays;
    _glBindVertexArray glBindVertexArray;
     
    glGenVertexArrays = (_glGenVertexArrays) QGLWidget::context()->getProcAddress("glGenVertexArrays");
    glBindVertexArray = (_glBindVertexArray) QGLWidget::context()->getProcAddress("glBindVertexArray");
     
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);    

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QGLBuffer::DynamicDraw);
#endif

    if (!mVertexBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mColorLocation = mProgram.uniformLocation("frag_color");
    reset_view();
}

void Viewer::paintGL() {    
    draw_init();
    // Here is where your drawing code should go.
    
    /* A few of lines are drawn below to show how it's done. */
    set_colour(QColor(1.0,1.0,1.0));
    QVector2D point;
    QVector2D point2;
    for(int i = 0; i < m_cubeModel.size(); i++ ){
        // Clip line
        clipToPlane(m_cubeModel.at(i), m_cubeModel.at(i+1), 0);
        i++;
    }

    for(int i = 0; i < m_gnomonModel.size(); i++ ){
        set_colour(QColor(1.0,1.0,1.0));
        clipToPlane(m_gnomonModel.at(i), m_gnomonModel.at(i+1), 2);
        i++;
        // point = (  m_projection  * m_view * m_RotateTranslateModel * m_gnomonModel.at(i++) ).toVector2DAffine();
        // point2 = (  m_projection * m_view * m_RotateTranslateModel * m_gnomonModel.at(i) ).toVector2DAffine();
        // draw_line(point,point2);
    }

    for(int i = 0; i < m_gnomonWorld.size(); i++ ){
        set_colour(QColor(0.0,0.0,1.0));
        clipToPlane(m_gnomonWorld.at(i), m_gnomonWorld.at(i+1), 1);
        i++;
        // point = (  m_projection  * m_view * m_gnomonWorld.at(i++) ).toVector2DAffine();
        // point2 = (  m_projection * m_view * m_gnomonWorld.at(i) ).toVector2DAffine();
        // draw_line(point,point2);
    }

    for(int i = 0; i < m_viewportModel.size(); i++ ){
        set_colour(QColor(0.0,0.0,0.0));
        point = ( m_viewportModel.at(i++) ).toVector2DAffine();
        point2 = (  m_viewportModel.at(i) ).toVector2DAffine();
        // if(m_mode == Mode::VIEWPORT_MODE){
            draw_line(point,point2);
        // }
    }
    // Don't Draw near/far plane
    for(int i = 0; i < m_nearPlaneModel.size(); i++ ){
        set_colour(QColor(0.0, 1.0, 1.0));
        point = (  m_projection * m_nearPlaneModel.at(i++) ).toVector2DAffine();
        point2 = (  m_projection * m_nearPlaneModel.at(i) ).toVector2DAffine();
        if(m_mode == Mode::VI_PERSPECTIVE){
            point.setX( ((vp_width/2) * (point.x() - -1)) + vp_x);
            point.setY( ((vp_height/2) * (point.y() - 1)) +  vp_y + vp_height);

            point2.setX( ((vp_width/2) * (point2.x() - -1)) + vp_x);
            point2.setY( ((vp_height/2) * (point2.y() - 1)) +  vp_y + vp_height);
            draw_line(point,point2);
        }

    }
    for(int i = 0; i < m_farPlaneModel.size(); i++ ){
        set_colour(QColor(1.0, 1.0, 0.0));
        point = (  m_projection * m_farPlaneModel.at(i++) ).toVector2DAffine();
        point2 = (  m_projection * m_farPlaneModel.at(i) ).toVector2DAffine();
        if(m_mode == Mode::VI_PERSPECTIVE){
            point.setX( ((vp_width/2) * (point.x() - -1)) + vp_x);
            point.setY( ((vp_height/2) * (point.y() - 1)) +  vp_y + vp_height);

            point2.setX( ((vp_width/2) * (point2.x() - -1)) + vp_x);
            point2.setY( ((vp_height/2) * (point2.y() - 1)) +  vp_y + vp_height);
            draw_line(point,point2);
        }
    }
}

// int  isWorld specifies what I am clipping so I know what matricies to apply
// That is, modelgnomes do not scale but should be rotated/transed, but worldgnome should only be have view matrix
void Viewer::clipToPlane(QVector4D a, QVector4D b, int isWorld){

    if(isWorld == 1){
        // world gnomon
        a = ( m_view *  a );
        b = (m_view * b );
    }
    else if(isWorld == 2){
        // model gnomon
        a = ( m_view  * m_RotateTranslateModel *  a );
        b = (m_view  * m_RotateTranslateModel * b );
    }
    else{
        // Model matrix
        a = ( m_view *  m_scaleModel * m_RotateTranslateModel *  a );
        b = (m_view *   m_scaleModel * m_RotateTranslateModel * b );
    }


    QVector4D p_NearField = QVector4D(0,0,nearFieldPlane,0);
    QVector4D n_NearField = QVector4D(0,0,1,0);

    QVector4D p_FarField = QVector4D(0,0,farfieldPlane,0);
    QVector4D n_FarField = QVector4D(0,0,-1,0);

    QVector2D point;
    QVector2D point2;

    float t;
    float wecA = QVector4D::dotProduct((a - p_NearField), n_NearField);
    float wecB = QVector4D::dotProduct((b - p_NearField), n_NearField);
    // std::cerr << "A:  " << a.x() << "   " << a.y() << "   "<< a.z() << "   "<< a.w() << "   |wecA" << wecA << std::endl;
    // std::cerr << "B:  " << b.x() << "   " << b.y() << "   "<< b.z() << "   "<< b.w() << "   |wecB" <<wecB << std::endl;

    if(wecA < 0 && wecB < 0){  return; }  //OUTSIDE OF PLANE DONT DRAW
    else if(wecA >= 0 && wecB >= 0){ } //BOTH INSIDE PLANE, CLIP TO FAR PLANE
    else{
        t = wecA/(wecA-wecB);
        if(wecA < 0){
            a = a + (t*(b-a));
        }
        else{
            b = a + (t*(b-a));
        }
    }
    
    wecA = QVector4D::dotProduct((a-p_FarField), n_FarField);
    wecB = QVector4D::dotProduct((b-p_FarField), n_FarField);
    if(wecA < 0 && wecB < 0){ return;} // BOTH ARE OUTSIDE FAR PLANE
    else if(wecA >= 0 && wecB >= 0){} //BOTH INSIDE  FARPLANE, DRAW AS IS
    else{
        //CLIP SHIT TO FAR PLANE
        t = wecA/(wecA-wecB);
        if(wecA < 0){
            a = a + t*(b-a);
        }
        else{
            b = a + t*(b-a);
        }

    }

    point = (m_projection * a).toVector2DAffine();  
    point2 = (m_projection * b).toVector2DAffine();

    // viewport, translate points to viewport then clip
    point.setX( ((vp_width/2) * (point.x() - -1)) + vp_x);
    point.setY( ((vp_height/2) * (point.y() - 1)) +  vp_y + vp_height);

    point2.setX( ((vp_width/2) * (point2.x() - -1)) + vp_x);
    point2.setY( ((vp_height/2) * (point2.y() - 1)) +  vp_y + vp_height);

    //Clip to viewport in 2D
    clipToViewport(point, point2);

}

void Viewer::clipToViewport(QVector2D a, QVector2D b){
   //Clip to viewport in 2D
    QVector2D p_vp = QVector2D(vp_x,0);
    QVector2D n_vp = QVector2D(1,0);
    float t;
    float wecA = QVector2D::dotProduct( (a-p_vp) , n_vp);
    float wecB = QVector2D::dotProduct( (b-p_vp) , n_vp);
    if(wecA < 0 && wecB < 0){  return; }
    else if(wecA >= 0 && wecB >= 0){ }
    else{
        t = wecA/(wecA-wecB);
        if(wecA < 0){
            a = a + (t*(b-a));
        }
        else{
            b = a + (t*(b-a));
        }
    }

    p_vp = QVector2D(vp_x+vp_width, 0);
    n_vp = QVector2D(-1,0);
    wecA = QVector2D::dotProduct( (a-p_vp) , n_vp);
    wecB = QVector2D::dotProduct( (b-p_vp) , n_vp);
    if(wecA < 0 && wecB < 0){  return; }  
    else if(wecA >= 0 && wecB >= 0){ }
    else{
        t = wecA/(wecA-wecB);
        if(wecA < 0){
            a = a + (t*(b-a));
        }
        else{
            b = a + (t*(b-a));
        }
    }

    p_vp = QVector2D(0, vp_y);
    n_vp = QVector2D(0,-1);
    wecA = QVector2D::dotProduct( (a-p_vp) , n_vp);
    wecB = QVector2D::dotProduct( (b-p_vp) , n_vp);
    if(wecA < 0 && wecB < 0){  return; } 
    else if(wecA >= 0 && wecB >= 0){ } 
    else{
        t = wecA/(wecA-wecB);
        if(wecA < 0){
            a = a + (t*(b-a));
        }
        else{
            b = a + (t*(b-a));
        }
    }

    p_vp = QVector2D(0, vp_y+vp_height);
    n_vp = QVector2D(0,1);
    wecA = QVector2D::dotProduct( (a-p_vp) , n_vp);
    wecB = QVector2D::dotProduct( (b-p_vp) , n_vp);
    if(wecA < 0 && wecB < 0){  return; } 
    else if(wecA >= 0 && wecB >= 0){ } 
    else{
        t = wecA/(wecA-wecB);
        if(wecA < 0){
            a = a + (t*(b-a));
        }
        else{
            b = a + (t*(b-a));
        }
    }

    draw_line(a,b);
}

void Viewer::mousePressEvent ( QMouseEvent * event ) {
    // std::cerr << "Stub: button " << event->x() << "   " << event->y()  << " pressed\n";
    mOrigin_x = event->x();
    if(event->buttons() & Qt::LeftButton){
        // std::cerr<<"LEFT DOWN" << std::endl;
        m_mouseDown = QVector2D(event->x(), event->y());        
    }
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    // std::cerr << "Stub: button " << event->x() << "   " << event->y() << " released\n";
    mOrigin_x = 0;
    // m_mouseUp = QVector2D(event->x(), event->y());

    if(event->button() & Qt::LeftButton){
        // std::cerr<<"LEFT UP" << std::endl;
        m_mouseUp = QVector2D(event->x(), event->y());
        if(m_mode == Mode::VIEWPORT_MODE){
            // viewportHandler();
        } 
    }
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    // std::cerr << "Stub: Motion at " << event->x() << std::endl;
    double dx = event->x() - mOrigin_x;
    mOrigin_x = event->x();
    float scale = 1;
    float translate = 0;
    switch(m_mode){
        case Mode::M_ROTATE:
        case Mode::VI_ROTATE:
            rotateModel(dx, event->buttons());
            break;
        case Mode::M_SCALE:
            if(dx<0){ scale = 0.99; }
            else if(dx>0){ scale = 1.01; }
            scaleModel(scale, event->buttons());
            break;
        case Mode::M_TRANSLATE:
        case Mode::VI_TRANSLATE:
            if(dx<0){ translate = -0.05; }
            else if(dx>0){translate = 0.05; }
            translateModel(translate, event->buttons());
            break;
        case Mode::VI_PERSPECTIVE:
            if(dx<0){ translate = 0.05; }
            else if(dx>0){translate = -0.05; }
            perspectiveView(dx, event->buttons());
            break;
        case Mode::VIEWPORT_MODE:
            m_mouseUp = QVector2D(event->x(), event->y());
            viewportHandler();
            break;
        default:
            break;
    }
    update();

}

// Drawing Functions
// ******************* Do Not Touch ************************ // 

void Viewer::viewportHandler(){
    float x, y, width, height;
    x = convertToWindowCoord(m_mouseDown.x());
    y = convertToWindowCoordY(m_mouseDown.y());
    height = convertToWindowCoordY(m_mouseUp.y()) - y;
    if(m_mouseUp.x() > (m_parent->size().width()-18)){ width = convertToWindowCoord((m_parent->size().width()-18)) - x; }
    else if(m_mouseUp.x() < 0){ width = convertToWindowCoord(0) - x; }
    else{ width = convertToWindowCoord(m_mouseUp.x()) - x; }

    if(m_mouseUp.y() > (m_parent->size().height()-65)){ height = convertToWindowCoordY((m_parent->size().height()-65)) - y; }
    else if(m_mouseUp.y() < 0){ height = convertToWindowCoordY(0) - y; }
    else{ height = convertToWindowCoordY(m_mouseUp.y()) - y; }


    if(width < 0){
        x = x + width;
        width = -width;
    }
    if(height < 0){
        y = y + height;
        height = -height;
    }
    vp_x = x;
    vp_y = -y;
    vp_width = width;
    vp_height = -height;

    m_viewportModel.clear();
    m_viewportModel << QVector4D(vp_x,vp_y,1,1)                      << QVector4D(vp_x+ vp_width,vp_y,1,1)
                    << QVector4D(vp_x + vp_width,vp_y,1,1)           << QVector4D(vp_x+ vp_width,vp_y+ vp_height,1,1)
                    << QVector4D(vp_x + vp_width,vp_y+vp_height,1,1) << QVector4D(vp_x, vp_y+ vp_height,1,1)
                    << QVector4D(vp_x, vp_y+vp_height,1,1)           << QVector4D(vp_x,vp_y,1,1);  

    set_perspective(m_fov, 1, nearFieldPlane, farfieldPlane);
}

float Viewer::convertToWindowCoord(float x){
    // std::cerr << m_parent->size().width() << std::endl;
    return  ((x / (m_parent->size().width()-18)) * 2 ) - 1;
}

float Viewer::convertToWindowCoordY(float y){
        // std::cerr << m_parent->size().height() << std::endl;
    return  ((y / (m_parent->size().height()-65)) * 2 ) - 1;
}

void Viewer::draw_line(const QVector2D& p1, const QVector2D& p2) {

  const GLfloat lineVertices[] = {
    p1.x(), p1.y(), 1.0,
    p2.x(), p2.y(), 1.0
  };

  mVertexBufferObject.allocate(lineVertices, 3 * 2 * sizeof(float));

  glDrawArrays(GL_LINES, 0, 2);
}

void Viewer::set_colour(const QColor& col)
{
  mProgram.setUniformValue(mColorLocation, col.red(), col.green(), col.blue());
}

void Viewer::draw_init() {
    glClearColor(0.7, 0.7, 0.7, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width(), 0.0, (double)height());
    glViewport(0, 0, width(), height());

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0);
}

double Viewer::cot(double theta){
    return cos(theta)/sin(theta);
}

void Viewer::setMessageBar(QString msg){
    m_messageBar->showMessage(msg);
}

void Viewer::setFieldBar(QString msg){
    QString nearFar = "Near-field plane: " + QString::number(nearFieldPlane);
    nearFar += "    Far-field plane: " + QString::number(farfieldPlane);
    nearFar += QString("  ") + msg;
    m_fieldBar->showMessage(nearFar);
}

void Viewer::rotateModel(float angle, Qt::MouseButtons pressed){
    float x, y, z;
    x = y = z = 0;

    if(pressed & Qt::LeftButton)      {x = 1;}
    if(pressed & Qt::RightButton)     {z = 1;}
    if(pressed & Qt::MidButton)       {y = 1;}
    if(m_mode == Mode::M_ROTATE){
        m_RotateTranslateModel.rotate(angle, x, y, z);
    }
    else if(m_mode == Mode::VI_ROTATE){
        m_view.translate(0,0,-5);
        m_view.rotate(angle,x,y,z);
        m_view.translate(0,0,5);
    }

}

void Viewer::scaleModel(float val, Qt::MouseButtons pressed){
    float x, y, z;
    x = y = z = 1;

    if(pressed & Qt::LeftButton){x = val;}
    else if(pressed & Qt::RightButton)     {z = val;}
    else if(pressed & Qt::MidButton)       {y = val;}

    m_scaleModel.scale(x, y, z);
   
}

void Viewer::translateModel(float val, Qt::MouseButtons pressed){
    float x, y, z;
    x = y = z = 0;

    if(pressed & Qt::LeftButton)      {x = val;}
    if(pressed & Qt::RightButton)     {z = val;}
    if(pressed & Qt::MidButton)       {y = val;}
    if(m_mode == Mode::M_TRANSLATE){
        m_RotateTranslateModel.translate(x, y, z);
    }
    else if(m_mode == Mode::VI_TRANSLATE){
        // std::cerr << val << std::endl;
        m_view.translate(x,y,z);
    }

}

void Viewer::perspectiveView(float val, Qt::MouseButtons pressed){
    float x, y, z;
    x = y = z = 0;

    if(pressed & Qt::LeftButton){
        if(val < 0 && m_fov < 2.7925268){
            m_fov +=  (M_PI/180);
        }
        else if(val > 0 && m_fov > 0.0872664626){ 
            m_fov -=  (M_PI/180);
        }
    }
    if(pressed & Qt::RightButton)     {
        if(val < 0){
            if(nearFieldPlane < farfieldPlane){
                nearFieldPlane +=  0.1;
            }
        }
        else{
            if(nearFieldPlane > 3.0){
                nearFieldPlane -=  0.1;
            }
        }

    }
    if(pressed & Qt::MidButton){
        if(val < 0){
            if(farfieldPlane > nearFieldPlane){
                farfieldPlane -=  0.1;
            }
        }
        else{
            if(farfieldPlane < 24){
                farfieldPlane +=  0.1;
            }
        }
    }
    setFieldBar(QString(""));
    set_perspective(m_fov,1, nearFieldPlane, farfieldPlane);
}



void Viewer::setRotateModel(){
    m_mode = Mode::M_ROTATE;
    setFieldBar("ROTATE MODEL MODE");
}

void Viewer::setScaleModel(){
    m_mode = Mode::M_SCALE;
    setFieldBar("SCALE MODEL MODE");
}

void Viewer::setTranslateModel(){
    m_mode = Mode::M_TRANSLATE;
    setFieldBar("TRANSLATE MODEL MODE");
}

void Viewer::setRotateView(){
    m_mode = Mode::VI_ROTATE;
    setFieldBar("ROTATE VIEW MODE");
}
void Viewer::setPerspectiveView(){
    m_mode = Mode::VI_PERSPECTIVE;
    setFieldBar("PERSPECTIVE VIEW MODE");
    update();
}
void Viewer::setTranslateView(){
    m_mode = Mode::VI_TRANSLATE;
    setFieldBar("TRANSLATE VIEW MODE");
}

void Viewer::setViewportMode(){
    m_mode = Mode::VIEWPORT_MODE;
    setFieldBar("VIEWPORT MODE");
    update();
}