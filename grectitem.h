#ifndef GRECTITEM_H
#define GRECTITEM_H

#include <QGraphicsItem>
#include <QLinearGradient>
#include "draggableitem.h"
#include "glabel.h"

class ItemParameters
{
public:
    inline ItemParameters()
    {
        angel = 0;
        isValid = false;
    }
    QPointF pos;
    qreal angel;
    bool isValid;
};

class GRectItem : public QObject, public QGraphicsItem, public DraggableItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit GRectItem(QGraphicsItem *parent = 0);
    GRectItem(QRectF rect, QString name, QGraphicsItem *parent = 0);
    ~GRectItem();

    inline QRectF area() const { return _area; }
    void setArea(const QRectF &area);

    inline QString name() const { return _name; }
    void setName(const QString &name);

    qreal rotation() const;
    void setPhysicalRotation(double deltaAngle);

    GLabel* label() const;
    void setLabel(GLabel *newLabel);
    void removeLabel();

    void draggingStart();
    void draggingStop();

    void cacheTransform();
    void applyTransform();

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:

public slots:

private:
    void initStyle();

    QLinearGradient SelectedFrame;
    QLinearGradient SelectedBody;
    QLinearGradient Frame;
    QLinearGradient Body;
    QColor          Text;

    QRectF _area;
    QString _name;
    qreal _zValue;
    GLabel *_label;

    ItemParameters _parameters;
};

#endif // GRECTITEM_H
