#ifndef QTTHREEDIMENSIONALGLWIDGET_H
#define QTTHREEDIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

#include <QPoint>

class QtThreeDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    float horizontalRotation, verticalRotation;

    QPoint lastPosition;

public:

    /// class constructor
    QtThreeDimensionalGlWidget(QWidget *parent = NULL);

    void drawImage();

protected:

    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();

    void rotate();

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

private slots:

    void rotateLeft();

    void rotateRight();

    void rotateUp();

    void rotateDown();
};

#endif // QTTHREEDIMENSIONALGLWIDGET_H
