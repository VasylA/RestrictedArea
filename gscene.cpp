#include "gscene.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include "draggableitem.h"
#include "grectitem.h"
#include "glabel.h"

#include <QDebug>

GScene::GScene(QObject *parent) :
    QGraphicsScene(parent),
    _allowedRect(QRect(-1800, -1400, 3600, 2800)),
    _selectedItemsRect(QRectF()),
    _dragState(DS_NoDrag),
    _dropOnFocusOut(true)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

//    qDebug() << __PRETTY_FUNCTION__ << " DS_NoDrag";
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
        const int adjustment = 100;
        setSceneRect(itemsBoundingRect().adjusted(-adjustment,
                                                  -adjustment,
                                                  adjustment,
                                                  adjustment));
    }
}

void GScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (event)
    {
        if (_dragState == DS_NoDrag)
        {
            if ((event->button() == Qt::LeftButton) && (itemAt(event->scenePos(), QTransform()) != NULL))
                _dragState = DS_CanDrag;
        }
        else
        {
            /* Check if is pressed more than one button by test
             * if a event->buttons() have more than one bit set to 1 */
            if (event->buttons() & (event->buttons() - 1))
            {
                /* Workaround to fix issue when left mouse button is pressed and selected items are
                 * moving and another mouse button is pressed. After that movingItemsInitialPositions
                 * field in QGraphicsScene class has previous value (before the moving).
                 * As a result when we want to move selected items again they all change their
                 * positions to positions before first moving.
                 *
                 * In generated mouse release event for graphics item that is under cursor while moving
                 * movingItemsInitialPositions will be cleared and then will be reinitialize
                 * in next mouse move event.
                 *
                 * Also we need call draggingStop() for every QGraphicsItem element selected before.*/
                stopItemsDragging();
            }
        }
        QGraphicsScene::mousePressEvent(event);
    }
}

void GScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);
    if (_dragState == DS_Drag)
    {
        stopItemsDragging();
        _selectedItemsRect = calculateSelectedItemsRect();
    }
    if (_dragState != DS_NoDrag)
    {
        _dragState = DS_NoDrag;
//        qDebug() << __PRETTY_FUNCTION__ << " ~        > DS_NoDrag";
    }

    if (event == NULL)
        return;

    QGraphicsScene::mouseReleaseEvent(event);
}

void GScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (_dragState == DS_NoDrag)
    {
        if ( (event->button() == Qt::LeftButton) && (itemAt(event->scenePos(), QTransform()) != NULL) )
        {
            _dragState = DS_CanDrag;
//            qDebug() << __PRETTY_FUNCTION__ << " DS_NoDrag > DS_CanDrag";
        }
    }

    QGraphicsScene::mouseDoubleClickEvent(event);
}

void GScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (event == NULL)
        return;

    DraggableItem *grabberItem = dynamic_cast<DraggableItem*>(mouseGrabberItem());
    if (grabberItem)
    {
        if (_dragState == DS_CanDrag)
            startItemsDragging();

        if (_dragState == DS_NoDrag)
        {
//            qDebug() << __PRETTY_FUNCTION__ << " Shit happened!";
            return;
        }
        else
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

    if ((event->modifiers() == Qt::AltModifier)
        || (event->modifiers() == (Qt::AltModifier | Qt::KeypadModifier)))
        x = x1;
    else if ((event->modifiers() == Qt::ControlModifier)
             || (event->modifiers() == (Qt::ControlModifier | Qt::KeypadModifier)))
        x = x10;

    if (x >= 0)
    {
        if (_dragState != DS_NoDrag)
            return;

        if (event->key() == Qt::Key_Up)
            offset = QPointF(0, -x);
        else if (event->key() == Qt::Key_Down)
            offset = QPointF(0, x);
        else if (event->key() == Qt::Key_Left)
            offset = QPointF(-x, 0);
        else if (event->key() == Qt::Key_Right)
            offset = QPointF(x, 0);

        if (!offset.isNull())
        {
            if (!_allowedRect.contains(_selectedItemsRect.translated(offset)))
                return;

            _selectedItemsRect.translate(offset);
            _lastSelectedElements = selectedItems();
            foreach (QGraphicsItem *i, _lastSelectedElements)
            {
                QTransform savedTransform = i->transform();
                i->setTransform(QTransform());

                if (i->parentItem() == NULL)
                    i->moveBy(offset.x(), offset.y());

                if ((i->parentItem() != NULL)
                    && (!i->parentItem()->isSelected()))
                {
                    //New item's position in scene coordinates
                    QPointF newScenePos = i->scenePos() + offset;
                    //New item's position in parent coordiantes
                    QPointF newPos = i->mapToParent(i->mapFromScene(newScenePos) );
                    //Current item's displacement in the parent coordinates
                    QPointF itemOffset = newPos - i->pos();
                    i->moveBy(itemOffset.x(), itemOffset.y());
                }

                i->setTransform(savedTransform);
            }

            fitSceneRectToVisibleItems();
            _dragState = DS_NoDrag;
//            qDebug() << __PRETTY_FUNCTION__ << " ~DS_NoDrag > DS_NoDrag";
        }
        return;
    }

    QGraphicsScene::keyPressEvent(event);
}

void GScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    QAction *someAction = new QAction("Some Scene Action", this);
    QMenu* contextMenu = new QMenu;
    contextMenu->addActions(QList<QAction*>() << someAction);

    if (event == NULL)
        return;

    if (_dragState == DS_Drag)
    {
        stopItemsDragging();
        return;
    }

    QPointF menuPos = event->scenePos();
    if (!_allowedRect.contains(menuPos) )
        return;

    if (itemAt(menuPos, QTransform()) == NULL)
        contextMenu->popup(event->screenPos());
    else
        QGraphicsScene::contextMenuEvent(event);
}

void GScene::focusOutEvent(QFocusEvent *event)
{
    if (_dropOnFocusOut)
        stopItemsDragging();
    else
        QGraphicsScene::focusOutEvent(event);
}

void GScene::onSelectionChanged()
{
    _selectedItemsRect = calculateSelectedItemsRect();
}

void GScene::rotateSelected(double angle)
{
    foreach (QGraphicsItem *item, selectedItems())
    {
        GRectItem *rectItem = dynamic_cast<GRectItem*>(item);
        if (rectItem != NULL)
        {
            rectItem->setPhysicalRotation(angle);
            continue;
        }

        GLabel *labelItem = dynamic_cast<GLabel*>(item);
        if (labelItem != NULL)
        {
            if (labelItem->parentItem() != NULL)
                if (labelItem->parentItem()->isSelected())
                    continue;
            labelItem->setPhysicalRotation(angle);
        }
    }
}

bool GScene::dropOnFocusOut() const
{
    return _dropOnFocusOut;
}

void GScene::setDropOnFocusOut(bool dropOnFocusOut)
{
    _dropOnFocusOut = dropOnFocusOut;
}

void GScene::rotateLeft()
{
    rotateSelected(-15);
}

void GScene::rotateRight()
{
    rotateSelected(15);
}

QRectF GScene::calculateSelectedItemsRect()
{
    _lastSelectedElements = selectedItems();
    if (_lastSelectedElements.isEmpty())
        return QRectF();

    QRectF selectedItemsRect;
    foreach (QGraphicsItem* item, _lastSelectedElements)
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
    if (_selectedItemsRect.isEmpty())
        return true;

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
        _lastSelectedElements = selectedItems();
        foreach (QGraphicsItem* item, _lastSelectedElements)
        {
            QTransform savedTransform = item->transform();
            item->setTransform(QTransform());
            /* Extra position changing only for items on scene without parents
             * (position of child items will be changed automaticaly with
             * position of parent items)*/
            if (item->parentItem() == NULL)
                item->moveBy(dx, dy);

            if ( (item->parentItem() != NULL)
                 && (!item->parentItem()->isSelected()) )
            {
                //New item's position in scene coordiantes
                QPointF newScenePos = item->scenePos() + QPointF(dx, dy);
                //New item's position in parent coordiantes
                QPointF newPos = item->mapToParent(item->mapFromScene(newScenePos) );
                //Current item's displacement in the parent coordinates
                QPointF itemOffset = newPos - item->pos();
                item->moveBy(itemOffset.x(), itemOffset.y());
            }

            item->setTransform(savedTransform);
        }

        _lastMousePosition = _lastMousePosition + QPointF(dx, dy);
        _selectedItemsRect.translate(dx, dy);
    }
    return false;
}

void GScene::startItemsDragging()
{
    if (_dragState == DS_CanDrag)
    {
        _dragState = DS_Drag;
        _lastSelectedElements = selectedItems();
        for (int i = 0; i < _lastSelectedElements.count(); i++)
        {
            DraggableItem *draggableItem = dynamic_cast<DraggableItem*>(_lastSelectedElements.at(i));
            if (draggableItem != NULL)
                draggableItem->draggingStart();
        }
    }
}

void GScene::stopItemsDragging()
{
    stopItemsDragging(_lastSelectedElements);
}

void GScene::stopItemsDragging(QList<QGraphicsItem*> items)
{
    if (_dragState != DS_NoDrag)
    {
        for (QList<QGraphicsItem*>::iterator it = items.begin(); it != items.end(); ++it)
            if (DraggableItem *draggableItem = dynamic_cast<DraggableItem*>(*it))
                draggableItem->draggingStop();

        _dragState = DS_NoDrag;
    }
}

