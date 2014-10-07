#include <QtWidgets>
#include <iostream>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Assignment Two");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    // layout->setContentsMargins(QMargins(0,0,0,0));
    // m_menubar = new QMenuBar;
    messageBar = new QStatusBar();
    fieldBar = new QStatusBar();

    this->setStatusBar(fieldBar);
    m_viewer = new Viewer(glFormat, messageBar, fieldBar, this);
    layout->addWidget(m_viewer);

    // layout->addWidget(messageBar);
    // layout->addWidget(fieldBar);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);


    createActions();
    createMenu();
}

void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector 
    QAction* quitAct = new QAction(tr("&Quit"), this);
    m_menu_actions.push_back(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P); 
    quitAct->setShortcuts(QKeySequence::Quit);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    // Added menus/options
    QList<QKeySequence> shortcuts;

    QAction* resetView = new QAction(tr("&Reset View"), this);
    shortcuts.append(tr("A"));
    resetView->setShortcuts(shortcuts);
    shortcuts.clear();
    resetView->setStatusTip(tr("Resets game view"));
    connect(resetView, SIGNAL(triggered()), this, SLOT(resetView()));
    m_menu_actions.push_back(resetView);

    QAction* viewTranslate = new QAction(tr("&Translate View"), this);
    shortcuts.append(tr("N"));
    viewTranslate->setShortcuts(shortcuts);
    shortcuts.clear();
    viewTranslate->setStatusTip(tr("Translate View"));
    connect(viewTranslate, SIGNAL(triggered()), this, SLOT(viewTranslateMode()));
    m_mode_actions.push_back(viewTranslate);

    QAction* viewRotate = new QAction(tr("&Rotate View"), this);
    shortcuts.append(tr("O"));
    viewRotate->setShortcuts(shortcuts);
    shortcuts.clear();
    viewRotate->setStatusTip(tr("Rotate View"));
    connect(viewRotate, SIGNAL(triggered()), this, SLOT(viewRotateMode()));
    m_mode_actions.push_back(viewRotate);

    QAction* viewPerspective = new QAction(tr("&Perspective View"), this);
    shortcuts.append(tr("P"));
    viewPerspective->setShortcuts(shortcuts);
    shortcuts.clear();
    viewPerspective->setStatusTip(tr("Perspective View"));
    connect(viewPerspective, SIGNAL(triggered()), this, SLOT(viewPerspectiveMode()));
    m_mode_actions.push_back(viewPerspective);

    QAction* modelRotate = new QAction(tr("&Rotate Model"), this);
    shortcuts.append(tr("R"));
    modelRotate->setShortcuts(shortcuts);
    shortcuts.clear();
    modelRotate->setStatusTip(tr("Resets game view"));
    connect(modelRotate, SIGNAL(triggered()), this, SLOT(modelRotateMode()));
    m_mode_actions.push_back(modelRotate);

    QAction* modelScale = new QAction(tr("&Scale Model"), this);
    shortcuts.append(tr("S"));
    modelScale->setShortcuts(shortcuts);
    shortcuts.clear();
    modelScale->setStatusTip(tr("Scale Model"));
    connect(modelScale, SIGNAL(triggered()), this, SLOT(modelScaleMode()));
    m_mode_actions.push_back(modelScale);

    QAction* modelTranslate = new QAction(tr("&Translate Model"), this);
    shortcuts.append(tr("T"));
    modelTranslate->setShortcuts(shortcuts);
    shortcuts.clear();
    modelTranslate->setStatusTip(tr("Translate Model"));
    connect(modelTranslate, SIGNAL(triggered()), this, SLOT(modelTranslateMode()));
    m_mode_actions.push_back(modelTranslate);


    QAction* viewportMode = new QAction(tr("&Viewport Mode"), this);
    shortcuts.append(tr("V"));
    viewportMode->setShortcuts(shortcuts);
    shortcuts.clear();
    viewportMode->setStatusTip(tr("Translate View"));
    connect(viewportMode, SIGNAL(triggered()), this, SLOT(viewportMode()));
    m_mode_actions.push_back(viewportMode);

    modelRotate->setCheckable(true);
    modelScale->setCheckable(true);
    modelTranslate->setCheckable(true);
    viewRotate->setCheckable(true);
    viewPerspective->setCheckable(true);
    viewTranslate->setCheckable(true);
    viewportMode->setCheckable(true);

    QActionGroup* group = new QActionGroup(this);
    modelRotate->setActionGroup(group);
    modelScale->setActionGroup(group);
    modelTranslate->setActionGroup(group);
    viewRotate->setActionGroup(group);
    viewPerspective->setActionGroup(group);
    viewTranslate->setActionGroup(group);
    viewportMode->setActionGroup(group);
    modelRotate->toggle();

}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }

    m_mode_app = menuBar()->addMenu(tr("&Mode"));

    for (auto& action : m_mode_actions) {
        m_mode_app->addAction(action);
    }
}

void AppWindow::resetView(){
    m_viewer->reset_view();
}

void AppWindow::modelRotateMode(){
    m_viewer->setRotateModel();
}

void AppWindow::modelScaleMode(){
    m_viewer->setScaleModel();
}

void AppWindow::modelTranslateMode(){
    m_viewer->setTranslateModel();
}

void AppWindow::viewRotateMode(){
    m_viewer->setRotateView();
}
void AppWindow::viewPerspectiveMode(){
    m_viewer->setPerspectiveView();
}
void AppWindow::viewTranslateMode(){
    m_viewer->setTranslateView();
}
void AppWindow::viewportMode(){
    m_viewer->setViewportMode();
}

