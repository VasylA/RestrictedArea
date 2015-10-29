#include "grectitem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <qmath.h>

#include <QDebug>

GRectItem::GRectItem(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _area(QRectF(0, 0, 30, 30)),
    _name("Unknown"),
    _label(NULL)
{
    initStyle();

    setTransformOriginPoint(boundingRect().center());
}

GRectItem::GRectItem(QRectF rect, QString name, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _area(rect),
    _name(name),
    _label(NULL)
{
    initStyle();

    setTransformOriginPoint(boundingRect().center());
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

//    painter->save();
//    painter->setPen(Qt::red);
//    painter->setBrush(QBrush(Qt::red));
//    painter->drawEllipse(QPointF(0, 0), 5, 5);
//    painter->drawEllipse(mapFromScene(scenePos()), 5, 5);
//    painter->drawEllipse(mapFromParent(pos()), 5, 5);
//    painter->restore();
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

    qDebug(__PRETTY_FUNCTION__);
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

qreal GRectItem::rotation() const
{
    if (parentItem())
    {
        QTransform tr = sceneTransform();
        qreal angel = atan2(tr.m12(), tr.m11());
        qreal angelDegree = (qreal)qRound(180 / M_PI * angel);
        if (angelDegree < 0)
            angelDegree += 360;
        return angelDegree;
    }
    return QGraphicsItem::rotation();

}

void GRectItem::setPhysicalRotation(double deltaAngle)
{
    qDebug() << "Pos before rotation: " << pos();

    setTransformOriginPoint(boundingRect().center());

    qreal currentAngle = rotation();
    currentAngle += deltaAngle;

    while (currentAngle >= 360 )
        currentAngle -= 360;

    while (currentAngle <= 0 )
        currentAngle += 360;

    setRotation(currentAngle);

    qDebug() << "Pos after rotation: " << pos();
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
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << " Shit happened!";
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
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << " Shit happened!";
    }
}

void GRectItem::cacheTransform()
{
    _parameters.isValid = true;
    _parameters.pos = scenePos();
    _parameters.angel = rotation();
}

void GRectItem::applyTransform()
{
    if (_parameters.isValid)
    {
        _parameters.isValid = false;
        setPos(_parameters.pos);
        setRotation(_parameters.angel);
    }
}
