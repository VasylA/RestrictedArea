#include "grectitem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

GRectItem::GRectItem(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _area(QRectF(0, 0, 30, 30)),
    _name("Unknown")
{
    initStyle();
}

GRectItem::GRectItem(QRectF rect, QString name, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _area(rect),
    _name(name)
{
    initStyle();
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

void GRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    setCursor(Qt::ClosedHandCursor);

    QGraphicsItem::mousePressEvent(event);
}

void GRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    setCursor(Qt::OpenHandCursor);

    QGraphicsItem::mouseReleaseEvent(event);
}

void GRectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    QGraphicsItem::mouseDoubleClickEvent(event);
}

void GRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug(__PRETTY_FUNCTION__);

    QGraphicsItem::mouseMoveEvent(event);
}

void GRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    setCursor(Qt::OpenHandCursor);

    QGraphicsItem::hoverEnterEvent(event);
}

void GRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug(__PRETTY_FUNCTION__);

    setCursor(Qt::ArrowCursor);

    QGraphicsItem::hoverLeaveEvent(event);
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

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void GRectItem::setName(const QString &name)
{
    _name = name;
}

void GRectItem::setArea(const QRectF &area)
{
    _area = area;
}

