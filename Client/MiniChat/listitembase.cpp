#include "listitembase.h"

ListItemBase::ListItemBase(QWidget *parent)
    : QWidget(parent)
{}

void ListItemBase::SetItemType(ListItemType itemType)
{
    item_type_ = itemType;
}

ListItemType ListItemBase::GetItemType()
{
    return item_type_;
}
