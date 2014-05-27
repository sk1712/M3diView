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

    /// set viewer labels
    void setLabels(const char t, const char b, const char l, const char r);

    /// update the images to be drawn
    void updateDrawable(QVector<QRgb**> drawable);

protected:

    /// function to initialize OpenGL inherited from QGLWidget
    void initializeGL();

    /// function called whenever OpenGL scene is resized inherited from QGLWidget
    void resizeGL(int w, int h);

    /// function called whenever scene is painted inherited from QGLWidget
    void paintGL();

    /// delete the drawable
    void deleteDrawable();

private:

    /// draw image on screen
    void drawImage() const;

    /// draw cursor on screen
    void drawCursor() const;

    /// draw four labels on screen
    void drawLabels();

};


inline void QtTwoDimensionalGlWidget::setLabels(char t, char b, char l, char r) {
    top = QString(QChar(t));
    bottom = QString(QChar(b));
    left = QString(QChar(l));
    right = QString(QChar(r));
}

#endif // QTTWODIMENSIONALGLWIDGET_H
