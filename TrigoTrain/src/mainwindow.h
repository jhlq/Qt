#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DiagramScene;
class NewGameDialog;
class ScreenBoard;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //double unitSize;

private:
    Ui::MainWindow *ui;

private slots:
    void addCircle(int x,int y,int player);
    void drawGrid();
    void placemoves();
    void updatescore();
    void newGameButtonClicked();
    void makeNewGame(int sideLength,int unitSize);
    void saveTrainingExample();

private:
    DiagramScene *diagramScene;
    ScreenBoard *screenboard;
    NewGameDialog *newGameDialog;
};

#endif // MAINWINDOW_H
