#include <QtWidgets>
#include <QtOpenGL>
#include "Viewer.hpp"
#include "material.hpp"

#include <iostream>
#include <math.h>
#include <GL/glu.h>


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

Viewer::Viewer(const QGLFormat& format, SceneNode* root, QWidget *parent) 
    : QGLWidget(format, parent) 
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    , mCircleBufferObject(QOpenGLBuffer::VertexBuffer)
    , mSphereBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else 
    , mCircleBufferObject(QGLBuffer::VertexBuffer)
#endif
{
    m_root = root;
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

void Viewer::initializeGL() {
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glClearColor( 0.4, 0.4, 0.4, 0.0 );
    
    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        std::cerr << "Cannot load vertex shader." << std::endl;
        return;
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        std::cerr << "Cannot load fraVertgment shader." << std::endl;
        return;
    }

    if ( !mProgram.link() ) {
        std::cerr << "Cannot link shaders." << std::endl;
        return;
    }

    // Init member  variables
    m_mode = Mode::M_ORIENTATION;
    zbuffer = backfacecull = frontfacecull = circle = m_picking_mode = false;

    numOfSphereVertices = 3;
    float sphereData[] = { 1, 0, 0,
                          -1, 0, 0,
                           0, 1, 0 };

    double radius = width() < height() ? 
        (float)width() * 0.25 : (float)height() * 0.25;
        
    for(size_t i=0; i<40; ++i) {
        circleData[i*3] = radius * cos(i*2*M_PI/40);
        circleData[i*3 + 1] = radius * sin(i*2*M_PI/40);
        circleData[i*3 + 2] = 0.0;
    }
    getAllSceneNodes();


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mCircleBufferObject.create();
    mCircleBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
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

    mCircleBufferObject.create();
    mCircleBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
    mSphereBufferObject.create();
    mSphereBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
#endif

    if (!mCircleBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }


    genSphereVerts();
    mSphereBufferObject.allocate(sphereVerts.data(), (numOfSphereVertices+40) * 3 * sizeof(float));
    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mMvpMatrixLocation = mProgram.uniformLocation("mvpMatrix");
    mColorLocation = mProgram.uniformLocation("frag_color");
    mKdLocation = mProgram.uniformLocation("Kd");
    mKsLocation = mProgram.uniformLocation("Ks");
    mShinyLocation = mProgram.uniformLocation("Shiny");
    mNormalLocation = mProgram.uniformLocation("normal");
    mLightLocation = mProgram.uniformLocation("lightPos");
    mEyeLocation = mProgram.uniformLocation("eyePos");
    mUseColour = mProgram.uniformLocation("useColour");
    mIsPicking = mProgram.uniformLocation("isPicking");
}

void Viewer::paintGL() {
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up lighting

    // Draw stuff
    // mCircleBufferObject.allocate(circleData, 40 * 3 * sizeof(float));
    if(circle){
        draw_trackball_circle();
    }
        
    // During init, I have already placed all nodes in allNodes Vector
    // Iterate over vector and draw each one.
    // QVectorIterator<SceneNode*> i(allNodes);
    // while (i.hasNext()){    
    //     SceneNode* n = i.next();
    //     drawSphere(n);
    // }
    drawRoot();

}

void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}

