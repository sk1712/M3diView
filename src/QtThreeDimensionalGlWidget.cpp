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

QtThreeDimensionalGlWidget::~QtThreeDimensionalGlWidget() {
    deleteDrawable();
}

void QtThreeDimensionalGlWidget::drawImage() {
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Begin drawing the perpendicular surfaces

    int max = dimensions[0];
    for (int i = 1; i < 3; i++)
        if (max < dimensions[i]) max = dimensions[i];
    float width, height;

    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    width = (float) dimensions[0]/max;
    height = (float) dimensions[1]/max;
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-height, 0.0f, -width);
    glTexCoord2f(0.0, 1.0); glVertex3f( height, 0.0f, -width);
    glTexCoord2f(1.0, 1.0); glVertex3f( height, 0.0f,  width);
    glTexCoord2f(1.0, 0.0); glVertex3f(-height, 0.0f,  width);
    glEnd();

    // Front face  (z = 1.0f)
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    width = (float) dimensions[1]/max;
    height = (float) dimensions[2]/max;
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, 0.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-width,  height, 0.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f( width,  height, 0.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f( width, -height, 0.0f);
    glEnd();

    // Right face (x = 1.0f)
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glBegin(GL_QUADS);
    width = (float) dimensions[0]/max;
    height = (float) dimensions[2]/max;
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0f, -height, -width);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0f,  height, -width);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0f,  height,  width);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0f, -height,  width);
    glEnd();  // End of drawing color-cube

    glFlush();
}

void QtThreeDimensionalGlWidget::createTextures() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(3, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions[0],
            dimensions[1], 0, GL_RGBA, GL_UNSIGNED_BYTE,
            _drawable[0][0]);

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions[1],
            dimensions[2], 0, GL_RGBA, GL_UNSIGNED_BYTE,
            _drawable[0][1]);

    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions[0],
            dimensions[2], 0, GL_RGBA, GL_UNSIGNED_BYTE,
            _drawable[0][2]);
}

void QtThreeDimensionalGlWidget::deleteDrawable() {
    QVector<QRgb**>::iterator it;

    for (it = _drawable.begin(); it != _drawable.end(); it++) {
        for (int i = 0; i < 3; i++)
            delete [] (*it)[i];
        //delete [] *it;
    }

    _drawable.clear();
}

void QtThreeDimensionalGlWidget::updateDrawable(QVector<QRgb**> drawable) {
    QtGlWidget::updateDrawable(drawable);
    createTextures();

}

void QtThreeDimensionalGlWidget::initializeGL() {
    // Set background depth to farthest
    glClearDepth(1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_FLAT); // Enable smooth shading
    // Nice perspective corrections
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    qglClearColor(Qt::black);
}

void QtThreeDimensionalGlWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);  // Move into the screen
    glRotatef((GLfloat) horizontalRotation, 0.0f, 1.0f, 0.0f);  // Rotate about (0,1,0)-axis
    glRotatef((GLfloat) verticalRotation, 1.0f, 0.0f, 0.0f);  // Rotate about (1,0,0)-axis
    glMatrixMode(GL_PROJECTION);
    GLfloat aspect = (GLfloat) w / (GLfloat) h;
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void QtThreeDimensionalGlWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!_drawable.empty()) {
        drawImage();
    }
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
