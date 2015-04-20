#ifndef GLABEL_H
#define GLABEL_H

#include <QGraphicsTextItem>
#include <QAction>
#include <QObject>

#include "draggableitem.h"
#include "ExportWorkArea/glabelstyle.h"

QT_FORWARD_DECLARE_CLASS (GItem)

class GLabel : public QGraphicsTextItem, public DraggableItem
{
    Q_OBJECT

protected:
    GItem* parentGItem;
    QMenu* contextMenu;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QAction *removeLabel;
    QAction *changeText;
    QAction *attachToBlock;
    QAction *detachFromBlock;

    void initActions();
    virtual QMenu* initMenu();

public:
    GLabel(QString text, QColor col = QColor(Qt::white), QColor bcol = QColor(20,20,20,100), GItem *parent = NULL);
    ~GLabel();

    void setTextColor(QColor c);
    void setBackgroundColor(QColor c);
    void setCenterPos(QPointF p);
    QColor backgroundColor()const;

    void setParentGItem(GItem*p);
    void setText(QString txt);

    void draggingStart();
    void draggingStop();

    void save();
    void restore();

    void setLabelStyle(const GLabelStyle &style);

public slots:
    void onRemoveLabel();
    void onChangeText();
    void onAttachToBlock();
    void onDetachFromBlock();

signals:
    void textChanged(const QString &newText);

private:
    GLabelStyle _labelStyle;
    GLabelStyle _savedStyle;
    bool _wasSave;
    qreal sZVAL;
};

#endif // GLABEL_H
