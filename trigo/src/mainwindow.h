#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DiagramScene;
class DiagramItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    double unitSize;

private:
    Ui::MainWindow *ui;

private slots:
    void addCircle(int x,int y);

private:
    DiagramScene *diagramScene;
};

#endif // MAINWINDOW_H
