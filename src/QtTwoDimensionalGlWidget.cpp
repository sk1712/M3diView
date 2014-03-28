#include <QtTwoDimensionalGlWidget.h>


QtTwoDimensionalGlWidget::QtTwoDimensionalGlWidget(QWidget *parent)
    :QtGlWidget(parent) {
    _drawable = NULL;
}

QtTwoDimensionalGlWidget::~QtTwoDimensionalGlWidget() {
    delete _drawable;
}

void QtTwoDimensionalGlWidget::drawCursor() {
    qglColor(Qt::green);
    glBegin(GL_LINES);
    glVertex2f(width()/2-10, height()/2);
    glVertex2f(width()/2+10, height()/2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(width()/2, height()/2-10);
    glVertex2f(width()/2, height()/2+10);
    glEnd();
}

void QtTwoDimensionalGlWidget::drawImage() {
    if (_drawable) {
        // Set raster position
        glRasterPos2f(0, 0);
        // Draw pixelmap
        glDrawPixels(width(), height(), GL_RGB, GL_UNSIGNED_BYTE,
                _drawable);
    }
}

void QtTwoDimensionalGlWidget::updateScene() {
    update();
}

void QtTwoDimensionalGlWidget::updateDrawable(irtkColor* drawable) {
    setDrawable(drawable);
    updateScene();
}

void QtTwoDimensionalGlWidget::initializeGL() {
    printf("Initializing GL \n");
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    qglClearColor(Qt::black);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void QtTwoDimensionalGlWidget::resizeGL(int w, int h) {
    printf("Resizing GL to width %d and height %d \n", w, h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double) w, 0.0, (double) h, -1.0, 1.0); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    emit resized(w, h);
}

void QtTwoDimensionalGlWidget::paintGL() {
    //printf("in paint gl \n");

    glClear(GL_COLOR_BUFFER_BIT);

    drawImage();
    drawCursor();
}


