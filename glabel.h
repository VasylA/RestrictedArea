#ifndef GLABEL_H
#define GLABEL_H

#include <QGraphicsTextItem>
#include <QPen>
#include "draggableitem.h"

class GLabel : public QGraphicsTextItem, public DraggableItem
{
    Q_OBJECT

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

public:
    GLabel(QString text, QColor col = QColor(Qt::yellow), QColor bcol = QColor(20,20,20,100), QGraphicsItem *parent = NULL);

    void setPhysicalRotation(double deltaAngle);

    void draggingStart();
    void draggingStop();

signals:
    void textChanged(const QString &newText);

private:
    QPen _pen;
    QBrush _brush;
    QBrush _selectedBrush;
    QColor _textColor;
    qreal _zValue;
};

#endif // GLABEL_H
