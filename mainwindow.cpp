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

    _scene = new GScene;
    _scene->setSceneRect(QRect(-500, -300, 1000, 600));
    ui->graphicsView->setScene(_scene);

    _allowedRect = new QGraphicsRectItem(QRect(-500, -300, 1000, 600));
    _allowedRect->setPen(QPen(Qt::red, 5.0));
    _scene->addItem(_allowedRect);

    _textItem = new QGraphicsTextItem("Some text");
    _textItem->setFont(QFont("Times", 15, QFont::Black) );
    _textItem->setPos(0, 0);
    _textItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    _scene->addItem(_textItem);

    _rectItem = new QGraphicsRectItem(QRect(200, -200, 70, 70));
    _rectItem->setPen(QPen(Qt::black, 2.0));
    _rectItem->setBrush(QBrush(Qt::blue));
    _rectItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    _scene->addItem(_rectItem);

    _circleItem = new QGraphicsEllipseItem(100, 100, 70, 70);
    _circleItem->setPen(QPen(Qt::black, 2.0));
    _circleItem->setBrush(QBrush(Qt::green));
    _circleItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    _scene->addItem(_circleItem);

    QRectF area(0, 0, 40, 20);

    for (int i = 2; i < 7; ++i)
    {
        _gItems << new GRectItem(area, QString("PIN %0").arg(i));
        _gItems.last()->setPos(-200, -400 + 100*i);
        _scene->addItem(_gItems.last());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
