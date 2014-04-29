#ifndef QTTWODIMENSIONALGLWIDGET_H
#define QTTWODIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

#include <QVector>

class QtTwoDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    /// viewer labels
    QString top, bottom, left, right;

public:

    /// class constructor
    QtTwoDimensionalGlWidget(QWidget *parent = NULL);

    /// class destructor
    ~QtTwoDimensionalGlWidget();

    /// draw image on screen
    void drawImage();

    /// draw cursor on screen
    void drawCursor();

    /// draw four labels on screen
    void drawLabels();

    /// set viewer labels
    void setLabels(const char t, const char b, const char l, const char r);

protected:

    void deleteDrawable();

    /// function to initialize OpenGL inherited from QGLWidget
    void initializeGL();

    /// function called whenever OpenGL scene is resized inherited from QGLWidget
    void resizeGL(int w, int h);

    /// function called whenever scene is painted inherited from QGLWidget
    void paintGL();

signals:

    /// signal emitted when widget is resized
    void resized(int w, int h);
};


inline void QtTwoDimensionalGlWidget::setLabels(char t, char b, char l, char r) {
    top = QString(QChar(t));
    bottom = QString(QChar(b));
    left = QString(QChar(l));
    right = QString(QChar(r));
}

#endif // QTTWODIMENSIONALGLWIDGET_H
