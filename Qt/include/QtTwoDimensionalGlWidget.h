#ifndef QTTWODIMENSIONALGLWIDGET_H
#define QTTWODIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

#include <QVector>


class QtTwoDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    /// Viewer labels
    QString top, bottom, left, right;

    /// Cursor, labels visible flags
    bool cursorVisible, labelsVisible;

public:

    /// Class constructor
    QtTwoDimensionalGlWidget(QWidget *parent = NULL);

    /// Class destructor
    ~QtTwoDimensionalGlWidget();

    /// Set viewer labels
    void setLabels(const char t, const char b, const char l, const char r);

    /// Set labels visible
    void setLabelsVisible(bool visible);

    /// Set cursor visible
    void setCursorVisible(bool visible);

protected:

    /// Function to initialize OpenGL inherited from QGLWidget
    void initializeGL();

    /// Function called whenever OpenGL scene is resized inherited from QGLWidget
    void resizeGL(int w, int h);

    /// Function called whenever scene is painted inherited from QGLWidget
    void paintGL();

    /// Delete the drawable
    void deleteDrawable();

private:

    /// Draw image on screen
    void drawImage() const;

    /// Draw cursor on screen
    void drawCursor() const;

    /// Draw four labels on screen
    void drawLabels();

};


inline void QtTwoDimensionalGlWidget::setLabels(char t, char b, char l, char r) {
    top = QString(QChar(t));
    bottom = QString(QChar(b));
    left = QString(QChar(l));
    right = QString(QChar(r));
}

#endif // QTTWODIMENSIONALGLWIDGET_H
