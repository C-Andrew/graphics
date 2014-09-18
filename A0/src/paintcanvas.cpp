#include <QtGui>
// #include <QRect>
#include <cmath>
#include "paintcanvas.hpp"

using namespace std;

void PaintLine::draw(QPainter& painter) {
    QPointF p1(m_from[0], m_from[1]);
    QPointF p2(m_to[0], m_to[1]);
    painter.drawLine(p1, p2);
}

void PaintRect::draw(QPainter& painter) {
    float x = (m_from[0] < m_to[0]) ? m_from[0] : m_to[0];
    float y = (m_from[1] < m_to[1]) ? m_from[1] : m_to[1];
    float width = abs(m_from[0] - m_to[0]);
    float height = abs(m_from[1] - m_to[1]);
    painter.drawRect(QRect(x, y, width, height));
}

void PaintOval::draw(QPainter& painter) {
    float x = (m_from[0] < m_to[0]) ? m_from[0] : m_to[0];
    float y = (m_from[1] < m_to[1]) ? m_from[1] : m_to[1];
    float width = abs(m_from[0] - m_to[0]);
    float height = abs(m_from[1] - m_to[1]);
    painter.drawEllipse(QRect(x, y, width, height));
}

PaintCanvas::PaintCanvas(QWidget *parent) 
    : QWidget(parent)
    , m_mode(DRAW_LINE) {
    penColour = QColor("black");

}

PaintCanvas::~PaintCanvas() {
    for (list<PaintShape*>::iterator i = m_shapes.begin(); i != m_shapes.end(); ++i) {
        delete *i;
    }
}

QSize PaintCanvas::minimumSizeHint() const {
    return QSize(50, 50);
}   

QSize PaintCanvas::sizeHint() const {
    return QSize(300, 300);
}

void PaintCanvas::clear(){
    m_shapes.clear();
    update();
}

void PaintCanvas::setPenColour(QColor colour){
    penColour = colour;
}


void PaintCanvas::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    //painter.setPen(penColour);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Iterate over the stored shapes and ask them to draw themselves.
    for( list<PaintShape*>::iterator i = m_shapes.begin(); 
            i != m_shapes.end(); ++i ) {
        PaintShape *shape = (*i);
        painter.setPen(shape->colour);
	shape->draw( painter );
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QColor("black"));
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void PaintCanvas::mousePressEvent ( QMouseEvent * event ) {
    if (event->button() == Qt::LeftButton) {
        m_start_pos = Point2D(event->x(), event->y());
    }
}

void PaintCanvas::mouseReleaseEvent ( QMouseEvent * event ) {

    if (event->button() == Qt::LeftButton) { // Left mouse button
        Point2D to(event->x(), event->y());

        PaintShape *shape = NULL;

        switch (m_mode) {
        case DRAW_LINE:
            shape = new PaintLine( m_start_pos, to );
            break;
        case DRAW_OVAL:
            shape = new PaintOval( m_start_pos, to );
            break;
        case DRAW_RECTANGLE:
            shape = new PaintRect( m_start_pos, to );
            break;
        default:
            break;
        }

        if (shape) {
            shape->colour = penColour;
            m_shapes.push_back( shape );
            update();
        }
    }
}
