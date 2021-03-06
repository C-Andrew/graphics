#include <QtWidgets>
#include <QGLFormat>
#include <iostream>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Tetrominoes on the Wall");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    m_game = new Game(10, 20);
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, SIGNAL(timeout()), this, SLOT(gameTick()));
    m_gameTimer->start(400);
    
    // m_menubar = new QMenuBar;
    m_viewer = new Viewer(glFormat, m_game, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);
    m_viewer->show();

    createActions();
    createMenu();
}

void AppWindow::keyPressEvent(QKeyEvent *event) {
    // Application Menu Controls
    if (event->key() == Qt::Key_Escape) {
        QCoreApplication::instance()->quit();
    } else if (event->key() == Qt::Key_Q) {
        QCoreApplication::instance()->quit();
    }

    // Game Controls
    else if (event->key() == Qt::Key_Left) {
        m_game->moveLeft();
    } else if (event->key() == Qt::Key_Right) {
        m_game->moveRight();
    } else if (event->key() == Qt::Key_Up) {
        m_game->rotateCCW();
    } else if (event->key() == Qt::Key_Down) {
        m_game->rotateCW();
    } else if (event->key() == Qt::Key_Space) {
        m_game->drop();
    } else {
        QWidget::keyPressEvent(event);
    }
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

    QAction* newGame = new QAction(tr("&New Game"), this);
    QList<QKeySequence> shortcuts;
    shortcuts.append(tr("N"));
    newGame->setShortcuts(shortcuts);
    shortcuts.clear();
    newGame->setStatusTip(tr("Starts a new game"));
    connect(newGame, SIGNAL(triggered()), this, SLOT(resetGame()));
    m_menu_actions.push_back(newGame);

    QAction* resetView = new QAction(tr("&Reset View"), this);
    shortcuts.append(tr("R"));
    resetView->setShortcuts(shortcuts);
    shortcuts.clear();
    resetView->setStatusTip(tr("Resets game view"));
    connect(resetView, SIGNAL(triggered()), this, SLOT(resetView()));
    m_menu_actions.push_back(resetView);

    // Draw Menu //
    QAction* wireFrame = new QAction(tr("&Wire-frame mode"), this);
    shortcuts.append(tr("W"));
    wireFrame->setShortcuts(shortcuts);
    shortcuts.clear();
    wireFrame->setStatusTip(tr("Sets draw mode to wire-frame"));
    connect(wireFrame, SIGNAL(triggered()), this, SLOT(wireFrame()));
    m_draw_actions.push_back(wireFrame);

    QAction* fillFace = new QAction(tr("&Fill-face mode"), this);
    shortcuts.append(tr("F"));
    fillFace->setShortcuts(shortcuts);
    shortcuts.clear();
    fillFace->setStatusTip(tr("Sets draw mode to fill-face mode"));
    connect(fillFace, SIGNAL(triggered()), this, SLOT(fillFace()));
    m_draw_actions.push_back(fillFace);

    QAction* multiColored = new QAction(tr("&Multicolored mode"), this);
    shortcuts.append(tr("M"));
    multiColored->setShortcuts(shortcuts);
    shortcuts.clear();
    multiColored->setStatusTip(tr("Sets draw mode to multicolored mode"));
    connect(multiColored, SIGNAL(triggered()), this, SLOT(multiColored()));
    m_draw_actions.push_back(multiColored);

    wireFrame->setCheckable(true);
    fillFace->setCheckable(true);
    multiColored->setCheckable(true);  

    QActionGroup* group = new QActionGroup(this);
    wireFrame->setActionGroup(group);
    fillFace->setActionGroup(group);
    multiColored->setActionGroup(group);
    fillFace->toggle();

    // Speed Menu //
    QAction* speedSlow = new QAction(tr("&Slow Speed Mode"), this);
    shortcuts.append(tr("1"));
    speedSlow->setShortcuts(shortcuts);
    shortcuts.clear();
    speedSlow->setStatusTip(tr("Sets difficulty to slow"));
    connect(speedSlow, SIGNAL(triggered()), this, SLOT(speedSlow()));
    m_speed_actions.push_back(speedSlow);

    QAction* speedMedium = new QAction(tr("&Medium Speed Mode"), this);
    shortcuts.append(tr("2"));
    speedMedium->setShortcuts(shortcuts);
    shortcuts.clear();
    speedMedium->setStatusTip(tr("Sets difficulty to medium"));
    connect(speedMedium, SIGNAL(triggered()), this, SLOT(speedMedium()));
    m_speed_actions.push_back(speedMedium);

    QAction* speedFast = new QAction(tr("&Fast Speed Mode"), this);
    shortcuts.append(tr("3"));
    speedFast->setShortcuts(shortcuts);
    shortcuts.clear();
    speedFast->setStatusTip(tr("Sets difficulty to fast"));
    connect(speedFast, SIGNAL(triggered()), this, SLOT(speedFast()));
    m_speed_actions.push_back(speedFast);

    speedSlow->setCheckable(true);
    speedMedium->setCheckable(true);
    speedFast->setCheckable(true);  

    QActionGroup* speedGroup = new QActionGroup(this);
    speedSlow->setActionGroup(speedGroup);
    speedMedium->setActionGroup(speedGroup);
    speedFast->setActionGroup(speedGroup);
    speedMedium->toggle();

}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_draw_app = menuBar()->addMenu(tr("&Draw Mode"));
    m_speed_app = menuBar()->addMenu(tr("&Speed Mode"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }

    for (auto& action : m_draw_actions) {
        m_draw_app->addAction(action);
    }

    for (auto& action : m_speed_actions) {
        m_speed_app->addAction(action);
    }
}

void AppWindow::gameTick() {
    m_game->tick();
}

void AppWindow::resetGame() {
    m_game->reset();
}

void AppWindow::resetView() {
    m_viewer->resetWorld();
}

void AppWindow::wireFrame() {
    m_viewer->setWireframeMode();
}

void AppWindow::fillFace() {
    m_viewer->setFillMode();
}

void AppWindow::multiColored() {
    m_viewer->setMulticolourMode();
}

void AppWindow::speedSlow(){
    m_gameTimer->stop();
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, SIGNAL(timeout()), this, SLOT(gameTick()));
    m_gameTimer->start(700);
}

void AppWindow::speedMedium(){
    m_gameTimer->stop();
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, SIGNAL(timeout()), this, SLOT(gameTick()));
    m_gameTimer->start(400);
}

void AppWindow::speedFast(){
    m_gameTimer->stop();
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, SIGNAL(timeout()), this, SLOT(gameTick()));
    m_gameTimer->start(100);
}