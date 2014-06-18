#include <QtGlWidget.h>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>


QtGlWidget::QtGlWidget(QWidget *parent)
    : QGLWidget(parent) {
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

void QtGlWidget::updateDrawable(QVector<QRgb**> drawable) {
    deleteDrawable();
    _drawable = drawable;
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

void QtGlWidget::wheelEvent(QWheelEvent *event) {
    // delta = the distance that the wheel is rotated, in eighths of a degree
    int numDegrees = event->delta() /8;
    // Most mouse types work in steps of 15 degrees
    int numSteps = numDegrees / 15;

    emit wheelMoved(numSteps);
}

void QtGlWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Left:
        emit leftKeyPressed();
        break;
    case Qt::Key_Right:
        emit rightKeyPressed();
        break;
    case Qt::Key_Up:
        emit upKeyPressed();
        break;
    case Qt::Key_Down:
        emit downKeyPressed();
        break;
    default:
        event->ignore();
        break;
    }
}
