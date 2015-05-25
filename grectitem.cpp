#include "grectitem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

#include <QDebug>

GRectItem::GRectItem(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _area(QRectF(0, 0, 30, 30)),
    _name("Unknown"),
    _currentAngle(0),
    _label(NULL)
{
    initStyle();
}

GRectItem::GRectItem(QRectF rect, QString name, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _area(rect),
    _name(name),
    _currentAngle(0),
    _label(NULL)
{
    initStyle();
}

GRectItem::~GRectItem()
{
    if (_label != NULL)
        delete _label;
}

QRectF GRectItem::boundingRect() const
{
    return QRectF(_area.x() - 1, _area.y() - 1, _area.width() + 2, _area.height() + 2);
}

void GRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setBrush(QBrush((option->state & QStyle::State_Selected) ? SelectedBody : Body) );
    painter->setPen(QPen((option->state & QStyle::State_Selected) ? SelectedFrame : Frame, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter->drawRoundRect(_area, 10, 10);

    painter->setPen(Text);
    painter->drawText(_area, _name, QTextOption(Qt::AlignCenter));
}

QVariant GRectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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

void GRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (_draggingState == IDS_Drag)
    {
        qDebug() << __PRETTY_FUNCTION__ << " Shit happened!";
        return;
    }

    QGraphicsItem::mousePressEvent(event);

    setCursor(Qt::ClosedHandCursor);
}

void GRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (!scene())
        return;

    QList<QGraphicsItem*> selectedItemsBefore = scene()->selectedItems();
    bool wasSelected = isSelected();

    QGraphicsItem::mouseReleaseEvent(event);

    setCursor(Qt::OpenHandCursor);

    /*
     * QTBUG-30990 in some cases right button release will select item and deselect other items
     * right release on item doesn't have to change any selections
     */
    if (event->button() != Qt::RightButton)
        return;

    if (wasSelected != isSelected())
        setSelected(wasSelected);

    foreach (QGraphicsItem* item, selectedItemsBefore)
    {
        if (!item->isSelected())
            item->setSelected(true);
    }
}

void GRectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (_draggingState == IDS_Drag)
    {
        qDebug() << __PRETTY_FUNCTION__ << " Shit happened!";
        return;
    }

    QGraphicsItem::mouseDoubleClickEvent(event);

    setCursor(Qt::OpenHandCursor);
}

void GRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (_draggingState == IDS_NoDrag)
    {
        qDebug() << __PRETTY_FUNCTION__ << " Shit happened!";
        return;
    }

    QGraphicsItem::mouseMoveEvent(event);

    setCursor(Qt::ClosedHandCursor);
}

void GRectItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    if (_draggingState == IDS_Drag)
    {
        qDebug() << __PRETTY_FUNCTION__ << " Shit happened!";
        return;
    }

    QAction *someAction = new QAction("Some Item Action", this);
    QMenu* contextMenu = new QMenu;
    contextMenu->addActions(QList<QAction*>() << someAction);
    contextMenu->popup(event->screenPos());

//    QGraphicsItem::contextMenuEvent(event);
}

void GRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    QGraphicsItem::hoverEnterEvent(event);

    setCursor(Qt::OpenHandCursor);
}

void GRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    QGraphicsItem::hoverLeaveEvent(event);

    setCursor(Qt::ArrowCursor);
}

void GRectItem::initStyle()
{
    Body.setStart(0, 0);
    Body.setFinalStop(0, _area.height());
    Body.setColorAt(0, Qt::white);
    Body.setColorAt(1, Qt::darkGray);

    SelectedBody.setStart(0, 0);
    SelectedBody.setFinalStop(0, _area.height());
    SelectedBody.setColorAt(0, QColor(164,255,164));
    SelectedBody.setColorAt(1, QColor(0,180,0));

    Frame = Body;
    SelectedFrame = SelectedBody;

    Text = Qt::black;

    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void GRectItem::setName(const QString &name)
{
    _name = name;
}

void GRectItem::setPhysicalRotation(double deltaAngle)
{
    _currentAngle += deltaAngle;

    if (qAbs(_currentAngle) == 360)
        _currentAngle = 0;

    qreal x = _area.width() / 2.0;
    qreal y = _area.height() / 2.0;

    this->setTransform(QTransform().translate(x, y).rotate(_currentAngle).translate(-x, -y));
}

GLabel *GRectItem::label() const
{
    return _label;
}

void GRectItem::setLabel(GLabel *newLabel)
{
    const int vertOffset = 5;

    newLabel->hide();
    newLabel->setParentItem(this);

    QRectF  parentRect = mapToParent( boundingRect() ).boundingRect();
    QPointF labelSizeOffset( newLabel->boundingRect().width()/2,
                             newLabel->boundingRect().height() );
    QPointF aboveTopCenterPoint( parentRect.center().x(),parentRect.top() - vertOffset );
    QPointF localCoordPoint( mapFromParent(aboveTopCenterPoint-labelSizeOffset) );
    newLabel->setPos( localCoordPoint );

    removeLabel();

    _label = newLabel;
    _label->show();
}

void GRectItem::removeLabel()
{
    if (_label != NULL)
    {
        _label->setParentItem(NULL);
        delete _label;
        _label = NULL;
    }
}

void GRectItem::setArea(const QRectF &area)
{
    _area = area;
}

void GRectItem::draggingStart()
{
    if (_draggingState == IDS_CanDrag)
    {
        _zValue = zValue();
        setZValue(100);

        setOpacity(0.3);
//        setAcceptedMouseButtons(Qt::LeftButton);
        _draggingState = IDS_Drag;
    }
}

void GRectItem::draggingStop()
{
    if (_draggingState == IDS_Drag)
    {
        setZValue(_zValue);
        setOpacity(1);

//        setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
        _draggingState = IDS_CanDrag;
    }
}
