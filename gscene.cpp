#include "gscene.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include "draggableitem.h"

#include <QDebug>

GScene::GScene(QObject *parent) :
    QGraphicsScene(parent),
    _allowedRect(QRect(-800, -400, 1600, 800)),
    _selectedItemsRect(QRectF()),
    _dragState(DS_NoDrag)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

    qDebug() << __PRETTY_FUNCTION__ << " DS_NoDrag";
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

    if(event)
    {
        if ( (mouseGrabberItem() != NULL) || (itemAt(event->scenePos()) != NULL) )
        {
            if ( (event->button() == Qt::LeftButton) && (_dragState == DS_NoDrag) )
            {
                _dragState = DS_CanDrag;
                qDebug() << __PRETTY_FUNCTION__ << " DS_NoDrag > DS_CanDrag";
            }
            else
            {
                restoreItems();
                return;
            }
        }

        QGraphicsScene::mousePressEvent(event);
    }
}

void GScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    if (event == NULL)
        return;

    if (_dragState == DS_Drag)
    {
        DraggableItem *item = dynamic_cast<DraggableItem*>(itemAt(event->scenePos()));
        if (item != NULL)
            item->draggingStop();

        QList<QGraphicsItem*> selectedElements = selectedItems();
        for(int i = 0; i < selectedElements.count(); i++)
        {
            item = dynamic_cast<DraggableItem*>(selectedElements.at(i));
            if (item != NULL)
                item->draggingStop();
        }
        _selectedItemsRect = calculateSelectedItemsRect();
    }
    if (_dragState != DS_NoDrag)
    {
        _dragState = DS_NoDrag;
        qDebug() << __PRETTY_FUNCTION__ << " ~        > DS_NoDrag";
    }

    QGraphicsScene::mouseReleaseEvent(event);
    fitSceneRectToVisibleItems();
}

void GScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    if (_dragState == DS_NoDrag)
    {
        if ( (event->button() == Qt::LeftButton) && (itemAt(event->scenePos()) != NULL) )
        {
            _dragState = DS_CanDrag;
            qDebug() << __PRETTY_FUNCTION__ << " DS_NoDrag > DS_CanDrag";
        }
    }

    QGraphicsScene::mouseDoubleClickEvent(event);
}

void GScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (event == NULL)
        return;

    DraggableItem* grabberItem = dynamic_cast<DraggableItem*>(mouseGrabberItem());
    if (grabberItem)
    {
        if (_dragState == DS_NoDrag)
        {
            qDebug() << __PRETTY_FUNCTION__ << " Shit happened!";
            return;
        }

        if (_dragState == DS_CanDrag)
        {
            _dragState = DS_Drag;
            qDebug() << __PRETTY_FUNCTION__ << " DS_CanDrag > DS_Drag";

            grabberItem->draggingStart();

            QList<QGraphicsItem*> selectedElements = selectedItems();
            for (int i = 0; i < selectedElements.count(); i++)
            {
                DraggableItem *item = dynamic_cast<DraggableItem*>(selectedElements.at(i));
                if (item != NULL)
                    item->draggingStart();
            }
        }

        if (_dragState != DS_NoDrag)
        {
            QPointF prevMousePos = _lastMousePosition;
            QPointF newMousePos = event->scenePos();

            QPointF offset = newMousePos - prevMousePos;

            if (!tryNaturalMoveByOffset(offset))
                return;
        }
    }

    _lastMousePosition = event->scenePos();

    QGraphicsScene::mouseMoveEvent(event);
}

