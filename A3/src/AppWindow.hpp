#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"
#include "scene_lua.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(SceneNode* root);
    SceneNode* m_root;
    static Viewer* m_viewer;

private slots:
    void resetAll();
    void resetPosition();
    void resetJoints();
    void resetOrientation();
    void show_trackball();
    void toggleFrontfaceCull();
    void toggleBackfaceCull();
    void toggleZBuffer();
    void togglePicking();
    void toggleOrientation();
    void undo();
    void redo();
private:
    void createActions();
    void createMenu();

    // Each menu itself
    QMenu* m_menu_app; 
    QMenu* m_edit_app;
    QMenu* m_mode_app;
    QMenu* m_options_app;

    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_edit_actions;
	std::vector<QAction*> m_mode_actions;
	std::vector<QAction*> m_options_actions;

};

#endif