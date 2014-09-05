#include <irtkQtTreeModel.h>

#include <QIcon>

irtkQtTreeModel::irtkQtTreeModel(QObject *parent)
    :QAbstractItemModel(parent)
{
    irtkQtImageObject *rootData = NULL;
    rootItem = new irtkQtTreeItem(rootData);
    header << "File";
}

irtkQtTreeModel::~irtkQtTreeModel() {
    delete rootItem;
}

QVariant irtkQtTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    irtkQtTreeItem *item = static_cast<irtkQtTreeItem*>(index.internalPointer());
    irtkQtImageObject *object = item->data(index.column());

    switch (role) {
    case Qt::DisplayRole:
        return object->GetFileName();
        break;
    case Qt::DecorationRole:
        if (object->IsVisible())
            return QIcon(":/icons/eye.png");
        else {
            QPixmap pixmap(25, 25);
            pixmap.fill();
            QIcon icon(pixmap);
            return icon;
        }
        break;
    case Qt::ToolTipRole:
        return object->GetPath();
        break;
    }

    return QVariant();
}

Qt::ItemFlags irtkQtTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant irtkQtTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return header[section];

    return QVariant();
}

QModelIndex irtkQtTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    irtkQtTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<irtkQtTreeItem*>(parent.internalPointer());

    irtkQtTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else return QModelIndex();
}

QModelIndex irtkQtTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    irtkQtTreeItem *childItem = static_cast<irtkQtTreeItem*> (index.internalPointer());
    irtkQtTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childIndex(), 0, parentItem);
}

int irtkQtTreeModel::rowCount(const QModelIndex &parent) const {
    irtkQtTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<irtkQtTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int irtkQtTreeModel::columnCount(const QModelIndex & /* parent */) const {
    return rootItem->columnCount();
}

bool irtkQtTreeModel::setData(const QModelIndex &index, irtkQtImageObject * const value, int role) {
    if (role != Qt::EditRole)
        return false;

    irtkQtTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    return result;
}

bool irtkQtTreeModel::insertRows(int position, int rows, const QModelIndex &parent) {
    irtkQtTreeItem *parentItem = getItem(parent);

    beginInsertRows(parent, position, position + rows - 1);
    bool success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

bool irtkQtTreeModel::removeRows(int position, int rows, const QModelIndex &parent) {
    irtkQtTreeItem *parentItem = getItem(parent);

    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

irtkQtTreeItem *irtkQtTreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        irtkQtTreeItem *item = static_cast<irtkQtTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }

    return rootItem;
}
