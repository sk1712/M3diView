#ifndef QTTHREEDIMENSIONALGLWIDGET_H
#define QTTHREEDIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>


class QtThreeDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    /// Rotation around y axis (horizontal) and x axis (vertical) of the model
    float horizontalRotation, verticalRotation;

    /// Camera field of view, window width and height
    float cameraFOV, width, height;

    /// Mouse position when dragging started
    QPoint lastPosition;

    /// Dimensions of quads, number of slices in each direction
    int dimensions[3];

    /// Current slice in each direction
    int currentSlice[3];

    /// Inverted axes
    bool invertedAxis[3];

    /// Texture parameters where images are stored
    GLuint textures[3];

    /// Flags used to rotate model and/or move camera when drawing
    bool doRotation, moveCamera;

public:

    /// Class constructor
    QtThreeDimensionalGlWidget(QWidget *parent = NULL);

    /// Class destructor
    ~QtThreeDimensionalGlWidget();

    /// Set image dimensions (in image coordinates)
    void setDimensions(const int *dim);

    /// Set current slices
    void setCurrentSlice(const int *current);

    /// Set inverted axis flag
    void setInvertedAxis(const bool *inverted);

protected:

    /// Function to initialize OpenGL inherited from QGLWidget
    void initializeGL();

    /// Function called whenever OpenGL scene is resized inherited from QGLWidget
    void resizeGL(int w, int h);

    /// Function called whenever scene is painted inherited from QGLWidget
    void paintGL();

    /// Perform a rotation
    void rotate();

    /// Delete drawable
    void deleteDrawable();

    /// Callback function handling mouse press
    void mousePressEvent(QMouseEvent *event);

    /// Callback function handling mouse release
    void mouseReleaseEvent(QMouseEvent *event);

    /// Callback function handling mouse move
    void mouseMoveEvent(QMouseEvent *event);

private:

    /// Draw planes with textured images on screen
    void drawImage() const;

    /// Draw borders of planes with different colours
    void drawBorders();

    /// Map 2D images to textures
    void createTextures();

    /// Register the signals of the widget
    void connectSignals();

private slots:

    /// Rotate model to the left
    void rotateLeft();

    /// Rotate model to the right
    void rotateRight();

    /// Rotate model upwards
    void rotateUp();

    /// Rotate model downwards
    void rotateDown();

    /// Zoom in/out on mouse wheel
    void changeZoom(int numSteps);
};


inline void QtThreeDimensionalGlWidget::setDimensions(const int *dim) {
    dimensions[0] = dim[0];
    dimensions[1] = dim[1];
    dimensions[2] = dim[2];
}

inline void QtThreeDimensionalGlWidget::setCurrentSlice(const int *current) {
    currentSlice[0] = current[0];
    currentSlice[1] = current[1];
    currentSlice[2] = current[2];
}

inline void QtThreeDimensionalGlWidget::setInvertedAxis(const bool *inverted) {
    invertedAxis[0] = inverted[0];
    invertedAxis[1] = inverted[1];
    invertedAxis[2] = inverted[2];
}

#endif // QTTHREEDIMENSIONALGLWIDGET_H
