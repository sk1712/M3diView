#ifndef IRTKQTTREEITEM_H
#define IRTKQTTREEITEM_H

#include <irtkQtImageObject.h>

#include <QList>
#include <QVariant>
#include <QVector>

class irtkQtTreeItem {

    QList<irtkQtTreeItem*> childItems;

    QVector<QVariant> itemData;

    irtkQtTreeItem *parentItem;

public:

    irtkQtTreeItem(QVector<QVariant> &data, irtkQtTreeItem *parent = 0);

    ~irtkQtTreeItem();

    void appendChild(irtkQtTreeItem *child);

    irtkQtTreeItem *child(int row);

    int childCount() const;

    int columnCount() const;

    QVariant data(int column) const;

    int childIndex() const;

    irtkQtTreeItem *parent();

    bool insertChildren(int position, int count, int columns);

    bool removeChildren(int position, int count);

    bool setData(int column, const QVariant &value);
};

#endif // IRTKQTTREEITEM_H
