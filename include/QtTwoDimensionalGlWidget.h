#ifndef QTTWODIMENSIONALGLWIDGET_H
#define QTTWODIMENSIONALGLWIDGET_H

#include <QtGlWidget.h>

class QtTwoDimensionalGlWidget : public QtGlWidget
{
    Q_OBJECT

    irtkColor* _drawable;

    /// viewer labels
    QString top, bottom, left, right;

public:
    QtTwoDimensionalGlWidget(QWidget *parent = NULL);
    ~QtTwoDimensionalGlWidget();

    void drawImage();
    void drawCursor();
    void drawLabels();

    void setDrawable(irtkColor* drawable);
    void setLabels(char t, char b, char l, char r);
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

inline void QtTwoDimensionalGlWidget::setLabels(char t, char b, char l, char r) {
    top = QString(QChar(t));
    bottom = QString(QChar(b));
    left = QString(QChar(l));
    right = QString(QChar(r));
}

#endif // QTTWODIMENSIONALGLWIDGET_H
