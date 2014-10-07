#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QtWidgets>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();

private slots:
	void resetView();
	void modelRotateMode();
	void modelScaleMode();
	void modelTranslateMode();
	void viewRotateMode();
	void viewPerspectiveMode();
	void viewTranslateMode();
    void viewportMode();

private:
    void createActions();
    void createMenu();

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_mode_app;
    QStatusBar* messageBar;
    QStatusBar* fieldBar;
    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_mode_actions;
    Viewer* m_viewer;
};

#endif