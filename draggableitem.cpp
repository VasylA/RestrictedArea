#include "draggableitem.h"

DraggableItem::DraggableItem()
    : _draggingState(IDS_NoDrag)
{
}

DraggableItem::ItemDragState DraggableItem::draggingState() const
{
    return _draggingState;
}

void DraggableItem::setDraggingState(const ItemDragState &draggingState)
{
    _draggingState = draggingState;
}

