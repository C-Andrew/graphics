#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QtWidgets>
#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>
#include <string>
// #include "algebra.hpp"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QStatusBar* messageBar, QStatusBar* fieldBar, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    // void resizeGL(int width, int height);

    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.

    // *** Fill in these functions (in viewer.cpp) ***

    // Set the parameters of the current perspective projection using
    // the semantics of gluPerspective().
    void set_perspective(double fov, double aspect,
                         double near, double far);

    // Restore all the transforms and perspective parameters to their
    // original state. Set the viewport to its initial size.
    void reset_view();
    void setMessageBar(QString msg);
    void setFieldBar(QString msg);
    void setRotateModel();
    void setScaleModel();
    void setTranslateModel();
    void setRotateView();
    void setPerspectiveView();
    void setTranslateView();
    void setViewportMode();

    enum class Mode { M_ROTATE, M_TRANSLATE, M_SCALE,
                      VI_ROTATE, VI_TRANSLATE, VI_PERSPECTIVE,
                      VIEWPORT_MODE };

protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when the mouse moves
    virtual void mouseMoveEvent ( QMouseEvent * event );
 
    // Draw a line -- call draw_init first!
    void draw_line(const QVector2D& p1, const QVector2D& p2) ;

    // Set the current colour
    void set_colour(const QColor& col);

    // Call t* m_viewhis before you begin drawing. Width and height are the width


    void draw_init();

    void rotateModel(float angle, Qt::MouseButtons pressed);
    void scaleModel(float val, Qt::MouseButtons pressed);
    void translateModel(float val, Qt::MouseButtons pressed);
    void perspectiveView(float val, Qt::MouseButtons pressed);
    void viewportHandler();
    void clipToPlane(QVector4D a, QVector4D b, int isWorld);
    void clipToViewport(QVector2D a, QVector2D b);
    double cot(double theta);
    float convertToWindowCoord(float x);
    float convertToWindowCoordY(float y);

private:

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mVertexBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mVertexBufferObject;
#endif

    QGLShaderProgram mProgram;

    int mColorLocation;
    
    // *** Fill me in ***
    // You will want to declare some more matrices here
    QMatrix4x4 m_projection;
    QMatrix4x4 m_RotateTranslateModel;
    QMatrix4x4 m_scaleModel;
    QMatrix4x4 m_modelgnomonTransforms;
    QMatrix4x4 m_worldgnomonTransforms;
    QMatrix4x4 m_view;
    QMatrix4x4 m_world;
    QVector<QVector4D> m_cubeModel;
    QVector<QVector4D> m_nearPlaneModel;
    QVector<QVector4D> m_farPlaneModel;
    QVector<QVector4D> m_viewportModel;


     
    QVector<QVector4D> m_gnomonModel;
    QVector<QVector4D> m_gnomonWorld; 
    QStatusBar* m_messageBar;
    QStatusBar* m_fieldBar;
    QVector2D m_mouseDown;
    QVector2D m_mouseUp;
    QWidget* m_parent;

    QVector<QColor> colors; 
    double mOrigin_x;

    double m_fov;
    double m_near;
    double m_far;
    double vp_x;
    double vp_y;
    double vp_width;
    double vp_height;
    float nearFieldPlane;
    float farfieldPlane;
    float windowSize;

    Mode m_mode;
};

#endif