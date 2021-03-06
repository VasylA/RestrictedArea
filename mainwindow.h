#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gview.h"
#include "gscene.h"
#include "grectitem.h"

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
    void initSceneContent();

    Ui::MainWindow *ui;

    GScene *_scene;
    QList<GRectItem*> _gItems;
};

#endif // MAINWINDOW_H
