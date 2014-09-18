#include <QtThreeDimensionalGlWidget.h>

#include <QMouseEvent>

QtThreeDimensionalGlWidget::QtThreeDimensionalGlWidget(QWidget *parent)
    :QtGlWidget(parent) {
    // Rotate model when first displaying
    horizontalRotation = 5.0f;
    verticalRotation = 5.0f;
    cameraFOV = 45.0f;

    // Initialize rotation and zoom flags to false
    doRotation = false;
    moveCamera = false;

    connectSignals();
}

QtThreeDimensionalGlWidget::~QtThreeDimensionalGlWidget() {
    deleteDrawable();
}

void QtThreeDimensionalGlWidget::initializeGL() {
    // Set background depth to farthest
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_FLAT); // Enable flat shading
    // Nice perspective corrections
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    qglClearColor(Qt::black);
}

void QtThreeDimensionalGlWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Move into the screen
    glTranslatef(0.0f, 0.0f, -400.0f);
    // Rotate about (0,1,0)-axis
    glRotatef((GLfloat) horizontalRotation, 0.0f, 1.0f, 0.0f);
    // Rotate about (1,0,0)-axis
    glRotatef((GLfloat) verticalRotation, 1.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    GLfloat aspect = (GLfloat) w / (GLfloat) h;
    width = w; height = h;
    glLoadIdentity();
    gluPerspective(cameraFOV, aspect, 0.1f, 1000.0f);
}

void QtThreeDimensionalGlWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (doRotation) {
        glMatrixMode(GL_MODELVIEW);
        GLfloat currentModelViewMatrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, currentModelViewMatrix);
        // X axis rotation
        glRotatef(horizontalRotation, currentModelViewMatrix[1],
        currentModelViewMatrix[5], currentModelViewMatrix[9]);
        glGetFloatv(GL_MODELVIEW_MATRIX, currentModelViewMatrix);
        // Y axis rotation
        glRotatef(verticalRotation, currentModelViewMatrix[0],
        currentModelViewMatrix[4], currentModelViewMatrix[8]);
        glGetFloatv(GL_MODELVIEW_MATRIX, currentModelViewMatrix);
        glLoadMatrixf(currentModelViewMatrix);
    }

    if (moveCamera) {
        // Move camera towards/away from the screen
        glMatrixMode(GL_PROJECTION);
        GLfloat aspect = (GLfloat) width / (GLfloat) height;
        glLoadIdentity();
        gluPerspective(cameraFOV, aspect, 0.1f, 1000.0f);
    }

    if (!_drawable.empty()) {
        createTextures();
        drawImage();
        drawBorders();
    }

    if (imageOriginVisible && !_drawable.empty()) drawImageOrigin();
}

void QtThreeDimensionalGlWidget::rotate() {
    doRotation = true;
    updateGL();
    doRotation = false;
}

void QtThreeDimensionalGlWidget::deleteDrawable() {
    QVector<QRgb**>::iterator it;

    for (it = _drawable.begin(); it != _drawable.end(); ++it) {
        for (int i = 0; i < 3; i++)
            delete [] (*it)[i];
        delete [] (*it);
    }

    _drawable.clear();
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

void QtThreeDimensionalGlWidget::drawImage() const {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Begin drawing the perpendicular surfaces

    float width, height, slice;

    // Axial view
    width = (float) dimensions[0]/2;
    height = (float) dimensions[1]/2;
    slice = currentSlice[2] - dimensions[2]/2.0;
    if (invertedAxis[2]) slice *= -1;

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(-height, slice, -width);
    glTexCoord2f(0.0, 1.0); glVertex3f( height, slice, -width);
    glTexCoord2f(1.0, 1.0); glVertex3f( height, slice,  width);
    glTexCoord2f(1.0, 0.0); glVertex3f(-height, slice,  width);
    glEnd();

    // Sagittal view
    width = (float) dimensions[1]/2;
    height = (float) dimensions[2]/2;
    slice = currentSlice[0] - dimensions[0]/2.0;
    if (invertedAxis[0]) slice *= -1;

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-width, -height, slice);
    glTexCoord2f(0.0, 1.0); glVertex3f(-width,  height, slice);
    glTexCoord2f(1.0, 1.0); glVertex3f( width,  height, slice);
    glTexCoord2f(1.0, 0.0); glVertex3f( width, -height, slice);
    glEnd();

    // Coronal view
    width = (float) dimensions[0]/2;
    height = (float) dimensions[2]/2;
    slice = currentSlice[1] - dimensions[1]/2.0;
    if (invertedAxis[1]) slice *= -1;

    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(slice, -height, -width);
    glTexCoord2f(0.0, 1.0); glVertex3f(slice,  height, -width);
    glTexCoord2f(1.0, 1.0); glVertex3f(slice,  height,  width);
    glTexCoord2f(1.0, 0.0); glVertex3f(slice, -height,  width);
    glEnd();  // End of drawing color-cube

    glDisable(GL_TEXTURE_2D);
}

