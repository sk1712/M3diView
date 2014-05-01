#ifndef QTTHREEDIMENSIONALGLWIDGET_H
#define QTTHREEDIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

#include <QPoint>

class QtThreeDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    /// rotation around y axis (horizontal) and x axis (vertical) of the model
    float horizontalRotation, verticalRotation;

    /// mouse position when dragging started
    QPoint lastPosition;

    /// dimensions of quads, number of slices in each direction
    int dimensions[3];

    /// current slice in each direction
    int currentSlice[3];

    /// texture parameters where images are stored
    GLuint textures[3];

public:

    /// class constructor
    QtThreeDimensionalGlWidget(QWidget *parent = NULL);

    /// class destructor
    ~QtThreeDimensionalGlWidget();

    /// draw planes with textured images on screen
    void drawImage() const;

    /// draw borders of planes with different colours
    void drawBorders();

    /// map 2D images to textures
    void createTextures();

    /// set image dimensions
    void setDimensions(int *dim);

    /// set current slices
    void setCurrentSlice(int *current);

    /// update the images to be drawn
    void updateDrawable(QVector<QRgb**> drawable);

protected:

    /// function to initialize OpenGL inherited from QGLWidget
    void initializeGL();

    /// function called whenever OpenGL scene is resized inherited from QGLWidget
    void resizeGL(int w, int h);

    /// function called whenever scene is painted inherited from QGLWidget
    void paintGL();

    /// perform a rotation
    void rotate();

    /// delete drawable
    void deleteDrawable();

    /// callback function handling mouse press
    void mousePressEvent(QMouseEvent *event);

    /// callback function handling mouse release
    void mouseReleaseEvent(QMouseEvent *event);

    /// callback function handling mouse move
    void mouseMoveEvent(QMouseEvent *event);

private slots:

    /// rotate model to the left
    void rotateLeft();

    /// rotate model to the right
    void rotateRight();

    /// rotate model upwards
    void rotateUp();

    /// rotate model downwards
    void rotateDown();
};


inline void QtThreeDimensionalGlWidget::setDimensions(int *dim) {
    dimensions[0] = dim[0];
    dimensions[1] = dim[1];
    dimensions[2] = dim[2];
}

inline void QtThreeDimensionalGlWidget::setCurrentSlice(int *current) {
    currentSlice[0] = current[0];
    currentSlice[1] = current[1];
    currentSlice[2] = current[2];
}

#endif // QTTHREEDIMENSIONALGLWIDGET_H
