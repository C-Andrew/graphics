#ifndef PAINTWINDOW_HPP
#define PAINTWINDOW_HPP

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include "paintcanvas.hpp"

// class PaintCanvas;

class PaintWindow : public QMainWindow
{
    Q_OBJECT

public:
    PaintWindow();
    virtual ~PaintWindow() {}


protected:

private slots:
    // Display a message box providing assistance with the line drawing
    // function.
    void help_line();
    void help_rectangle();
    void help_oval();

    void set_line();
    void set_rect();
    void set_oval();
    void set_pen_red();
    void set_pen_black();
    void set_pen_blue();
    void set_pen_green();
    void clear();

private:

    void createMenu();

    // Note: QMainWindow has its own QMenuBar where as QWidget does not
    // Each drop down menus
    QMenu* m_menu_app;
    QMenu* m_menu_tools;
    QMenu* m_menu_colours;
    QMenu* m_menu_help;

    // The canvas onto which the user draws.
    PaintCanvas* m_canvas;
};

#endif
