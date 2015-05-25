#include "glabel.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

#include "gscene.h"

#include <QDebug>

GLabel::GLabel(QString text, QColor col, QColor bcol, QGraphicsItem *parent)
    : QGraphicsTextItem(parent),
      _pen(Qt::NoPen),
      _brush(bcol),
      _selectedBrush(QColor(85, 85, 85, 190)),
      _textColor(col),
      _currentAngle(0),
      _zValue(40)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIgnoresParentOpacity);
    setAcceptHoverEvents(true);

    setPlainText(text);
    setDefaultTextColor(col);
}

void GLabel::setPhysicalRotation(double deltaAngle)
{
    _currentAngle += deltaAngle;

    if (qAbs(_currentAngle) == 360)
        _currentAngle = 0;

    qreal x = boundingRect().width() / 2.0;
    qreal y = boundingRect().height() / 2.0;

    this->setTransform(QTransform().translate(x, y).rotate(_currentAngle).translate(-x, -y));
}

void GLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    QStyleOptionGraphicsItem* opt = new QStyleOptionGraphicsItem(*option);
    opt->palette.setColor(QPalette::WindowText,(Qt::white));
    opt->state &= ~QStyle::State_Selected;

    painter->setPen(_pen);
    painter->setBrush(QBrush(((isSelected()) ? (_selectedBrush) : (_brush))));

    painter->drawRoundedRect(boundingRect(), 5, 5);

    QGraphicsTextItem::paint(painter, opt, widget);
    delete opt;
}

void GLabel::draggingStart()
{
    if (_draggingState == IDS_CanDrag)
    {
        _zValue = zValue();
        setZValue(100);

        setOpacity(0.3);
        _draggingState = IDS_Drag;
    }
}

void GLabel::draggingStop()
{
    if (_draggingState == IDS_Drag)
    {
        setZValue(_zValue);
        setOpacity(1);

        _draggingState = IDS_CanDrag;
    }
}

void GLabel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (_draggingState == IDS_Drag)
        return;

    QGraphicsTextItem::mousePressEvent(event);
    setCursor(Qt::ClosedHandCursor);
}

void GLabel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);
    setCursor(Qt::OpenHandCursor);

    if (event && scene())
    {
        QList<QGraphicsItem*> selectedItemsBefore = scene()->selectedItems();
        bool wasSelected = isSelected();
        QGraphicsTextItem::mouseReleaseEvent(event);

        GScene *currentScene = dynamic_cast<GScene*>(scene());
        if (currentScene)
            currentScene->fitSceneRectToVisibleItems();

        /*
         * QTBUG-30990 in some cases right button release will select item and deselect other items
         * right release on item doesn't have to change any selections
         */
        if (event->button() != Qt::RightButton || !scene())
            return;

        if (wasSelected != isSelected())
            setSelected(wasSelected);

        foreach (QGraphicsItem* item, selectedItemsBefore)
        {
            if (!item->isSelected())
                item->setSelected(true);
        }
    }
}

void GLabel::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void GLabel::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //    qDebug(__PRETTY_FUNCTION__);

    if (_draggingState != IDS_Drag)
        return;

    if (event == NULL)
        return;

    QGraphicsTextItem::mouseMoveEvent(event);
    if (cursor().shape() != Qt::ClosedHandCursor)
        setCursor(Qt::ClosedHandCursor);
}

void GLabel::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    QList<QGraphicsItem*> selectedItems = scene()->selectedItems();
    if ( (selectedItems.count() > 0) && (!isSelected()) )
    {
        foreach (QGraphicsItem* currItem, selectedItems)
        {
            if (currItem != this)
                currItem->setSelected(false);
        }
    }
    if (!isSelected())
        setSelected(true);

    if (_draggingState == IDS_Drag)
    {
        qDebug() << __PRETTY_FUNCTION__ << " Shit happened!";
        return;
    }

    QAction *someAction = new QAction("Some Label Action", this);
    QMenu* contextMenu = new QMenu;
    contextMenu->addActions(QList<QAction*>() << someAction);
    contextMenu->popup(event->screenPos());
}

void GLabel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    QGraphicsItem::hoverEnterEvent(event);

    setCursor(Qt::OpenHandCursor);
}

void GLabel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    QGraphicsItem::hoverLeaveEvent(event);

    setCursor(Qt::ArrowCursor);
}

QVariant GLabel::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedChange)
    {
        bool itemIsSelected = ((value == true) ? (true) : (false));

        if (_draggingState == IDS_Drag)
            draggingStop();

        _draggingState = ((itemIsSelected) ? (IDS_CanDrag) : (IDS_NoDrag));
    }
    return QGraphicsItem::itemChange(change, value);
}
