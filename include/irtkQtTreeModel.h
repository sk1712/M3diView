#ifndef IRTKQTTREEMODEL_H
#define IRTKQTTREEMODEL_H

#include <irtkQtTreeItem.h>

#include <QAbstractItemModel>

class irtkQtTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    irtkQtTreeItem *rootItem;

public:

    irtkQtTreeModel(QObject *parent = 0);

    ~irtkQtTreeModel();

    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool insertRows(int row, int count,
                    const QModelIndex &parent = QModelIndex());

    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex());

private:

    irtkQtTreeItem *getItem(const QModelIndex &index) const;
};

#endif // IRTKQTTREEMODEL_H
