#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <math.h>
#include <QtGlobal>
#include "scene_lua.hpp"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:

    QGLShaderProgram mProgram;
    Viewer(const QGLFormat& format, SceneNode* root, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void drawSphere(QMatrix4x4 m, QColor c, double color_id, bool picking, bool isSelected);
    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.
    bool setzbuffer();
    bool setbackfacecull();
    bool setfrontfacecull();
    void resetAll();
    void resetOrientation();
    void resetJoints();
    void resetPosition();
    void enableTrackballCircle();
    void enablePicking();
    void enableOrientation();

    void setupMaterial(Colour kd, Colour ks, double shininess, QMatrix4x4 trans);

    enum class Mode { M_ORIENTATION, M_JOINTS };
  
protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when the mouse moves
    virtual void mouseMoveEvent ( QMouseEvent * event );
    
    // Draw a circle for the trackball, with OpenGL commands.
    // Assumes the context for the viewer is active.
    void draw_trackball_circle();
    void drawRoot();

private:

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float theta, QVector3D rotateVector);
    void scaleWorld(float x, float y, float z);
    void handleOrientationMode(double dx, double dy, Qt::MouseButtons pressed);
    void handleJointsMode(double dx, double dy, Qt::MouseButtons pressed);


    void set_colour(const QColor& col);
    void genSphereVerts();
    void getAllSceneNodes();

    void vCalcRotVec(float fNewX, float fNewY,
                 float fOldX, float fOldY,
                 float fDiameter);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mCircleBmSphereBuffermousePresOglEnable(GL_CULL_FACE);glEnable(GL_CULL_FACE);bjectufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mCircleBufferObject;
    QGLBuffer mSphereBufferObject;
#endif
    
    int mMvpMatrixLocation;
    int mUseColour;
    int mKdLocation;
    int mKsLocation;
    int mShinyLocation;
    int mColorLocation;
    int mNormalLocation;
    int mLightLocation;
    int mEyeLocation;
    int mIsPicking;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mTransformMatrix;
    QMatrix4x4 mTranslateMatrix;
    QMatrix4x4 mScaleMatrix;
    QMatrix4x4 mRotateMatrix;

    int numOfSphereVertices;

    float circleData[120];
    float sphereData[];
    // float sphereVerts[];u
    QVector3D m_rotateVector;
    QVector<float> sphereVerts;
    SceneNode* m_root;

    QVector<SceneNode*> allNodes;
    SceneNode* all_nodes[];
    int m_numOfNodes;

    QVector2D m_mouseDown;
    QVector2D m_mouseUp;
    double m_Originx;
    double m_Originy;
    double m_Originz;

    bool zbuffer;
    bool backfacecull;
    bool frontfacecull;
    bool circle;
    bool m_picking_mode;
    bool m_picking;
    Mode m_mode;
};

#endif