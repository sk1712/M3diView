#ifndef QTTWODIMENSIONALGLWIDGET_H
#define QTTWODIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

class QtTwoDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    irtkColor* _drawable;

public:
    QtTwoDimensionalGlWidget(QWidget *parent = NULL);
    ~QtTwoDimensionalGlWidget();

    void drawImage();
    void drawCursor();
    void setDrawable(irtkColor* drawable);
    void updateScene();

public slots:
    void updateDrawable(irtkColor* drawable);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

signals:
    void resized(int w, int h);
};

inline void QtTwoDimensionalGlWidget::setDrawable(irtkColor *drawable) {
    delete _drawable;
    _drawable = drawable;
}

#endif // QTTWODIMENSIONALGLWIDGET_H