void QtThreeDimensionalGlWidget::drawBorders() {
    float width, height, slice;

    // Surround axial view with yellow lines
    width = (float) dimensions[0]/2;
    height = (float) dimensions[1]/2;
    slice = currentSlice[2] - dimensions[2]/2.0;
    if (invertedAxis[2]) slice *= -1;

    qglColor(Qt::yellow);
    glBegin(GL_LINES);
    glVertex3f(-height, slice, -width);
    glVertex3f( height, slice, -width);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f( height, slice, -width);
    glVertex3f( height, slice,  width);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f( height, slice,  width);
    glVertex3f(-height, slice,  width);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-height, slice,  width);
    glVertex3f(-height, slice, -width);
    glEnd();

    // Surround sagittal view with red lines
    width = (float) dimensions[1]/2;
    height = (float) dimensions[2]/2;
    slice = currentSlice[0] - dimensions[0]/2.0;
    if (invertedAxis[0]) slice *= -1;

    qglColor(Qt::red);
    glBegin(GL_LINES);
    glVertex3f(-width, -height, slice);
    glVertex3f(-width,  height, slice);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-width,  height, slice);
    glVertex3f( width,  height, slice);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f( width,  height, slice);
    glVertex3f( width, -height, slice);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f( width, -height, slice);
    glVertex3f(-width, -height, slice);
    glEnd();

    // Surround coronal view with green lines
    width = (float) dimensions[0]/2;
    height = (float) dimensions[2]/2;
    slice = currentSlice[1] - dimensions[1]/2.0;
    if (invertedAxis[1]) slice *= -1;

    qglColor(Qt::green);
    glBegin(GL_LINES);
    glVertex3f(slice, -height, -width);
    glVertex3f(slice,  height, -width);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(slice,  height, -width);
    glVertex3f(slice,  height,  width);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(slice,  height,  width);
    glVertex3f(slice, -height,  width);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(slice, -height,  width);
    glVertex3f(slice, -height, -width);
    glEnd();
}

void QtThreeDimensionalGlWidget::createTextures() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // the following line might be causing error on MacOS X
    // glDeleteTextures(3, textures);
    glGenTextures(3, textures);

    // Create texture for axial view
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions[0],
            dimensions[1], 0, GL_BGRA, GL_UNSIGNED_BYTE,
            _drawable[0][0]);

    // Create texture for sagittal view
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions[1],
            dimensions[2], 0, GL_BGRA, GL_UNSIGNED_BYTE,
            _drawable[0][1]);

    // Create texture for coronal view
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions[0],
            dimensions[2], 0, GL_BGRA, GL_UNSIGNED_BYTE,
            _drawable[0][2]);

    glDisable(GL_TEXTURE_2D);
}

void QtThreeDimensionalGlWidget::connectSignals() {
    // Signals for rotation
    connect(this, SIGNAL(leftKeyPressed()), this, SLOT(rotateLeft()));
    connect(this, SIGNAL(rightKeyPressed()), this, SLOT(rotateRight()));
    connect(this, SIGNAL(upKeyPressed()), this, SLOT(rotateUp()));
    connect(this, SIGNAL(downKeyPressed()), this, SLOT(rotateDown()));
    // Signals for zoom in/out
    connect(this, SIGNAL(wheelMoved(int)), this, SLOT(changeZoom(int)));
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

void QtThreeDimensionalGlWidget::changeZoom(int numSteps) {
    moveCamera = true;
    cameraFOV -= numSteps*2.5;
    updateGL();
    moveCamera = false;
}
