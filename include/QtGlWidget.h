#ifndef QTGLWIDGET_H
#define QTGLWIDGET_H

#include <QGLWidget>
#include <QWindow>

#include <irtkLookupTable.h>

#include <cstdio>


class QtGlWidget : public QGLWidget
{
    Q_OBJECT // must include this if you use Qt signals/slots

protected:
    int _width, _height;
    int pixelRatio;

public:
    QtGlWidget(QWidget *parent = NULL);
    //virtual ~QtGlWidget();

    QSize sizeHint() const;
    int customWidth() const;
    int customHeight() const;

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