void Viewer::mousePressEvent ( QMouseEvent * event ) {
    // std::cerr << "Stub: button " << event->button() << " pressed\n";
    m_Originx = event->x();
    m_Originy = event->y();
    m_mouseDown = QVector2D(event->x(), event->y());    

    // turn off texturing, lighting and fog
    // glDisable(GL_TEXTURE_2D);
    // glDisable(GL_FOG);
    // glDisable(GL_LIGHTING);

    if(event->button() & Qt::LeftButton && m_mode == Mode::M_JOINTS){
        m_picking = true;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //If in picking mode
        //redraw everything with their specific colours
        drawRoot();
        unsigned char pixel[3];
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glReadPixels(event->x(), viewport[3] - event->y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        // std::cerr << "colour id: " << (unsigned int) pixel[0] << std::endl;
        // m_root->selectNode(unsigned int id);
        if(m_mode == Mode::M_JOINTS){
            QVectorIterator<SceneNode*> i(allNodes);
            while (i.hasNext()){    
                SceneNode* n = i.next();
                if((unsigned int)n->m_color == (unsigned int) pixel[0]){
                    n->m_selected = !n->m_selected;
                    break;
                }
            }    
        }


        m_picking = false;
        mProgram.setUniformValue(mIsPicking, 0.0f);
        mProgram.setUniformValue(mUseColour, 1.0f);
        drawRoot();
    }
    else if(event->button() & Qt::RightButton && m_mode == Mode::M_JOINTS){
        m_picking = true;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //If in picking mode
        //redraw everything with their specific colours
        drawRoot();
        unsigned char pixel[3];
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glReadPixels(event->x(), viewport[3] - event->y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        // std::cerr << "colour id: " << (unsigned int) pixel[0] << std::endl;


        m_picking = false;
        mProgram.setUniformValue(mIsPicking, 0.0f);
        mProgram.setUniformValue(mUseColour, 1.0f);
        drawRoot();
    }

    update();

}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    // std::cerr << "Stub: button " << event->button() << " released\n";
    m_mouseUp = QVector2D(event->x(), event->y());
    m_Originx = 0;
    m_Originy = 0;
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    // std::cerr << "Stub: Motion at " << event->x() << ", " << event->y() << std::endl;
    double dx = event->x() - m_Originx;
    double dy = event->y() - m_Originy;
    m_Originx = event->x();
    m_Originy = event->y();

    switch(m_mode){
        case Mode::M_ORIENTATION:
            handleOrientationMode(dx, dy, event->buttons());
            break;
        case Mode::M_JOINTS:
            // Move everything that has been selected
            handleJointsMode(dx,dy, event->buttons());
            break;
        default:
            break;
    }
    update(); 
}

QMatrix4x4 Viewer::getCameraMatrix() {
    // Todo: Ask if we want to keep this.m_Originx
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    // return mPerspMatrix * vMatrix * mTransformMatrix;
    return mPerspMatrix * vMatrix * mTranslateMatrix * mRotateMatrix * mScaleMatrix;
}    // else{
    //     mProgram.setUniformValue(mIsPicking, 0.0f);
    // }

void Viewer::translateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTranslateMatrix.translate(x, y, z);
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float theta, QVector3D rotateVector) {
    // Todo: Ask if we want to keedrawSpherep this
    mRotateMatrix.rotate(theta, rotateVector);
    mTransformMatrix.rotate(theta, rotateVector);

}

void Viewer::scaleWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mScaleMatrix.scale(x,y,z);
    mTransformMatrix.scale(x, y, z);
}

void Viewer::set_colour(const QColor& col)
{
  mProgram.setUniformValue(mUseColour, 0.0f);
  mProgram.setUniformValue(mColorLocation, col.red(), col.green(), col.blue());
}

void Viewer::draw_trackball_circle()
{   
    int current_width = width();
    int current_height = height();

    // Set up for orthogonal drawing to draw a circle on screen.mEyeLocation
    // You'll want to make the rest of the function conditional on
    // whether or not we want to draw the circle this time around.



    // Set orthographic Matrix
    QMatrix4x4 orthoMatrix;
    orthoMatrix.ortho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);

    // Translate the view to the middle
    QMatrix4x4 transformMatrix;
    transformMatrix.translate(width()/2.0, height()/2.0, 0.0);

    // Bind buffer object
    mSphereBufferObject.bind();
    mProgram.setUniformValue(mMvpMatrixLocation, orthoMatrix * transformMatrix);

    // Draw buffer
    set_colour(QColor(0.0, 0.0, 0.0));
    glDrawArrays(GL_LINE_LOOP, numOfSphereVertices, 40);   
}

