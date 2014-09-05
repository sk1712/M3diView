#ifndef IRTKQTTREEITEM_H
#define IRTKQTTREEITEM_H

#include <irtkQtImageObject.h>

#include <QList>
#include <QVariant>


class irtkQtTreeItem {

    QList<irtkQtTreeItem*> childItems;

    irtkQtImageObject *itemData;

    irtkQtTreeItem *parentItem;

public:

    irtkQtTreeItem(irtkQtImageObject * const data, irtkQtTreeItem *parent = 0);

    ~irtkQtTreeItem();

    void appendChild(irtkQtTreeItem *child);

    irtkQtTreeItem *child(int row);

    int childCount() const;

    int columnCount() const;

    irtkQtImageObject* data(int column = 0) const;

    int childIndex() const;

    irtkQtTreeItem *parent();

    bool insertChildren(int position, int count, int columns);

    bool removeChildren(int position, int count);

    bool setData(int column, irtkQtImageObject * const value);
};

#endif // IRTKQTTREEITEM_H
