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
    m_gameTimer->start(10000/30);
    
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
    }
    else if (event->key() == Qt::Key_N) {
        m_game->reset();
    }
    else if (event->key() == Qt::Key_R) {
       m_viewer->resetWorld();
    }
    else if (event->key() == Qt::Key_Q) {
        QCoreApplication::instance()->quit();
    }

    // Draw Mode Controls
    else if (event->key() == Qt::Key_W) {
        m_viewer->setWireframeMode();
    }
    else if (event->key() == Qt::Key_F) {
       m_viewer->setFillMode();
    }
    else if (event->key() == Qt::Key_M) {
      m_viewer->setMulticolourMode();
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
}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
}

void AppWindow::gameTick() {
    m_game->tick();
}
