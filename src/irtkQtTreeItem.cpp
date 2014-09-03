#include <irtkQtTreeItem.h>

irtkQtTreeItem::irtkQtTreeItem(QVector<QVariant> &data, irtkQtTreeItem *parent) {
    parentItem = parent;
    itemData = data;
}

irtkQtTreeItem::~irtkQtTreeItem() {
    qDeleteAll(childItems);
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
    return itemData.count();
}

QVariant irtkQtTreeItem::data(int column) const {
    return itemData.value(column);
}

int irtkQtTreeItem::childIndex() const {
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<irtkQtTreeItem*>(this));

    return 0;
}

irtkQtTreeItem* irtkQtTreeItem::parent() {
    return parentItem;
}

bool irtkQtTreeItem::insertChildren(int position, int count, int columns) {
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data;
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

bool irtkQtTreeItem::setData(int column, const QVariant &value) {
    if (column < 0 || column > itemData.size()) {
        return false;
    }

    itemData[column] = value;
    return true;
}
