#include <QtWidgets>
#include <QList>
#include <QPushButton>
#include <iostream>
#include "paintwindow.hpp"

PaintWindow::PaintWindow() {
    setWindowTitle("488 Paint");

    QVBoxLayout *layout = new QVBoxLayout;

    m_canvas = new PaintCanvas(this);
    layout->addWidget(m_canvas);

    QPushButton *button = new QPushButton("&Quit", this);
    connect(button, SIGNAL(released()), this, SLOT(close()));
    layout->addWidget(button);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createMenu();
}

void PaintWindow::createMenu() {
    // Adding the drop down menu to the menubar
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_menu_tools = menuBar()->addMenu(tr("&Tools"));
    m_menu_colours = menuBar()->addMenu(tr("&Colours"));
    m_menu_help = menuBar()->addMenu(tr("&Help"));

    //////////////////////
    // Application Menu //
    //////////////////////

    QAction* quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Exits the program"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
    m_menu_app->addAction(quitAct);

    QAction* clearAct = new QAction(tr("&Clear"), this);
    QList<QKeySequence> shortcuts;
    shortcuts.append(tr("c"));
    shortcuts.append(tr("shift+c"));
    clearAct->setShortcuts(shortcuts);
    shortcuts.clear();
    clearAct->setStatusTip(tr("Clears the window"));
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clear()));
    m_menu_app->addAction(clearAct);


    ////////////////
    // Tools Menu //
    ////////////////
    QAction* drawLineAct = new QAction(tr("&Line"), this);
    shortcuts.append(tr("l"));
    shortcuts.append(tr("shift+l"));
    drawLineAct->setShortcuts(shortcuts);
    shortcuts.clear();
    drawLineAct->setStatusTip(tr("Draws a line"));
    connect(drawLineAct, SIGNAL(triggered()), this, SLOT(set_line()));

    QAction* drawOvalAct = new QAction(tr("&Oval"), this);
    shortcuts.append(tr("o"));
    shortcuts.append(tr("shift+o"));
    drawOvalAct->setShortcuts(shortcuts);
    shortcuts.clear();
    drawOvalAct->setStatusTip(tr("Draws an Oval"));
    connect(drawOvalAct, SIGNAL(triggered()), this, SLOT(set_oval()));

    QAction* drawRectangleAct = new QAction(tr("&Rectangle"), this);
    shortcuts.append(tr("r"));
    shortcuts.append(tr("shift+r"));
    drawRectangleAct->setShortcuts(shortcuts);
    shortcuts.clear();
    drawRectangleAct->setStatusTip(tr("Draws a rectangle"));
    connect(drawRectangleAct, SIGNAL(triggered()), this, SLOT(set_rect()));

    drawLineAct->setCheckable(true);
    drawOvalAct->setCheckable(true);
    drawRectangleAct->setCheckable(true);  

    QActionGroup* group = new QActionGroup(this);
    drawLineAct->setActionGroup(group);
    drawOvalAct->setActionGroup(group);
    drawRectangleAct->setActionGroup(group);
    drawLineAct->toggle();

    m_menu_tools->addAction(drawLineAct);
    m_menu_tools->addAction(drawOvalAct);
    m_menu_tools->addAction(drawRectangleAct);

    //////////////////
    // Colours Menu //
    //////////////////

    QAction* blackAct = new QAction(tr("&Black"), this);
    blackAct->setStatusTip(tr("Change drawing colour to black"));
    connect(blackAct, SIGNAL(triggered()), this, SLOT(set_pen_black()));
    
    QAction* redAct = new QAction(tr("&Red"), this);
    redAct->setStatusTip(tr("Change drawing colour to red"));
    connect(redAct, SIGNAL(triggered()), this, SLOT(set_pen_red()));
    
    QAction* greenAct = new QAction(tr("&Green"), this);
    greenAct->setStatusTip(tr("Change drawing colour to green"));
    connect(greenAct, SIGNAL(triggered()), this, SLOT(set_pen_green()));
    
    QAction* blueAct = new QAction(tr("&Blue"), this);
    blueAct->setStatusTip(tr("Change drawing colour to blue"));
    connect(blueAct, SIGNAL(triggered()), this, SLOT(set_pen_blue()));
    
    m_menu_colours->addAction(blackAct);
    m_menu_colours->addAction(redAct);
    m_menu_colours->addAction(greenAct);
    m_menu_colours->addAction(blueAct);

    ////////////////
    // Help Menu //
    ///////////////

    QAction* helpLineAct = new QAction(tr("&Line Help"), this);
    helpLineAct->setStatusTip(tr("Line Help Instructions"));
    connect(helpLineAct, SIGNAL(triggered()), this, SLOT(help_line()));
    
    QAction* helpRectangleAct = new QAction(tr("&Rectangle Help"), this);
    helpRectangleAct->setStatusTip(tr("Rectangle Help Instructions"));
    connect(helpRectangleAct, SIGNAL(triggered()), this, SLOT(help_rectangle()));

    QAction* helpOvalAct = new QAction(tr("&Oval Help"), this);
    helpOvalAct->setStatusTip(tr("Oval Help Instructions"));
    connect(helpOvalAct, SIGNAL(triggered()), this, SLOT(help_oval()));
    
    m_menu_help->addAction(helpLineAct);
    m_menu_help->addAction(helpRectangleAct);
    m_menu_help->addAction(helpOvalAct);

}

void PaintWindow::help_line() {
    const char* message =
    "Drawing a Line\n"
    "\n"
    "To draw a line, press the left mouse button to mark the beginning of the line.  Drag the mouse to the end of the line and release the button.";

    QMessageBox msgBox;
    msgBox.setText(QString(message));
    msgBox.exec();
}

void PaintWindow::help_rectangle() {
    const char* message =
    "Drawing a Rectangle\n"
    "\n"
    "To draw a rectangle, press the left mouse button to mark the origin (top-right corner) of the rectangle.  Drag the mouse to the end of the rectangle (bottom-left) then release the button.";

    QMessageBox msgBox;
    msgBox.setText(QString(message));
    msgBox.exec();
}

void PaintWindow::help_oval() {
    const char* message =
    "Drawing an oval\n"
    "\n"
    "To draw an oval, press the left mouse button to mark the beginning of the oval (top-right corner).  Drag the mouse to where you want to end (bottom-left) oval.  Then release the button.";

    QMessageBox msgBox;
    msgBox.setText(QString(message));
    msgBox.exec();
}

void PaintWindow::set_line() {
    m_canvas->set_mode(PaintCanvas::DRAW_LINE);
}

void PaintWindow::set_oval() {
    m_canvas->set_mode(PaintCanvas::DRAW_OVAL);
}

void PaintWindow::set_rect() {
    m_canvas->set_mode(PaintCanvas::DRAW_RECTANGLE);
}

void PaintWindow::clear(){
    m_canvas->clear();
}

void PaintWindow::set_pen_blue(){
     m_canvas->setPenColour(QColor("blue"));
}

void PaintWindow::set_pen_black(){
     m_canvas->setPenColour(QColor("black"));
}

void PaintWindow::set_pen_green(){
     m_canvas->setPenColour(QColor("green"));
}

void PaintWindow::set_pen_red(){
     m_canvas->setPenColour(QColor("red"));
}
