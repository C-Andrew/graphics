#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>
#include <QTimer>
#include "game.hpp"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, Game* game, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void resetWorld();
    void setWireframeMode();
    void setFillMode();
    void setMulticolourMode();

    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.
  
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

private slots:
    void gravity();

private:

    QMatrix4x4 getCameraMatrix();
    void paintWell();
    void paintCube();
    bool prepareColorsBuffer();
    bool prepareWellColorsBuffer();
    bool prepareVertexBuffer();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float angle, float x, float y, float z);
    void scaleWorld(float x, float y, float z);


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mVertexBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mVertexBufferObject;
    QGLBuffer mColorBufferObject;
#endif

    int mVertexLocation;
    int mMvpMatrixLocation;
    int mGravity;

    bool mApplyGravity;
    bool mWireframeMode;

    double mOrigin_x;
    double mOrigin_y;
    double mPrevX;
    double mXDiff;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mModelMatrices[4];
    QMatrix4x4 mTransformMatrix;
    QMatrix4x4 trs;
    QMatrix4x4 mGravityMatrix;

    Game* mGame;

    QTimer* mTimer;
    QTimer* mGravityTimer;
    QGLShaderProgram mProgram;
    QVector<QVector3D> colors;
};

#endif