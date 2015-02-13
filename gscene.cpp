#include "gscene.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

GScene::GScene(QObject *parent) :
    QGraphicsScene(parent),
    _allowedRect(QRect(-800, -400, 1600, 800))
{
    _currentMousePosition = new QPointF;
}

QRectF GScene::allowedRect() const
{
    return _allowedRect;
}

void GScene::setAllowedRect(const QRectF &allowedRect)
{
    _allowedRect = allowedRect;
}

void GScene::fitSceneRectToVisibleItems()
{
    if(items().count() > 0)
    {
        setSceneRect(itemsBoundingRect().x() - 100,
                     itemsBoundingRect().y() - 100,
                     itemsBoundingRect().width() + 200,
                     itemsBoundingRect().height() + 200);
    }
}

void GScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    QGraphicsScene::mousePressEvent(event);
}

void GScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    QGraphicsScene::mouseReleaseEvent(event);

    fitSceneRectToVisibleItems();
}

void GScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    QGraphicsScene::mouseDoubleClickEvent(event);
}

void GScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);
    if (mouseGrabberItem() != NULL)
    {
        QRectF selectedItemsRect;
        QList<QGraphicsItem*> selectedElements = selectedItems();
        foreach (QGraphicsItem* item, selectedElements)
        {
            selectedItemsRect |= item->mapRectToScene(item->boundingRect() | item->childrenBoundingRect());
        }

        QPointF prevMousePos = *_currentMousePosition;
        QPointF newMousePos = event->scenePos();

        QPointF offset = newMousePos - prevMousePos;

        if (!_allowedRect.contains(selectedItemsRect.translated(offset)) )
        {
            return;
        }
    }

    if (_currentMousePosition != NULL)
        *_currentMousePosition = event->scenePos();

    QGraphicsScene::mouseMoveEvent(event);
}

