#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "diagramscene.h"
//#include "diagramitem.h"
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    unitSize=30;
    diagramScene = new DiagramScene();
    diagramScene->setSceneRect(QRect(0, 0, 500, 500));

    connect(diagramScene, SIGNAL(released(int,int)),this,SLOT(addCircle(int,int)));
    QGraphicsRectItem *rect=new QGraphicsRectItem();
    rect->setRect(0,0,100,100);
    diagramScene->addItem(rect);
    QGraphicsEllipseItem *c=new QGraphicsEllipseItem();
    c->setRect(0,0,200,150);
    diagramScene->addItem(c);

    QGraphicsView *view =this->findChild<QGraphicsView*>("graphicsView");
    view->setScene(diagramScene);
    addCircle(10,20);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addCircle(int x, int y)
{
    QGraphicsEllipseItem *circle=new QGraphicsEllipseItem();
    circle->setRect(x,y,unitSize,unitSize);
    circle->setBrush(Qt::red);
    diagramScene->addItem(circle);
}
