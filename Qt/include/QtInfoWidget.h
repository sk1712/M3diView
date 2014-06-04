#ifndef QTINFOWIDGET_H
#define QTINFOWIDGET_H

#include <irtkImage.h>

#include <QLineEdit>
#include <QTextEdit>


class QtInfoWidget : public QWidget
{
    Q_OBJECT

    irtkImage *_image;

    QLineEdit *imageSizeEdit, *voxelSizeEdit, *imageOriginEdit;

    QLineEdit *xAxisEdit, *yAxisEdit, *zAxisEdit;

    QTextEdit *imageToWorldMatrix;

    QTextEdit *worldToImageMatrix;

public:

    /// Class constructor
    QtInfoWidget(QWidget * parent = 0);

    void setImage(irtkImage *image);

    void update();

private:

    void updateImageInfo();

    void updateImageToWorldMatrix();

    void updateWorldToImageMatrix();
};

#endif // QTINFOWIDGET_H
