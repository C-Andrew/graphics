#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <vector>
#include "Viewer.hpp"
#include "game.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void resetGame();
    void resetView();
    void gameTick();
    void wireFrame();
    void fillFace();
    void multiColored();

private:
    void createActions();
    void createMenu();

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_draw_app;


    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_draw_actions;
    Viewer* m_viewer;
    Game* m_game;
    QTimer* m_gameTimer;

};

#endif