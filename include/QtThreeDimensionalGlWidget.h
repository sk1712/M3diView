#ifndef QTTHREEDIMENSIONALGLWIDGET_H
#define QTTHREEDIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

#include <QPoint>

class QtThreeDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    float horizontalRotation, verticalRotation;

    QPoint lastPosition;

    float dimensions[3];

    float currentSlice[3];

    GLuint textures[3];

    double Xsize, Ysize, Zsize;

public:

    /// class constructor
    QtThreeDimensionalGlWidget(QWidget *parent = NULL);

    ~QtThreeDimensionalGlWidget();

    void drawImage() const;

    void createTextures();

    void setDimensions(int *dim);

    void setResolution(double dx, double dy, double dz);

    void updateDrawable(QVector<QRgb**> drawable);

    void setCurrentSlice(int *current);

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
    dimensions[2] = dim[2] * Zsize;
}

inline void QtThreeDimensionalGlWidget::setResolution(double dx, double dy, double dz) {
    Xsize = dx;
    Ysize = dy;
    Zsize = dz;
}

inline void QtThreeDimensionalGlWidget::setCurrentSlice(int *current) {
    currentSlice[0] = current[0];
    currentSlice[1] = current[1];
    currentSlice[2] = current[2] * Zsize;
}

#endif // QTTHREEDIMENSIONALGLWIDGET_H
