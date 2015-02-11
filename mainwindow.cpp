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

    _view = new QGraphicsView(this);
    this->setCentralWidget(_view);

    _scene = new GScene;
    _view->setScene(_scene);

    _allowedRect = new QGraphicsRectItem(QRect(-500, -300, 1000, 600));
    _allowedRect->setPen(QPen(Qt::red, 5.0));
    _scene->addItem(_allowedRect);

    _textItem = new QGraphicsTextItem("Some text");
    _textItem->setFont(QFont("Times", 20, QFont::Black) );
    _textItem->setPos(-200, 200);
    _textItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    _scene->addItem(_textItem);

    _rectItem = new QGraphicsRectItem(QRect(-200, -200, 100, 100));
    _rectItem->setPen(QPen(Qt::black, 2.0));
    _rectItem->setBrush(QBrush(Qt::blue));
    _rectItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    _scene->addItem(_rectItem);

    _circleItem = new QGraphicsEllipseItem(100, 100, 100, 100);
    _circleItem->setPen(QPen(Qt::black, 2.0));
    _circleItem->setBrush(QBrush(Qt::green));
    _circleItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    _scene->addItem(_circleItem);
}

MainWindow::~MainWindow()
{
    delete ui;
}
