#include <QtGlWidget.h>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <iostream>

QtGlWidget::QtGlWidget(QWidget *parent)
    : QGLWidget(parent) {
    setAttribute(Qt::WA_AcceptTouchEvents);
    pixelRatio = windowHandle()->devicePixelRatio();

    imageWorldOrigin = "";
    imageOriginVisible = true;
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

bool QtGlWidget::saveScreenshotInFile(QString file) {
    QImage screenshot = getDisplayedImage();
    bool status = screenshot.save(file);
    return status;
}

QImage QtGlWidget::getDisplayedImage() {
    return this->grabFrameBuffer();
}

void QtGlWidget::setWorldOrigin(const double x, const double y, const double z) {
    imageWorldOrigin = "World : " + QString::number(x) +
                             ", " + QString::number(y) +
                             ", " + QString::number(z);
}

void QtGlWidget::setImageOriginVisible(const bool visible) {
    imageOriginVisible = visible;
    update();
}

void QtGlWidget::updateSegmentation(QVector<QRgb *> drawable) {
    deleteSegmentationDrawable();
    _segmentationDrawable = drawable;
}

void QtGlWidget::updateDrawable(QVector<QRgb**> drawable) {
    deleteDrawable();
    _drawable = drawable;
}

void QtGlWidget::deleteSegmentationDrawable() {
    QVector<QRgb*>::iterator it;

    for (it = _segmentationDrawable.begin(); it != _segmentationDrawable.end(); ++it) {
        delete [] (*it);
    }

    _segmentationDrawable.clear();
}

void QtGlWidget::drawImageOrigin() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    qglColor(Qt::green);
    QFont arialFont("Arial", 10, QFont::Bold, false);

    renderText(5, 10, imageWorldOrigin, arialFont);
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
    // Most mouse types work in steps of 15 degrees
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numPixels.isNull()) {
        emit wheelMoved(numPixels.y());
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        emit wheelMoved(numSteps.y());
    }

    event->accept();
}

void QtGlWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Left:
        qDebug("Left key pressed");
        emit leftKeyPressed();
        break;
    case Qt::Key_Right:
        qDebug("Right key pressed");
        emit rightKeyPressed();
        break;
    case Qt::Key_Up:
        qDebug("Up key pressed");
        emit upKeyPressed();
        break;
    case Qt::Key_Down:
        qDebug("Down key pressed");
        emit downKeyPressed();
        break;
    default:
        event->ignore();
        break;
    }
}
