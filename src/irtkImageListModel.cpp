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
        // Text shown for the item
        case Qt::DisplayRole :
            return _imageList.at(index.row())->GetFileName();
            break;
        // Icon shown next to the item
        case Qt::DecorationRole :
            if (_imageList.at(index.row())->IsVisible())
                // If image is visible show eye icon
                return QIcon(":/icons/eye.png");
            else {
                // Else show white icon
                QPixmap pixmap(16, 16);
                pixmap.fill();
                QIcon icon(pixmap);
                return icon;
            }
            break;
        // Item tooltip
        case Qt::ToolTipRole :
            return _imageList.at(index.row())->GetPath();
            break;
    }

    return QVariant();
}

int irtkImageListModel::rowCount(const QModelIndex &parent) const {
    return _imageList.size();
}
