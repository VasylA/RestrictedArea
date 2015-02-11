#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "gscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QGraphicsView *_view;
    GScene *_scene;

    QGraphicsRectItem *_allowedRect;

    QGraphicsTextItem *_textItem;
    QGraphicsRectItem *_rectItem;
    QGraphicsEllipseItem *_circleItem;
};

#endif // MAINWINDOW_H
