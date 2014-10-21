#include <QtWidgets>
#include <iostream>
#include "AppWindow.hpp"

Viewer* AppWindow::m_viewer = NULL;

AppWindow::AppWindow(SceneNode* root) {
    setWindowTitle("488 Assignment Two");

    m_root = root;
    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    m_viewer = new Viewer(glFormat, root, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createActions();
    createMenu();
}

void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions ve0ctor
    QList<QKeySequence> shortcuts;

    QAction* quitAct = new QAction(tr("&Quit"), this);
    m_menu_actions.push_back(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P); 
    quitAct->setShortcuts(QKeySequence::Quit);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    QAction* resetPosition = new QAction(tr("&Reset Position"), this);
    shortcuts.append(tr("I"));
    resetPosition->setShortcuts(shortcuts);
    shortcuts.clear();
    resetPosition->setStatusTip(tr("Resets Position"));
    connect(resetPosition, SIGNAL(triggered()), this, SLOT(resetPosition())); 
    m_menu_actions.push_back(resetPosition);

    QAction* resetOrientation = new QAction(tr("&Reset Orientation"), this);
    shortcuts.append(tr("O"));
    resetOrientation->setShortcuts(shortcuts);
    shortcuts.clear();
    resetOrientation->setStatusTip(tr("Resets Orientation"));
    connect(resetOrientation, SIGNAL(triggered()), this, SLOT(resetOrientation()));
    m_menu_actions.push_back(resetOrientation);

    QAction* resetJoints = new QAction(tr("&Reset Joints"), this);
    shortcuts.append(tr("N"));
    resetJoints->setShortcuts(shortcuts);
    shortcuts.clear();
    resetJoints->setStatusTip(tr("Resets Joints"));
    connect(resetJoints, SIGNAL(triggered()), this, SLOT(resetJoints()));
    m_menu_actions.push_back(resetJoints);

    QAction* resetAll= new QAction(tr("&Reset All"), this);
    shortcuts.append(tr("A"));
    resetAll->setShortcuts(shortcuts);
    shortcuts.clear();
    resetAll->setStatusTip(tr("Resets All"));
    connect(resetAll, SIGNAL(triggered()), this, SLOT(resetAll()));
    m_menu_actions.push_back(resetAll);

    ///////////////
    // Mode Menu //
    ///////////////
    QAction* posOri= new QAction(tr("&Position/Orientation"), this);
    shortcuts.append(tr("P"));
    posOri->setShortcuts(shortcuts);
    shortcuts.clear();
    posOri->setStatusTip(tr("Position/Orientation"));
    connect(posOri, SIGNAL(triggered()), this, SLOT(toggleOrientation()));
    m_mode_actions.push_back(posOri);

    QAction* joints= new QAction(tr("&Joints"), this);
    shortcuts.append(tr("J"));
    joints->setShortcuts(shortcuts);
    shortcuts.clear();
    joints->setStatusTip(tr("Joints"));
    connect(joints, SIGNAL(triggered()), this, SLOT(togglePicking())); 
    m_mode_actions.push_back(joints);

    posOri->setCheckable(true);
    joints->setCheckable(true);


    QActionGroup* group = new QActionGroup(this);
    posOri->setActionGroup(group);
    joints->setActionGroup(group);

    posOri->toggle();

    ///////////////
    // Edit Menu //
    ///////////////
    QAction* undo= new QAction(tr("&Undo"), this);
    shortcuts.append(tr("U"));
    undo->setShortcuts(shortcuts);
    shortcuts.clear();
    undo->setStatusTip(tr("Undo"));
    connect(undo, SIGNAL(triggered()), this, SLOT(undo()));
    m_edit_actions.push_back(undo);

    QAction* redo = new QAction(tr("&redo"), this);
    shortcuts.append(tr("R"));
    redo->setShortcuts(shortcuts);
    shortcuts.clear();
    undo->setStatusTip(tr("Redo"));
    connect(undo, SIGNAL(triggered()), this, SLOT(redo()));
    m_edit_actions.push_back(redo);

    //////////////////
    // Options Menu //
    //////////////////
    QAction* circle = new QAction(tr("&Circle"), this);
    shortcuts.append(tr("C"));
    circle->setShortcuts(shortcuts);
    shortcuts.clear();
    circle->setStatusTip(tr("Circle"));
    connect(circle, SIGNAL(triggered()), this, SLOT(show_trackball()));
    m_options_actions.push_back(circle);

    QAction* zbuf = new QAction(tr("&Z-Buffer"), this);
    shortcuts.append(tr("Z"));
    zbuf->setShortcuts(shortcuts);
    shortcuts.clear();
    zbuf->setStatusTip(tr("Z-Buffer"));
    connect(zbuf, SIGNAL(triggered()), this, SLOT(toggleZBuffer()));
    m_options_actions.push_back(zbuf);

    QAction* backCull = new QAction(tr("&Backface Culling"), this);
    shortcuts.append(tr("B"));
    backCull->setShortcuts(shortcuts);
    shortcuts.clear();
    backCull->setStatusTip(tr("Backface Culling"));
    connect(backCull, SIGNAL(triggered()), this, SLOT(toggleBackfaceCull()));
    m_options_actions.push_back(backCull);

    QAction* frontCull = new QAction(tr("&Frontface Culling"), this);
    shortcuts.append(tr("F"));
    frontCull->setShortcuts(shortcuts);
    shortcuts.clear();
    frontCull->setStatusTip(tr("Frontface Culling"));
    connect(frontCull, SIGNAL(triggered()), this, SLOT(toggleFrontfaceCull())); 
    m_options_actions.push_back(frontCull);

    circle->setCheckable(true);
    frontCull->setCheckable(true);
    backCull->setCheckable(true);
    zbuf->setCheckable(true);

    // QActionGroup* group = new QActionGroup(this);
    // circle->setActionGroup(group);
    // frontCull->setActionGroup(group);
    // backCull->setActionGroup(group);
    // zbuf->setActionGroup(group);
}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_edit_app = menuBar()->addMenu(tr("&Edit"));
    m_mode_app = menuBar()->addMenu(tr("&Mode"));
    m_options_app = menuBar()->addMenu(tr("&Options"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
    for (auto& action : m_edit_actions) {
        m_edit_app->addAction(action);
    }
    for (auto& action : m_mode_actions) {
        m_mode_app->addAction(action);
    }
    for (auto& action : m_options_actions) {
        m_options_app->addAction(action);
    }

}

void AppWindow::resetAll(){
    m_viewer->resetAll();
}

void AppWindow::resetPosition(){
    m_viewer->resetPosition();
}

void AppWindow::resetJoints(){
    m_viewer->resetJoints();
}

void AppWindow::resetOrientation(){
    m_viewer->resetOrientation();
}

void AppWindow::show_trackball(){
    m_viewer->enableTrackballCircle();
}

void AppWindow::toggleBackfaceCull(){
    m_viewer->setbackfacecull();
}

void AppWindow::toggleFrontfaceCull(){
    m_viewer->setfrontfacecull();
}

void AppWindow::toggleZBuffer(){
    m_viewer->setzbuffer();
}

void AppWindow::togglePicking(){
    m_viewer->enablePicking();
    //disbale joints
}

void AppWindow::toggleOrientation(){
    m_viewer->enableOrientation();
    //disbale joints
}

void AppWindow::undo(){
}
void AppWindow::redo(){
}
