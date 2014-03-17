#include <QtGlWidget.h>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

QtGlWidget::QtGlWidget(QWidget *parent)
    : QGLWidget(parent) {
    setMouseTracking(true);
}

QSize QtGlWidget::sizeHint() const {
    return QSize(307, 307);
}

void QtGlWidget::mousePressEvent(QMouseEvent *event) {
    switch(event->button()) {
    case Qt::LeftButton:
        emit leftButtonPressed(event->x(), event->y());
        break;
    default:
        event->ignore();
        break;
    }
}

void QtGlWidget::mouseMoveEvent(QMouseEvent *event) {
    printf("%d, %d \n", event->x(), event->y());
}

void QtGlWidget::wheelEvent(QWheelEvent *event) {
    int numDegrees = event->delta() /8;
    int numSteps = numDegrees / 15;

    emit wheelMoved(numSteps);
}

void QtGlWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {

    default:
        event->ignore();
        break;
    }
}
