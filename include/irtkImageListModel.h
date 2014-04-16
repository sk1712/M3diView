#ifndef IRTKIMAGELISTMODEL_H
#define IRTKIMAGELISTMODEL_H

#include <irtkQtImageObject.h>

#include <QAbstractListModel>

class irtkImageListModel : public QAbstractListModel
{
    Q_OBJECT

    QList<irtkQtImageObject*> _imageList;

public:
    /// model for list view
    irtkImageListModel(QObject *parent = 0);

    irtkImageListModel(const QList<irtkQtImageObject*> & imageList, QObject *parent = 0);

    ~irtkImageListModel();

    /// reimplemented virtual functions of QAbstractListModel
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
};

#endif // IRTKIMAGELISTMODEL_H
