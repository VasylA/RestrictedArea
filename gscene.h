#ifndef GSCENE_H
#define GSCENE_H

#include <QGraphicsScene>

class GScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum DragState
    {
        DS_NoDrag,
        DS_CanDrag,
        DS_Drag
    };

    explicit GScene(QObject *parent = 0);
    ~GScene();

    QRectF allowedRect() const;
    void setAllowedRect(const QRectF &allowedRect);

    void fitSceneRectToVisibleItems();

    bool dropOnFocusOut() const;
    void setDropOnFocusOut(bool dropOnFocusOut);

signals:

public slots:
    void rotateLeft();
    void rotateRight();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void focusOutEvent(QFocusEvent *event);

private slots:
    void onSelectionChanged();

private:
    QRectF _allowedRect;
    QRectF _selectedItemsRect;
    QPointF _lastMousePosition;
    DragState _dragState;
    QList<QGraphicsItem*> _lastSelectedElements;
    bool _dropOnFocusOut;

    void rotateSelected(double angle);
    QRectF calculateSelectedItemsRect();
    bool tryNaturalMoveByOffset(QPointF offset);
    void startItemsDragging();
    void stopItemsDragging();
    void stopItemsDragging(QList<QGraphicsItem*> items);
};

#endif // GSCENE_H
