#include <QtTwoDimensionalGlWidget.h>


QtTwoDimensionalGlWidget::QtTwoDimensionalGlWidget(QWidget *parent)
    :QtGlWidget(parent) {
    _drawable.clear();
    cursorVisible = true;
    labelsVisible = true;
}

QtTwoDimensionalGlWidget::~QtTwoDimensionalGlWidget() {
    deleteDrawable();
}

void QtTwoDimensionalGlWidget::setLabelsVisible(bool visible) {
    labelsVisible = visible;
    update();
}

void QtTwoDimensionalGlWidget::setCursorVisible(bool visible) {
    cursorVisible = visible;
    update();
}

void QtTwoDimensionalGlWidget::updateDrawable(QVector<QRgb**> drawable) {
    QtGlWidget::updateDrawable(drawable);
    updateGL();
}

void QtTwoDimensionalGlWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    qglClearColor(Qt::black);
}

void QtTwoDimensionalGlWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set origin to bottom left corner
    glOrtho(0.0, (double) w, 0.0, (double) h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    emit resized(w, h);
}

void QtTwoDimensionalGlWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    _width = customWidth();
    _height = customHeight();

    if (!_drawable.empty()) drawImage();
    if (cursorVisible) drawCursor();
    if (labelsVisible) drawLabels();
}

void QtTwoDimensionalGlWidget::drawImage() const {
    QVector<QRgb**>::const_iterator rit;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Draw last image in the list first and the others on top of it
    for (rit = _drawable.constEnd()-1; rit >= _drawable.constBegin(); rit--) {
        // Set raster position
        glRasterPos2f(0, 0);
        // Draw pixelmap
        glDrawPixels(_width, _height, GL_BGRA, GL_UNSIGNED_BYTE,
                     (*rit)[0]);
    }
}

void QtTwoDimensionalGlWidget::drawCursor() const {
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

void QtTwoDimensionalGlWidget::deleteDrawable() {
    QVector<QRgb**>::iterator it;

    for (it = _drawable.begin(); it != _drawable.end(); it++) {
        delete [] (*it)[0];
        delete it[0];
    }

    _drawable.clear();
}


