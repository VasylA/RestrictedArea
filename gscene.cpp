#include "gscene.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

GScene::GScene(QObject *parent) :
    QGraphicsScene(parent),
    _allowedRect(QRect(-800, -400, 1600, 800))
{
}

GScene::~GScene()
{
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

        QPointF prevMousePos = _lastMousePosition;
        QPointF newMousePos = event->scenePos();

        QPointF offset = newMousePos - prevMousePos;

        //Rectangle of selected items shifted at offset
        QRectF translatedRect = selectedItemsRect.translated(offset);

        if (!_allowedRect.contains(translatedRect) )
        {
            qreal dx;
            if (translatedRect.left() < _allowedRect.left())
                dx = _allowedRect.left() - selectedItemsRect.left();
            else
                if (translatedRect.right() > _allowedRect.right())
                    dx = _allowedRect.right() - selectedItemsRect.right();
                else
                    dx = offset.x();

            qreal dy;
            if (translatedRect.top() < _allowedRect.top())
                dy = _allowedRect.top() - selectedItemsRect.top();
            else
                if (translatedRect.bottom() > _allowedRect.bottom())
                    dy = _allowedRect.bottom() - selectedItemsRect.bottom();
                else
                    dy = offset.y();

            foreach (QGraphicsItem* item, selectedElements)
            {
                item->moveBy(dx, dy);
            }
            selectedItemsRect.translate(dx, dy);

            _lastMousePosition = _lastMousePosition + QPointF(dx, dy);

            return;
        }
    }

    _lastMousePosition = event->scenePos();

    QGraphicsScene::mouseMoveEvent(event);
}

