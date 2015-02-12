#ifndef GRECTITEM_H
#define GRECTITEM_H

#include <QGraphicsItem>

class GRectItem : public QGraphicsRectItem
{
    Q_OBJECT
public:
    GRectItem();

protected:
    QRect boundingRect();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:

};

#endif // GRECTITEM_H
