#ifndef QTVIEWERWIDGET_H
#define QTVIEWERWIDGET_H

#include <QtTwoDimensionalGlWidget.h>

#include <QSlider>
#include <QLabel>
#include <QToolButton>

class QtViewerWidget : public QWidget
{
    Q_OBJECT

    QtTwoDimensionalGlWidget *glWidget;
    QSlider *sliceSlider;
    QToolButton *expandToolButton;
    QLabel *sliceLabel;

protected:
    int currentSlice;
    int maximumSlice;

public:
    QtViewerWidget(QWidget *parent = 0);
    //~QtViewerWidget();
    QtTwoDimensionalGlWidget* getGlWidget() const;
    QSlider* getSlider() const;
    QLabel* getLabel() const;

    void setCurrentSlice(int current);
    void setMaximumSlice(int maximum);

protected:
    void updateLabel();

private:
    void initializeParameters();

private slots:
    void changeSlider(int slice);
    void updateSlice(int slice);
    void expandWindow();

signals:
    void windowExpanded();
};


inline QtTwoDimensionalGlWidget* QtViewerWidget::getGlWidget() const {
    return glWidget;
}

inline QSlider* QtViewerWidget::getSlider() const {
    return sliceSlider;
}

inline QLabel* QtViewerWidget::getLabel() const {
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