void Viewer::drawSphere(QMatrix4x4 m, QColor c, double color_id, bool picking, bool isSelected){

    QMatrix4x4 transformMatrix = m;

    mSphereBufferObject.bind();
    mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * transformMatrix);
    if(m_picking){
        mProgram.setUniformValue(mKdLocation, QVector3D(color_id/255.0f, 0, 0));
        mProgram.setUniformValue(mIsPicking, 1.0f);
        mProgram.setUniformValue(mUseColour, 1.0f);
    }
    else if(isSelected){
        mProgram.setUniformValue(mKdLocation, QVector3D(1.0f, 1.0f, 1.0f));
    }
    // else{
    //     mProgram.setUniformValue(mIsPicking, 0.0f);
    // }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numOfSphereVertices);  
    // update();  
}

void Viewer::drawRoot(){
    m_root->walk_gl(m_picking);
}

void Viewer::genSphereVerts(){
    numOfSphereVertices= 0;

    for(int i = 0; i <= 20; i++)
    {
        float lat0 = M_PI * (-0.5 + (float) (i - 1) / 20);
        float z0  = sin(lat0);
        float zr0 =  cos(lat0);

        float lat1 = M_PI * (-0.5 + (float) i / 20);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        for(int j = 0; j <= 20; j++)
        {
            float lng = 2 * M_PI * (float) (j - 1) / 20;
            float x = cos(lng);
            float y = sin(lng);
            
            sphereVerts << (x * zr0); //X
            sphereVerts << (y * zr0); //Y
            sphereVerts << (z0);      //Z

            sphereVerts << (x * zr1); //X
            sphereVerts << (y * zr1); //Y
            sphereVerts << (z1);      //Z
            numOfSphereVertices += 2;
        }
    }
    for(int i = 0; i < 120; i++){
        sphereVerts << circleData[i];
        // numOfSphereVertices++;
    }
    // std::cerr << numOfSphereVertices << std::endl;
    // mSphereBufferObject.allocate(sphereVerts.data(), numOfSphereVertices * 3 * sizeof(float));
}

void Viewer::getAllSceneNodes(){
    QVector<SceneNode*> nodes;
    nodes << m_root;
    int i = 0;
    while(!nodes.isEmpty()){
        SceneNode* cur = nodes.data()[0];
        nodes.pop_front();

        for (std::list<SceneNode*>::iterator it=cur->m_children.begin(); it != cur->m_children.end(); ++it){
            nodes << *it;
        }
        cur->initTrans = cur->get_transform();
        allNodes << cur;
        i++;
        // std::cerr << (all_nodes[i])->get_name();
    }
    m_numOfNodes = i;
}



void Viewer::setupMaterial(Colour kd, Colour ks, double shininess, QMatrix4x4 trans){
    QMatrix4x4 mvp = (getCameraMatrix() * trans);

    // set_colour(QColor(kd.R(), kd.G(), kd.B())); //Change for shading
    mProgram.setUniformValue(mKdLocation, QVector3D(kd.R(), kd.G(), kd.B()));
    mProgram.setUniformValue(mKsLocation, QVector3D(ks.R(), ks.G(), ks.B()));
    mProgram.setUniformValue(mShinyLocation, QVector3D(shininess, 0, 0));
    mProgram.setUniformValue(mLightLocation, QVector4D(0,100,-100,1));
    mProgram.setUniformValue(mEyeLocation, getCameraMatrix());
    mProgram.setUniformValue(mNormalLocation, mvp.normalMatrix());
    mProgram.setUniformValue(mUseColour, 1.0f);


    // update();
}

