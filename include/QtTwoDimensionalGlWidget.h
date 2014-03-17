#ifndef QTTWODIMENSIONALGLWIDGET_H
#define QTTWODIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

class QtTwoDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

public:
    QtTwoDimensionalGlWidget(QWidget *parent = NULL);
    //~QtTwoDimensionalGlWidget();

    void drawImage(QColor *drawable);
    void drawCursor();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // QTTWODIMENSIONALGLWIDGET_H
