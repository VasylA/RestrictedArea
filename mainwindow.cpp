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

//    QRectF rect = _scene->allowedRect();
//    QPen borderPen(Qt::red, 3.0);

//    _scene->addLine(QLineF(rect.topLeft(), rect.topRight()), borderPen);          //Top line
//    _scene->addLine(QLineF(rect.topRight(), rect.bottomRight()), borderPen);      //Right line
//    _scene->addLine(QLineF(rect.bottomRight(), rect.bottomLeft()), borderPen);    //Bottom line
//    _scene->addLine(QLineF(rect.bottomLeft(), rect.topLeft()), borderPen);        //Left line

//    _textItem = new QGraphicsTextItem("Some text");
//    _textItem->setFont(QFont("Times", 15, QFont::Black) );
//    _textItem->setPos(0, 0);
//    _textItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
//    _scene->addItem(_textItem);

//    _rectItem = new QGraphicsRectItem(QRect(200, -200, 70, 70));
//    _rectItem->setPen(QPen(Qt::black, 2.0));
//    _rectItem->setBrush(QBrush(Qt::blue));
//    _rectItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
//    _scene->addItem(_rectItem);

//    _circleItem = new QGraphicsEllipseItem(100, 100, 70, 70);
//    _circleItem->setPen(QPen(Qt::black, 2.0));
//    _circleItem->setBrush(QBrush(Qt::green));
//    _circleItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
//    _scene->addItem(_circleItem);

    QRectF area(0, 0, 45, 20);

    for (int i = 2; i < 7; ++i)
    {
        _gItems << new GRectItem(area, QString("PIN %0").arg(i));
        _gItems.last()->setPos(-300, -400 + 100*i);
        _gItems.last()->setLabel(new GLabel(QString("Label %0").arg(i)) );
        _scene->addItem(_gItems.last());

        _gItems << new GRectItem(area, QString("PIN %0").arg(18 - i));
        _gItems.last()->setPos(300, -400 + 100*i);
        _gItems.last()->setLabel(new GLabel(QString("Label %0").arg(18 - i)) );
        _scene->addItem(_gItems.last());
    }

    _gItems.at(0)->setRotation(-35);

    _gItems.at(2)->setRotation(-135);
    _gItems.at(2)->label()->setRotation(45);

    _gItems.at(4)->setRotation(360);
    _gItems.at(4)->label()->setRotation(-180);

    _gItems.at(6)->setRotation(-75);
    _gItems.at(6)->label()->setRotation(315);

    _gItems.at(8)->setRotation(105);
    _gItems.at(8)->label()->setRotation(-30);


    _gItems.at(1)->setPhysicalRotation(-35);

    _gItems.at(3)->setPhysicalRotation(-135);
    _gItems.at(3)->label()->setPhysicalRotation(45);

    _gItems.at(5)->setPhysicalRotation(360);
    _gItems.at(5)->label()->setPhysicalRotation(-180);

    _gItems.at(7)->setPhysicalRotation(-75);
    _gItems.at(7)->label()->setPhysicalRotation(315);

    _gItems.at(9)->setPhysicalRotation(105);
    _gItems.at(9)->label()->setPhysicalRotation(-30);



//    _gItems << new GRectItem(area, "PIN 1");
//    _gItems.last()->setPos(-900, 0);
//    _gItems.last()->setLabel(new GLabel("Label 1") );
//    _scene->addItem(_gItems.last());



    GLabel *separateLabel1 = new GLabel("Separate Label #1");
    separateLabel1->setPos(-30, -30);
    separateLabel1->setPhysicalRotation(-30);
    GLabel *separateLabel2 = new GLabel("Separate Label #2");
    separateLabel2->setPos(30, 30);
    separateLabel2->setTransformOriginPoint(separateLabel2->boundingRect().center());
    separateLabel2->setRotation(30);
    _scene->addItem(separateLabel1);
    _scene->addItem(separateLabel2);

    connect(ui->actionRotate_Left,  SIGNAL(triggered()), _scene, SLOT(rotateLeft()) );
    connect(ui->actionRotate_Right, SIGNAL(triggered()), _scene, SLOT(rotateRight()) );
}
