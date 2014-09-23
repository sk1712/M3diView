#ifndef QTINFOWIDGET_H
#define QTINFOWIDGET_H

#include <irtkImage.h>

#include <QWidget>

class QLineEdit;
class QTextEdit;
class QTableWidget;

class irtkQtImageObject;

/*
 * Widget displaying image information
 */

class QtInfoWidget : public QWidget
{
    Q_OBJECT

    /// Image currently chosen
    irtkQtImageObject *_image;

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
    void setImage(irtkQtImageObject *image);

    /// Update all image information
    void update();

private:

    /// Create component widgets
    void createWidgets();

    /// Create QLineEdit and set its parameters
    void createLineEdit(QLineEdit** lineEdit);

    /// Create QTableWidget and set its parameters
    void createTableWidget(QTableWidget** tableWidget);

    /// Update basic image information
    void updateImageInfo();

    /// Update image to world matrix
    void updateImageToWorldMatrix();

    /// Update world to image matrix
    void updateWorldToImageMatrix();
};

#endif // QTINFOWIDGET_H