void Viewer::handleOrientationMode(double dx, double dy, Qt::MouseButtons pressed){
    if(pressed & Qt::LeftButton){
        //translate x 
        translateWorld(dx/25, -dy/25, 0);
    }
    else if(pressed & Qt::RightButton){
        if( abs(m_Originy - m_mouseDown.y()) < 200 ){
            translateWorld(0, 0, dy/25);            
        }
    }
    else if(pressed & Qt::MidButton){
        vCalcRotVec((float) (m_Originx)- (width()/2), (float) (m_Originy) - (width()/2),
                    m_mouseDown.x()- (height()/2), m_mouseDown.y() - (height()/2),
                    250.0);
        rotateWorld(m_rotateVector.length() * (180/M_PI), m_rotateVector);
    }

}

void Viewer::handleJointsMode(double dx, double dy, Qt::MouseButtons pressed){
    if(pressed & Qt::RightButton || pressed & Qt::MidButton){
        // Get everything that is selected's parents
        QVector<SceneNode*> selectedJoints;
        QVectorIterator<SceneNode*> i(allNodes);
        while (i.hasNext()){    
            SceneNode* n = i.next();
            for (std::list<SceneNode*>::const_iterator it=n->m_children.begin(); it != n->m_children.end(); ++it){
                if( (*it)->m_selected == true){
                    if( !selectedJoints.contains( (*it) ) ){
                        selectedJoints << n;                   
                    }
                    break;
                }
            }
        }  


        QVectorIterator<SceneNode*> j(selectedJoints);
        while (j.hasNext()){    
            SceneNode* n = j.next();
            // std::cerr << "NAME:" <<  n->get_name() << std::endl;     
            if(pressed & Qt::RightButton){
                n->rotate('x', dy); 
                if(pressed & Qt::MidButton && n->get_name() == "headJoint"){
                     n->rotate('y', dx);  
                }               
            }
            else if(pressed & Qt::MidButton && n->get_name() == "headJoint"){
                 n->rotate('y', dx);  
            }

            // n->set_transform( transToRotate );
        }  
    }

}

/*******************************************************
 * 
 * void vCalcRotVec(float fNewX, float fNewY, 
 *                  float fOldX, float fOldY,
 *                  float fDiameter,
 *                  float *fVecX, float *fVecY, float *fVecZ);
 *
 *    Calculates a rotation vector based on mouse motion over
 *    a virtual trackball.
 *
 *    The fNew and fOld mouse positions
 *    should be in 'trackball' space. That     bool frontfacecull;is, they have been
 *    transformed into a coordinate system centered at the middle
 *    of the trackball. fNew, fOld, and fDiameter must all be specified
 *    in the same units (pixels for example).
 *
 * Parameters: fNewX, fNewY - New mouse position in trackball space.
 *                            This is the second point along direction
 *                            of rotation.
 *             fOldX, fOldY - Old mouse position in trackball space.
 *                            This is the first point along direction
 *                            of rotation.
 *             fDiameter - Diameter of the trackball. This should
 *                         be specified in the same units as fNew and fOld.
 *                         (ie, usually pixels if fNew and fOld are transformed
 *                         mouse positions)
 *             fVec - The output rotation vector. The length of the vector
 *                    is proportional to the angle of rotation.
 *
 *******************************************************/
