#ifndef QTGLWIDGET_H
#define QTGLWIDGET_H

#include <QGLWidget>
#include <QWindow>
#include <QColor>

class QtGlWidget : public QGLWidget
{
    Q_OBJECT // must include this if you use Qt signals/slots

protected:

    /// array of values to be drawn on the screen
    QVector<QRgb**> _drawable;

    /// width and height in physical pixels
    int _width, _height;

    /// device pixel ratio (=2 for retina display)
    int pixelRatio;

public:

    /// class constructor
    QtGlWidget(QWidget *parent = NULL);

    /// class destructor
    //virtual ~QtGlWidget();

    /// size hint for widget
    QSize sizeHint() const;

    /// get width in physical pixels
    int customWidth() const;

    /// get height in physical pixels
    int customHeight() const;

    /// function to be implemented by all derived classes
    virtual void drawImage() = 0;

public slots:

    /// callback function to update drawable
    virtual void updateDrawable(QVector<QRgb**> drawable);

protected:

    virtual void deleteDrawable() = 0;

    /// function handling mouse press events
    void mousePressEvent(QMouseEvent *event);

    /// function handling mouse wheel events
    void wheelEvent(QWheelEvent *event);

    /// function handling key press events
    void keyPressEvent(QKeyEvent *event);

signals:

    /// signal emitted when left button is pressed
    void leftButtonPressed(int x, int y);

    /// signal emitted when mouse wheel moves
    void wheelMoved(int steps);

    /// signals emitted when arrow buttons are pressed
    void leftKeyPressed();
    void rightKeyPressed();
    void upKeyPressed();
    void downKeyPressed();

};

#endif // QTGLWIDGET_H
