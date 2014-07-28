#ifndef IRTKIMAGELISTMODEL_H
#define IRTKIMAGELISTMODEL_H

#include <irtkQtImageObject.h>

#include <QAbstractListModel>

/*
 * Class used as a model for the image QListView
 */

class irtkImageListModel : public QAbstractListModel
{
    Q_OBJECT

    /// List of all images loaded on the application
    QList<irtkQtImageObject*> _imageList;

public:

    /// Model for list view
    irtkImageListModel(QObject *parent = 0);

    irtkImageListModel(const QList<irtkQtImageObject*> & imageList, QObject *parent = 0);

    ~irtkImageListModel();

    /// Reimplemented virtual functions of QAbstractListModel
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
};

#endif // IRTKIMAGELISTMODEL_H
