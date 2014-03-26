#ifndef QTTWODIMENSIONALGLWIDGET_H
#define QTTWODIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

class QtTwoDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    bool initialized;

public:
    QtTwoDimensionalGlWidget(QWidget *parent = NULL);
    //~QtTwoDimensionalGlWidget();

    void drawImage(irtkColor *drawable);
    void drawCursor();

public slots:
    void updateImage(irtkColor *drawable);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

signals:
    void resized(int w, int h);
};

#endif // QTTWODIMENSIONALGLWIDGET_H
