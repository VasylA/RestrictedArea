#ifndef GSCENE_H
#define GSCENE_H

#include <QGraphicsScene>

class GScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GScene(QObject *parent = 0);
    ~GScene();

    QRectF allowedRect() const;
    void setAllowedRect(const QRectF &allowedRect);

    void fitSceneRectToVisibleItems();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:

public slots:

private:
    QRectF _allowedRect;
    QPointF _lastMousePosition;
};

#endif // GSCENE_H
