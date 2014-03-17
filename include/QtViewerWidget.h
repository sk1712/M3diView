#ifndef QTVIEWERWIDGET_H
#define QTVIEWERWIDGET_H

#include <QtTwoDimensionalGlWidget.h>

#include <QSlider>
#include <QLabel>

class QtViewerWidget : public QWidget
{
    Q_OBJECT

    QtTwoDimensionalGlWidget *glWidget;
    QSlider *sliceSlider;
    QLabel *sliceLabel;

protected:
    int currentSlice;
    int maximumSlice;

public:
    QtViewerWidget(QWidget *parent = 0);
    //~QtViewerWidget();

private:
    void initializeParameters();

private slots:
    void changeSlice(int slice);
};

#endif // QTVIEWERWIDGET_H