void GScene::keyPressEvent(QKeyEvent *event)
{
    static const int x1 = 1;
    static const int x10 = 10;
    QPointF offset;
    int x = - 1;

    if ((event->modifiers() == Qt::AltModifier) ||
        (event->modifiers() == (Qt::AltModifier | Qt::KeypadModifier)))
            x = x1;
    else if ((event->modifiers() == Qt::ControlModifier) ||
            (event->modifiers() == (Qt::ControlModifier | Qt::KeypadModifier)))
                x = x10;

    if ( x >= 0 )
    {
        if ( event->key() == Qt::Key_Up )
            offset = QPointF(0, -x);
        else if ( event->key() == Qt::Key_Down )
            offset = QPointF(0, x);
        else if ( event->key() == Qt::Key_Left )
            offset = QPointF(-x, 0);
        else if ( event->key() == Qt::Key_Right )
            offset = QPointF(x, 0);

        if (!offset.isNull())
        {
            if (!_allowedRect.contains(_selectedItemsRect.translated(offset)))
                return;

            _selectedItemsRect.translate(offset);
            QList<QGraphicsItem*> selectedElements = selectedItems();
            foreach (QGraphicsItem *i, selectedElements)
            {
                if ( (i->parentItem() == NULL) )
                    i->moveBy(offset.x(), offset.y());

                if ( (i->parentItem() != NULL)
                     && (!i->parentItem()->isSelected()) )
                {
                    QPointF itemTargetPos = i->pos();
                    switch ((int)i->rotation())
                    {
                    case 0:
                        itemTargetPos = i->pos() + offset;
                        break;
                    case -90:
                        itemTargetPos = i->pos() + QPointF(offset.y(), -offset.x());
                        break;
                    case -180:
                        itemTargetPos = i->pos() - offset;
                        break;
                    case -270:
                        itemTargetPos = i->pos() - QPointF(offset.y(), -offset.x());
                        break;
                    default:
                        itemTargetPos = i->pos() + offset;
                        break;
                    }
                    i->setPos(itemTargetPos);
                }
            }
            _dragState = DS_NoDrag;
            qDebug() << __PRETTY_FUNCTION__ << " ~DS_NoDrag > DS_NoDrag";
        }
        return;
    }
    QGraphicsScene::keyPressEvent(event);
}

void GScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    QAction *someAction = new QAction("Some Scene Action", this);
    QMenu* contextMenu = new QMenu;
    contextMenu->addActions(QList<QAction*>() << someAction);

    QPointF menuPos = event->scenePos();
    if (!_allowedRect.contains(menuPos) )
    {
        restoreItems();
        event->accept();
        return;
    }

    QGraphicsItem *item = itemAt(menuPos);

    if (item == NULL)
    {
        contextMenu->popup(event->screenPos());
    }
    else
    {
        if (_dragState != DS_Drag)
        {
            QGraphicsScene::contextMenuEvent(event);
        }
        else
        {
            restoreItems();
            event->accept();
        }
    }
}

void GScene::onSelectionChanged()
{
    _selectedItemsRect = calculateSelectedItemsRect();
}

QRectF GScene::calculateSelectedItemsRect()
{
    QList<QGraphicsItem*> selectedElements = selectedItems();
    if (selectedElements.isEmpty())
        return QRectF();

    QRectF selectedItemsRect;
    foreach (QGraphicsItem* item, selectedElements)
    {
        selectedItemsRect |= item->mapRectToScene(item->boundingRect() | item->childrenBoundingRect());
    }
    return selectedItemsRect;
}

/* Return true if selected items can be moved by offset.
 * Otherwise calculate maximum offset for moving items,
 * change selected items position and return false. */
bool GScene::tryNaturalMoveByOffset(QPointF offset)
{
    _selectedItemsRect = calculateSelectedItemsRect();
    //Rectangle of selected items shifted at offset
    QRectF translatedRect = _selectedItemsRect.translated(offset);

    if (_allowedRect.contains(translatedRect) )
    {
        _selectedItemsRect.translate(offset);
        return true;
    }

    qreal dx;   //Allowed distance for moving selected items in horizontal direction
    if (translatedRect.left() < _allowedRect.left())
        dx = _allowedRect.left() - _selectedItemsRect.left();
    else
        if (translatedRect.right() > _allowedRect.right())
            dx = _allowedRect.right() - _selectedItemsRect.right();
        else
            dx = offset.x();

    qreal dy;   //Allowed distance for moving selected items in vertical direction
    if (translatedRect.top() < _allowedRect.top())
        dy = _allowedRect.top() - _selectedItemsRect.top();
    else
        if (translatedRect.bottom() > _allowedRect.bottom())
            dy = _allowedRect.bottom() - _selectedItemsRect.bottom();
        else
            dy = offset.y();

    if ( (dx != 0) || (dy != 0) )
    {
        QList<QGraphicsItem*> selectedElements = selectedItems();
        foreach (QGraphicsItem* item, selectedElements)
        {
            /* Extra position changing only for items on scene without parents
             * (position of child items will be changed automaticaly with
             * position of parent items)*/
            if (item->parentItem() == NULL)
                item->moveBy(dx, dy);
        }

        _lastMousePosition = _lastMousePosition + QPointF(dx, dy);
        _selectedItemsRect.translate(dx, dy);
    }
    return false;
}

void GScene::restoreItems()
{
    foreach (QGraphicsItem *item, selectedItems())
    {
        DraggableItem *draggableItem = dynamic_cast<DraggableItem*>(item);
        if (draggableItem != NULL)
            draggableItem->draggingStop();
    }
}
