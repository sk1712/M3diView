#ifndef QTTHREEDIMENSIONALGLWIDGET_H
#define QTTHREEDIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

#include <QPoint>

class QtThreeDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    float horizontalRotation, verticalRotation;

    QPoint lastPosition;

    int dimensions[3];

    GLuint textures[3];

public:

    /// class constructor
    QtThreeDimensionalGlWidget(QWidget *parent = NULL);

    ~QtThreeDimensionalGlWidget();

    void drawImage();

    void createTextures();

    void setDimensions(int *dim);

    void updateDrawable(QVector<QRgb**> drawable);

protected:

    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();

    void rotate();

    void deleteDrawable();

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

private slots:

    void rotateLeft();

    void rotateRight();

    void rotateUp();

    void rotateDown();
};


inline void QtThreeDimensionalGlWidget::setDimensions(int *dim) {
    dimensions[0] = dim[0];
    dimensions[1] = dim[1];
    dimensions[2] = dim[2];
}

#endif // QTTHREEDIMENSIONALGLWIDGET_H
