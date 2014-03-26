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
    QtTwoDimensionalGlWidget* getGlWidget();
    QSlider* getSlider();
    QLabel* getLabel();

    void setCurrentSlice(int current);
    void setMaximumSlice(int maximum);

protected:
    void updateLabel();

private:
    void initializeParameters();

private slots:
    void changeSlider(int slice);
    void updateSlice(int slice);
};


inline QtTwoDimensionalGlWidget* QtViewerWidget::getGlWidget() {
    return glWidget;
}

inline QSlider* QtViewerWidget::getSlider() {
    return sliceSlider;
}

inline QLabel* QtViewerWidget::getLabel() {
    return sliceLabel;
}

inline void QtViewerWidget::setCurrentSlice(int current) {
    sliceSlider->setValue(current);
}

inline void QtViewerWidget::setMaximumSlice(int maximum) {
    maximumSlice = maximum;
    sliceSlider->setMaximum(maximumSlice);
}

#endif // QTVIEWERWIDGET_H
