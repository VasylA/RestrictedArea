#ifndef GRECTITEM_H
#define GRECTITEM_H

#include <QGraphicsItem>
#include <QLinearGradient>

class GRectItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit GRectItem(QGraphicsItem *parent = 0);
    GRectItem(QRectF rect, QString name, QGraphicsItem *parent = 0);

    inline QRectF area() const { return _area; }
    void setArea(const QRectF &area);

    inline QString name() const { return _name; }
    void setName(const QString &name);

protected:
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

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

};

#endif // GRECTITEM_H
