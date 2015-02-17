#ifndef DRAGGABLEITEM_H
#define DRAGGABLEITEM_H

class DraggableItem
{
public:
    enum ItemDragState
    {
        IDS_NoDrag  = 0,
        IDS_CanDrag = 1,
        IDS_Drag    = 3
    };

    DraggableItem();

    virtual void draggingStart() = 0;
    virtual void draggingStop() = 0;

    ItemDragState draggingState() const;
    void setDraggingState(const ItemDragState &draggingState);

protected:
    ItemDragState _draggingState;
};

#endif // DRAGGABLEITEM_H
