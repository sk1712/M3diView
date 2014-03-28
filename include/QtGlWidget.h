#ifndef QTGLWIDGET_H
#define QTGLWIDGET_H

#include <QtOpenGL/QGLWidget>

#include <irtkLookupTable.h>

#include <cstdio>


class QtGlWidget : public QGLWidget
{
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    QtGlWidget(QWidget *parent = NULL);
    //virtual ~QtGlWidget();

    QSize sizeHint() const;

protected:
    virtual void drawImage() = 0;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void leftButtonPressed(int x, int y);
    void wheelMoved(int steps);
};

#endif // QTGLWIDGET_H
