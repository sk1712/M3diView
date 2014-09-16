#include <irtkQtTreeItem.h>

irtkQtTreeItem::irtkQtTreeItem(irtkQtImageObject * const data, irtkQtTreeItem *parent)
    : itemData(data), parentItem(parent)
{}

irtkQtTreeItem::~irtkQtTreeItem() {
    qDeleteAll(childItems);
    delete itemData;
}

void irtkQtTreeItem::appendChild(irtkQtTreeItem *item) {
    childItems.append(item);
}

irtkQtTreeItem *irtkQtTreeItem::child(int row) {
    return childItems.value(row);
}

int irtkQtTreeItem::childCount() const {
    return childItems.count();
}

int irtkQtTreeItem::columnCount() const {
    return 1;
}

irtkQtImageObject* irtkQtTreeItem::data() const {
    return itemData;
}

int irtkQtTreeItem::childIndex() const {
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<irtkQtTreeItem*>(this));

    return 0;
}

irtkQtTreeItem* irtkQtTreeItem::parent() {
    return parentItem;
}

bool irtkQtTreeItem::insertChildren(int position, int count, int /*columns*/) {
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        irtkQtImageObject* data = NULL;
        irtkQtTreeItem *item = new irtkQtTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool irtkQtTreeItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.count())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool irtkQtTreeItem::setData(int column, irtkQtImageObject * const value) {
    if (column != 0) {
        return false;
    }

    itemData = value;
    return true;
}

irtkQtTreeItem * irtkQtTreeItem::takeChildAt(int row) {
    if (row < 0 || row > childItems.size())
        return NULL;

    return childItems.takeAt(row);
}

bool irtkQtTreeItem::insertChildAt(int row, irtkQtTreeItem *childItem) {
    if (row < 0 || row > childItems.size())
        return false;

    childItems.insert(row, childItem);
    return true;
}
