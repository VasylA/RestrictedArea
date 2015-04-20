#include <GLabel.h>
#include <ComponentLib.h>
#include <QPalette>
#include <QMenu>
#include <QObject>

GLabel::GLabel(QString text, QColor col, QColor bcol, GItem *parent)
    : QGraphicsTextItem (parent),
      parentGItem(0),
      contextMenu(0),
      removeLabel(0),
      changeText(0),
      attachToBlock(0),
      detachFromBlock(0),
      _wasSave(false),
      sZVAL(40)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIgnoresParentOpacity /*| ItemIsFocusable*/);

    initActions();
    contextMenu = initMenu();

    setParentGItem(parent);

    setAcceptHoverEvents(true);

    setText(text);
    setTextColor(col);
    setBackgroundColor(bcol);

    QFont f = font();
#ifdef Q_OS_MAC
    f.setPixelSize(10);
#else
    f.setPixelSize(12);
#endif
    setFont(f);
}

GLabel::~GLabel()
{
    delete removeLabel;
    delete changeText;
    delete attachToBlock;
    delete detachFromBlock;
}

void GLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    QStyleOptionGraphicsItem* opt = new QStyleOptionGraphicsItem(*option);
    opt->palette.setColor(QPalette::WindowText,(Qt::white));
    opt->state &= ~QStyle::State_Selected;

    painter->setPen(_labelStyle.pen());
    painter->setBrush(QBrush(((isSelected()) ? (_labelStyle.selectedBrush()) : (_labelStyle.brush()))));

    painter->drawRoundedRect(boundingRect(), 5, 5);

    QGraphicsTextItem::paint(painter, opt, widget);
    delete opt;
}

void GLabel::setParentGItem(GItem *p)
{
    parentGItem = p;
    setParentItem(p);

    attachToBlock->setVisible(parentItem() == NULL);
    detachFromBlock->setVisible(parentItem() != NULL);
}

void GLabel::setText(QString txt)
{
    setPlainText(txt);
    emit textChanged(txt);
    //setTransformOriginPoint(boundingRect().center());
}

void GLabel::draggingStart()
{
    if (_draggingState == IDS_CanDrag)
    {
        sZVAL = zValue();
        setZValue(100);

        setOpacity(0.3);
        _draggingState = IDS_Drag;
    }
}

void GLabel::draggingStop()
{
    if (_draggingState == IDS_Drag)
    {
        setZValue(sZVAL);
        setOpacity(1);

        _draggingState = IDS_CanDrag;
    }
}

void GLabel::save()
{
    if (!_wasSave)
    {
        _savedStyle = _labelStyle;
        _wasSave = true;
    }
}

void GLabel::restore()
{
    if (_wasSave)
    {
        _labelStyle = _savedStyle;
        setDefaultTextColor(_labelStyle.textColor());
        update();
        _wasSave = false;
    }
}

void GLabel::setLabelStyle(const GLabelStyle &style)
{
    _labelStyle = style;
    setDefaultTextColor(_labelStyle.textColor());
    update();
}

void GLabel::setTextColor(QColor c)
{
    _labelStyle.setTextColor(c);
    setDefaultTextColor(c);
    emit textChanged(toPlainText());
    update();
}

void GLabel::setBackgroundColor(QColor c)
{
    _labelStyle.setBrush(QBrush(c));
    update();
}

void GLabel::setCenterPos(QPointF p)
{
    this->setPos(p.x()-boundingRect().center().x(),p.y()-boundingRect().center().y());
}

QColor GLabel::backgroundColor()const
{
    return _labelStyle.brush().color();
}

void GLabel::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (_draggingState != IDS_Drag)
        return;

    if (event == NULL)
        return;

    QGraphicsTextItem::mouseMoveEvent(event);
    if (cursor().shape() != Qt::ClosedHandCursor)
        setCursor(Qt::ClosedHandCursor);

    if (GItem::isModify != NULL)
        *GItem::isModify = true;
}

void GLabel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
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

void GLabel::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{    
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

    contextMenu->popup(event->screenPos());
}

QMenu* GLabel::initMenu()
{
    QMenu *m = new QMenu;

    m->addActions(QList<QAction*>() << changeText << removeLabel);
    m->addSeparator();
    m->addActions(QList<QAction*>() << attachToBlock << detachFromBlock);

    return m;
}

void GLabel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (_draggingState == IDS_Drag)
        return;

    QGraphicsTextItem::mousePressEvent(event);
    setCursor(Qt::ClosedHandCursor);
}

void GLabel::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseDoubleClickEvent(event);
    onChangeText();
}

void GLabel::onChangeText()
{
    if (GItem::isModify != NULL)
        *GItem::isModify = true;
    GItem::action_dummyAddLabel->trigger();
}

void GLabel::onAttachToBlock()
{
    GScene *gScene = dynamic_cast<GScene*>(scene());
    if (gScene)
    {
        if (gScene->selectedItems().count() != 2)
        {
            gScene->clearSelection();
            setSelected(true);
        }

        if (GItem::isModify != NULL)
            *GItem::isModify = true;
        GItem::action_attachLabel->trigger();
    }
}

void GLabel::onDetachFromBlock()
{
    if (GItem::isModify != NULL)
        *GItem::isModify = true;
    GItem::action_detachLabel->trigger();
}

void GLabel::onRemoveLabel()
{
    if (GItem::isModify != NULL)
        *GItem::isModify = true;
    GItem::action_dummyCutLabel->trigger();
}

void GLabel::initActions()
{
    removeLabel = new QAction("Erase Label(s)", this);
    removeLabel->setIcon(QIcon(":/new/icons/icons/1306760864_label_32 del.png") );
    changeText = new QAction("Edit Label(s)", this);
    changeText->setIcon(QIcon(":/new/icons/icons/1306760864_label_32.png") );
    attachToBlock = new QAction("Attach Label", this);
    detachFromBlock = new QAction("Detach Label(s)", this);

    connect(removeLabel, SIGNAL(triggered()), this, SLOT(onRemoveLabel()));
    connect(changeText, SIGNAL(triggered()), this, SLOT(onChangeText()));
    connect(attachToBlock, SIGNAL(triggered()), this, SLOT(onAttachToBlock()));
    connect(detachFromBlock, SIGNAL(triggered()), this, SLOT(onDetachFromBlock()));
}

void GLabel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (parentGItem != NULL)
    {
        parentGItem->hoverLeaveEvent(event);
    }
    setCursor(Qt::OpenHandCursor);
}

void GLabel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
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
