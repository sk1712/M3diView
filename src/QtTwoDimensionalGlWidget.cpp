#include <QtTwoDimensionalGlWidget.h>

QtTwoDimensionalGlWidget::QtTwoDimensionalGlWidget(QWidget *parent)
    :QtGlWidget(parent) {
    _drawable = NULL;
}

QtTwoDimensionalGlWidget::~QtTwoDimensionalGlWidget() {
    delete _drawable;
}

void QtTwoDimensionalGlWidget::drawImage() {
    if (_drawable) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // Set raster position
        glRasterPos2f(0, 0);
        // Draw pixelmap
        glDrawPixels(_width, _height, GL_RGBA, GL_UNSIGNED_BYTE,
                _drawable);
    }
}

void QtTwoDimensionalGlWidget::drawCursor() {
    qglColor(Qt::green);
    glBegin(GL_LINES);
    glVertex2f(_width/2-10, _height/2);
    glVertex2f(_width/2+10, _height/2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(_width/2, _height/2-10);
    glVertex2f(_width/2, _height/2+10);
    glEnd();
}

void QtTwoDimensionalGlWidget::drawLabels() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    qglColor(Qt::green);
    QFont arialFont("Arial", 12, QFont::Bold, false);

    renderText(width()/2-5, 15, top, arialFont);
    renderText(width()/2-5, height()-5, bottom, arialFont);
    renderText(5, height()/2+5, left, arialFont);
    renderText(width()-15, height()/2+5, right, arialFont);
}

//void QtTwoDimensionalGlWidget::updateDrawable(irtkColor* drawable) {
void QtTwoDimensionalGlWidget::updateDrawable(QRgb* drawable) {
    delete _drawable;
    _drawable = drawable;
    update();
}

void QtTwoDimensionalGlWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    qglClearColor(Qt::black);
}

void QtTwoDimensionalGlWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // set origin to bottom left corner
    glOrtho(0.0, (double) w, 0.0, (double) h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    emit resized(w, h);
}

void QtTwoDimensionalGlWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    _width = customWidth();
    _height = customHeight();

    drawImage();
    drawCursor();
    drawLabels();
}


