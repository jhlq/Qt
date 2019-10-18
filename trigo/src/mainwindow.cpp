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

    screenboard=new ScreenBoard(7,30,0,0);
    diagramScene = new DiagramScene();
    diagramScene->setSceneRect(QRect(0, 0, 600, 500));

    //connect(diagramScene, SIGNAL(released(int,int)),this,SLOT(addCircle(int,int)));
    connect(diagramScene, SIGNAL(released(int,int)),this->screenboard,SLOT(clickevent(int,int)));
    connect(screenboard, SIGNAL(placedmove()),this,SLOT(placemoves()));
    /*QGraphicsRectItem *rect=new QGraphicsRectItem();
    rect->setRect(0,0,100,100);
    diagramScene->addItem(rect);
    QGraphicsEllipseItem *c=new QGraphicsEllipseItem();
    c->setRect(0,0,200,150);
    diagramScene->addItem(c);
    QGraphicsLineItem *l=new QGraphicsLineItem(1,1,300,600);
    diagramScene->addItem(l);*/

    QGraphicsView *view =this->findChild<QGraphicsView*>("graphicsView");
    view->setScene(diagramScene);
    drawGrid();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addCircle(int x,int y,int player)
{
    QGraphicsEllipseItem *circle=new QGraphicsEllipseItem();
    double us=screenboard->unitSize;
    circle->setRect(x-us/2,y-us/2,us,us);
    if (player==1){
        circle->setBrush(Qt::green);
    } else if (player==2){
        circle->setBrush(Qt::blue);
    } else {
        circle->setBrush(Qt::red);
    }
    diagramScene->addItem(circle);
}
void MainWindow::drawGrid(){
    int ylen=screenboard->triangles.size();
    for (int yt = 0; yt < ylen; yt++){
        int xlen=screenboard->triangles[yt].size();
        for (int xt=0;xt<xlen;xt++){
            ScreenTriangle tri=screenboard->triangles[yt][xt];
            std::vector<Triangle> adj=screenboard->board.tg.adjacent(tri.x,tri.y);
            for (int a=0;a<adj.size();a++){
                //g.drawLine(tri.pixX,tri.pixY,adj.get(a).pixX,adj.get(a).pixY);
                ScreenTriangle adja=screenboard->triangles[adj[a].y][adj[a].x];
                QGraphicsLineItem *l=new QGraphicsLineItem(tri.pixX,tri.pixY,adja.pixX,adja.pixY);
                diagramScene->addItem(l);
            }
        }
    }
}
void MainWindow::placemoves(){
    diagramScene->clear();
    drawGrid();
    int ylen=screenboard->triangles.size();
    for (int yt = 0; yt < ylen; yt++){
        int xlen=screenboard->triangles[yt].size();
        for (int xt=0;xt<xlen;xt++){
            ScreenTriangle tri=screenboard->triangles[yt][xt];
            Triangle t=screenboard->board.tg.get(tri.x,tri.y);
            if (t.player>0){
                addCircle(tri.pixX,tri.pixY,t.player);
            }
        }
    }
}
