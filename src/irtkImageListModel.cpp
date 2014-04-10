#include <irtkImageListModel.h>

#include <QIcon>
#include <QPixmap>

irtkImageListModel::irtkImageListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

irtkImageListModel::irtkImageListModel(const QList<irtkQtImageObject*> & imageList, QObject *parent)
    : QAbstractListModel(parent)
{
    _imageList = imageList;
}

irtkImageListModel::~irtkImageListModel() {

}

QVariant irtkImageListModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid())
        return QVariant();

    switch (role) {
        case Qt::DisplayRole :
            return _imageList.at(index.row())->GetFileName();
        case Qt::DecorationRole :
            if (_imageList.at(index.row())->IsVisible())
                return QIcon(":/icons/eye.png");
            else {
                QPixmap pixmap(16, 16);
                pixmap.fill();
                QIcon icon(pixmap);
                return icon;
            }
        case Qt::ToolTipRole :
            return _imageList.at(index.row())->GetPath();
    }

    return QVariant();
}

int irtkImageListModel::rowCount(const QModelIndex &parent) const {
    return _imageList.size();
}


