#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _scene(NULL)
{
    ui->setupUi(this);

    initSceneContent();
}

MainWindow::~MainWindow()
{
    if (_scene != NULL)
        delete _scene;

    delete ui;
}

void MainWindow::initSceneContent()
{
    _scene = new GScene;
//    _scene->setSceneRect(QRect(-500, -300, 1000, 600));
    ui->graphicsView->setScene(_scene);
    ui->graphicsView->setZoomEnabled(true);

    QRectF area(0, 0, 45, 20);

    for (int i = 2; i < 7; ++i)
    {
        _gItems << new GRectItem(area, QString("PIN %0").arg(i));
        _gItems.last()->setPos(-300, -400 + 100*i);
        _scene->addItem(_gItems.last());

        _gItems << new GRectItem(area, QString("PIN %0").arg(18 - i));
        _gItems.last()->setPos(300, -400 + 100*i);
        _scene->addItem(_gItems.last());
    }

    connect(ui->actionRotate_Left,  SIGNAL(triggered()), _scene, SLOT(rotateLeft()) );
    connect(ui->actionRotate_Right, SIGNAL(triggered()), _scene, SLOT(rotateRight()) );
    connect(ui->actionGroup_Items,  SIGNAL(triggered()), _scene, SLOT(groupItems()) );
    connect(ui->actionUngroup_Items, SIGNAL(triggered()), _scene, SLOT(ungroupItems()) );
    connect(ui->actionSet_Central_Transform, SIGNAL(triggered()), _scene, SLOT(setCentralTranform()) );
}
