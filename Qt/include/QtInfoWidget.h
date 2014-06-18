#ifndef QTINFOWIDGET_H
#define QTINFOWIDGET_H

#include <irtkImage.h>

#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>


class QtInfoWidget : public QWidget
{
    Q_OBJECT

    /// Image currently chosen
    irtkImage *_image;

    /// Basic image info
    QLineEdit *imageSizeEdit, *voxelSizeEdit, *imageOriginEdit;

    /// Axes info
    QLineEdit *xAxisEdit, *yAxisEdit, *zAxisEdit;

    /// Image to world matrix
    QTableWidget *imageToWorldMatrix;

    /// World to image matrix
    QTableWidget *worldToImageMatrix;

public:

    /// Class constructor
    QtInfoWidget(QWidget * parent = 0);

    /// Set a size hint
    QSize sizeHint() const;

    /// Set currently chosen image
    void setImage(irtkImage *image);

    /// Update all image information
    void update();

private:

    /// Update basic image information
    void updateImageInfo();

    /// Update image to world matrix
    void updateImageToWorldMatrix();

    /// Update world to image matrix
    void updateWorldToImageMatrix();
};

#endif // QTINFOWIDGET_H