void Viewer::vCalcRotVec(float fNewX, float fNewY,
                 float fOldX, float fOldY,
                 float fDiameter) {
   long  nXOrigin, nYOrigin;
   float fNewVecX, fNewVecY, fNewVecZ,        /* Vector corresponding to new mouse location */
         fOldVecX, fOldVecY, fOldVecZ,        /* Vector corresponding to old mouse location */
         fLength;

   /* Vector pointing from center of virtual trackball to
    * new mouse position
    */
   fNewVecX = fNewX * 2.0 / fDiameter;
   fNewVecY = fNewY * 2.0 / fDiameter;
   fNewVecZ = (1.0 - fNewVecX * fNewVecX - fNewVecY * fNewVecY);

   /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
   if (fNewVecZ < 0.0) {
      fLength = sqrt(1.0 - fNewVecZ);
      fNewVecZ  = 0.0;
      fNewVecX /= fLength;
      fNewVecY /= fLength;
   } else {
      fNewVecZ = sqrt(fNewVecZ);
   }

   /* Vector pointing from center of virtual trackball to
    * old mouse position
    */
   fOldVecX    = fOldX * 2.0 / fDiameter;
   fOldVecY    = fOldY * 2.0 / fDiameter;
   fOldVecZ    = (1.0 - fOldVecX * fOldVecX - fOldVecY * fOldVecY);
 
   /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
   if (fOldVecZ < 0.0) {
      fLength = sqrt(1.0 - fOldVecZ);
      fOldVecZ  = 0.0;
      fOldVecX /= fLength;
      fOldVecY /= fLength;
   } else {
      fOldVecZ = sqrt(fOldVecZ);
   }

   /* Generate rotation vector by calculating cross product:
    * 
    * fOldVec x fNewVec.
    * 
    * The rotation vector is the axis of rotation
    * and is non-unit length since the length of a crossproduct
    * is related to the angle between fOldVec and fNewVec which we need
    * in order to perform the rotation.
    */
    float fVecX, fVecY, fVecZ;
   fVecX = fOldVecY * fNewVecZ - fNewVecY * fOldVecZ;
   fVecY = fOldVecZ * fNewVecX - fNewVecZ * fOldVecX;
   fVecZ = fOldVecX * fNewVecY - fNewVecX * fOldVecY;

    m_rotateVector = QVector3D(fVecX, fVecY, fVecZ);
}

void Viewer::resetAll(){
    mScaleMatrix.setToIdentity();
    mRotateMatrix.setToIdentity();
    mTranslateMatrix.setToIdentity();
    QVectorIterator<SceneNode*> i(allNodes);
    while (i.hasNext()){    
        SceneNode* n = i.next();
            // std::cerr << "MARKED AS UN/SELECTED" << n->m_selected << std:: endl;
        n->m_selected = false;
            // std::cerr << "MARKED AS UN/SELECTED" << n->m_selected << std:: endl;
    }  
    resetJoints();
    update();
}

void Viewer::resetPosition(){
    mTranslateMatrix.setToIdentity();
    update();
}

void Viewer::resetJoints(){
    QVectorIterator<SceneNode*> i(allNodes);
    while (i.hasNext()){    
        SceneNode* n = i.next();
        n->set_transform(n->initTrans);
        n->m_selected = false;
    }  
    update();
}

void Viewer::resetOrientation(){
    mRotateMatrix.setToIdentity();
    update();
}

void Viewer::enableTrackballCircle(){
    circle = !circle;
    update();
}

void Viewer::enablePicking(){
    m_mode = Mode::M_JOINTS;
    m_picking_mode = !m_picking_mode;
    // std::cerr << m_picking << std::endl;
}

void Viewer::enableOrientation(){
    m_mode = Mode::M_ORIENTATION;
    // std::cerr << m_picking << std::endl;
}


bool Viewer::setzbuffer(){
    zbuffer = !zbuffer;
    if(zbuffer){
        glEnable(GL_DEPTH_TEST);
        glDepthMask(true);
    }
    else{
        glDisable(GL_DEPTH_TEST);
        glDepthMask(false);
    }
    update();
    return zbuffer;
}

bool Viewer::setbackfacecull(){
    backfacecull = !backfacecull;
    if(backfacecull){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if(frontfacecull){
            glCullFace(GL_FRONT_AND_BACK);
        }
    }
    else{
        if(frontfacecull){
            glCullFace(GL_FRONT);
        }
        else{
            glDisable(GL_CULL_FACE);            
        }
    }
    update();
    return backfacecull;
}

bool Viewer::setfrontfacecull(){
    frontfacecull = !frontfacecull;
    if(frontfacecull){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        if(backfacecull){
            glCullFace(GL_FRONT_AND_BACK);
        }
    }
    else{
        if(backfacecull){
            glCullFace(GL_BACK);
        }
        else{
            glDisable(GL_CULL_FACE);            
        }
    }
    update();
    return frontfacecull;
}


