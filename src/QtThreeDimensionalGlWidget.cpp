#include <QtThreeDimensionalGlWidget.h>

#ifdef Q_OS_MAC
#include "OpenGL/glu.h"
#else
#include "GL/glu.h"
#endif

#include <QMouseEvent>
#include <QDebug>


QtThreeDimensionalGlWidget::QtThreeDimensionalGlWidget(QWidget *parent)
    :QtGlWidget(parent) {
    connect(this, SIGNAL(leftKeyPressed()), this, SLOT(rotateLeft()));
    connect(this, SIGNAL(rightKeyPressed()), this, SLOT(rotateRight()));
    connect(this, SIGNAL(upKeyPressed()), this, SLOT(rotateUp()));
    connect(this, SIGNAL(downKeyPressed()), this, SLOT(rotateDown()));

    horizontalRotation = 0.0f;
    verticalRotation = 0.0f;
}

void QtThreeDimensionalGlWidget::drawImage() {
    glBegin(GL_QUADS);                // Begin drawing the perpendicular surfaces

    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor4f(0.0f, 1.0f, 0.0f, 0.5f);     // Green
    glVertex3f( 1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f,  1.0f);
    glVertex3f( 1.0f, 0.0f,  1.0f);

    // Front face  (z = 1.0f)
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);     // Red
    glVertex3f( 1.0f,  1.0f, 0.0f);
    glVertex3f(-1.0f,  1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, 0.0f);

    // Right face (x = 1.0f)
    glColor4f(1.0f, 0.0f, 1.0f, 0.5f);     // Magenta
    glVertex3f(0.0f,  1.0f, -1.0f);
    glVertex3f(0.0f,  1.0f,  1.0f);
    glVertex3f(0.0f, -1.0f,  1.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);

    glEnd();  // End of drawing color-cube
}

void QtThreeDimensionalGlWidget::initializeGL() {
    // Set background depth to farthest
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    // Nice perspective corrections
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    qglClearColor(Qt::black);
}

void QtThreeDimensionalGlWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -7.0f);  // Move into the screen
    glRotatef((GLfloat) horizontalRotation, 0.0f, 1.0f, 0.0f);  // Rotate about (0,1,0)-axis
    glRotatef((GLfloat) verticalRotation, 1.0f, 0.0f, 0.0f);  // Rotate about (1,0,0)-axis
    glMatrixMode(GL_PROJECTION);
    GLfloat aspect = (GLfloat) w / (GLfloat) h;
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void QtThreeDimensionalGlWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawImage();
}

void QtThreeDimensionalGlWidget::rotate() {
    glMatrixMode(GL_MODELVIEW);

    GLfloat currentModelViewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, currentModelViewMatrix);
    // x axis rotation
    glRotatef(horizontalRotation, currentModelViewMatrix[1], currentModelViewMatrix[5], currentModelViewMatrix[9]);
    glGetFloatv(GL_MODELVIEW_MATRIX, currentModelViewMatrix);
    // y axis rotation
    glRotatef(verticalRotation, currentModelViewMatrix[0], currentModelViewMatrix[4], currentModelViewMatrix[8]);
    glGetFloatv(GL_MODELVIEW_MATRIX, currentModelViewMatrix);
    glLoadMatrixf(currentModelViewMatrix);

    update();
}

void QtThreeDimensionalGlWidget::mousePressEvent(QMouseEvent *event) {
    switch(event->button()) {
    case Qt::LeftButton:
        setMouseTracking(true);
        lastPosition = event->pos();
        break;
    default:
        event->ignore();
        break;
    }
}

void QtThreeDimensionalGlWidget::mouseReleaseEvent(QMouseEvent *event) {
    switch(event->button()) {
    case Qt::LeftButton:
        setMouseTracking(false);
        break;
    default:
        event->ignore();
        break;
    }
}

void QtThreeDimensionalGlWidget::mouseMoveEvent(QMouseEvent *event) {
    horizontalRotation = (event->pos().x() - lastPosition.x()) * 1.0f;
    verticalRotation = (event->pos().y() - lastPosition.y()) * 1.0;

    lastPosition = event->pos();
    rotate();
}

void QtThreeDimensionalGlWidget::rotateLeft() {
    horizontalRotation = -5.0;
    verticalRotation = 0.0;
    rotate();
}

void QtThreeDimensionalGlWidget::rotateRight() {
    horizontalRotation = 5.0;
    verticalRotation = 0.0;
    rotate();
}

void QtThreeDimensionalGlWidget::rotateUp() {
    verticalRotation = -5.0;
    horizontalRotation = 0;
    rotate();
}

void QtThreeDimensionalGlWidget::rotateDown() {
    verticalRotation = 5.0;
    horizontalRotation = 0.0;
    rotate();
}
