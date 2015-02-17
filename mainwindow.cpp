#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initSceneContent();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSceneContent()
{
    _scene = new GScene;
    _scene->setSceneRect(QRect(-500, -300, 1000, 600));
    ui->graphicsView->setScene(_scene);

    QRectF rect = _scene->allowedRect();
    QPen borderPen(Qt::red, 3.0);

    _scene->addLine(QLineF(rect.topLeft(), rect.topRight()), borderPen);          //Top line
    _scene->addLine(QLineF(rect.topRight(), rect.bottomRight()), borderPen);      //Right line
    _scene->addLine(QLineF(rect.bottomRight(), rect.bottomLeft()), borderPen);    //Bottom line
    _scene->addLine(QLineF(rect.bottomLeft(), rect.topLeft()), borderPen);        //Left line

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

    QRectF area(0, 0, 40, 20);

    for (int i = 2; i < 7; ++i)
    {
        _gItems << new GRectItem(area, QString("PIN %0").arg(i));
        _gItems.last()->setPos(-300, -400 + 100*i);
        _scene->addItem(_gItems.last());

        _gItems << new GRectItem(area, QString("PIN %0").arg(18 - i));
        _gItems.last()->setPos(300, -400 + 100*i);
        _scene->addItem(_gItems.last());
    }
}
