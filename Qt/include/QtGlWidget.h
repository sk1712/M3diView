#ifndef QTGLWIDGET_H
#define QTGLWIDGET_H

#include <QGLWidget>
#include <QWindow>
#include <QColor>

#ifdef Q_OS_MAC
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#else
#include "GL/gl.h"
#include "GL/glu.h"
#endif


class QtGlWidget : public QGLWidget
{
    Q_OBJECT // Must include this if you use Qt signals/slots

protected:

    /// Values to be drawn on the screen
    QVector<QRgb**> _drawable;

    /// Width and height in physical pixels
    int _width, _height;

    /// Device pixel ratio (=2 for retina display)
    int pixelRatio;

public:

    /// Class constructor
    QtGlWidget(QWidget *parent = NULL);

    /// Class destructor
    //virtual ~QtGlWidget();

    /// Size hint for widget
    QSize sizeHint() const;

    /// Get width in physical pixels
    int customWidth() const;

    /// Get height in physical pixels
    int customHeight() const;

    /// Save screenshot in file
    bool saveScreenshotInFile(QString file);

    /// Get image displayed in widget as QImage
    QImage getDisplayedImage();

    /// Function to be implemented by all derived classes
    virtual void drawImage() const = 0;

public slots:

    /// Callback function to update drawable
    virtual void updateDrawable(QVector<QRgb**> drawable);

protected:

    /// Delete the drawable
    virtual void deleteDrawable() = 0;

    /// Function handling mouse press events
    void mousePressEvent(QMouseEvent *event);

    /// Function handling mouse wheel events
    void wheelEvent(QWheelEvent *event);

    /// Function handling key press events
    void keyPressEvent(QKeyEvent *event);

signals:

    /// Signal emitted when left button is pressed
    void leftButtonPressed(int x, int y);

    /// Signal emitted when mouse wheel moves
    void wheelMoved(int steps);

    /// Signals emitted when arrow buttons are pressed
    void leftKeyPressed();
    void rightKeyPressed();
    void upKeyPressed();
    void downKeyPressed();

    /// Signal emitted when widget is resized
    void resized(int w, int h);
};

#endif // QTGLWIDGET_H
