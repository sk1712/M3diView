#include <QtGlWidget.h>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

QtGlWidget::QtGlWidget(QWidget *parent)
    : QGLWidget(parent) {
    setMouseTracking(true);
    pixelRatio = windowHandle()->devicePixelRatio();
}

QSize QtGlWidget::sizeHint() const {
    return QSize(307, 307);
}

int QtGlWidget::customWidth() const {
    return width() * pixelRatio;
}

int QtGlWidget::customHeight() const {
    return height() * pixelRatio;
}

void QtGlWidget::mousePressEvent(QMouseEvent *event) {
    switch(event->button()) {
    case Qt::LeftButton:
        emit leftButtonPressed(event->x()*pixelRatio, event->y()*pixelRatio);
        break;
    default:
        event->ignore();
        break;
    }
}

void QtGlWidget::mouseMoveEvent(QMouseEvent *event) {
    //printf("%d, %d \n", event->x(), event->y());
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
