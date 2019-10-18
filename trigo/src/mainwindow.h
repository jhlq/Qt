#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "screenboard.h"

class DiagramScene;
//class DiagramItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ScreenBoard *screenboard;
    //double unitSize;

private:
    Ui::MainWindow *ui;

private slots:
    void addCircle(int x,int y,int player);
    void drawGrid();
    void placemoves();

private:
    DiagramScene *diagramScene;
};

#endif // MAINWINDOW_H
