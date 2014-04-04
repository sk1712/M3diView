#ifndef QTVIEWERWIDGET_H
#define QTVIEWERWIDGET_H

#include <QtTwoDimensionalGlWidget.h>

#include <QSlider>
#include <QLabel>
#include <QToolButton>


class QtViewerWidget : public QWidget
{
    Q_OBJECT

    /// OpenGL widget
    QtTwoDimensionalGlWidget *glWidget;

    /// slider for going through the slices
    QSlider *sliceSlider;

    /// toolbutton to expand/collapse viewer
    QToolButton *expandToolButton;

    //QToolButton *deleteToolButton;

    /// label showing number of slice
    QLabel *sliceLabel;

protected:

    /// slice currently shown
    int currentSlice;

    /// maximum number of slices
    int maximumSlice;

public:

    /// class constructor
    QtViewerWidget(QWidget *parent = 0);

    /// getter for OpenGL widget
    QtTwoDimensionalGlWidget* getGlWidget() const;

    /// getter for slice slider
    QSlider* getSlider() const;

    /// getter for slice label
    QLabel* getLabel() const;

    /// update currentSlice
    void setCurrentSlice(int current);

    /// update maximumSlice
    void setMaximumSlice(int maximum);

protected:

    /// update slice label
    void updateLabel();

private:

    /// initialize current and maximum slice
    void initializeParameters();

private slots:

    /// callback function for mouse wheel over OpenGL widget
    void changeSlider(int slice);

    /// callback function for slice slider change
    void updateSlice(int slice);

    /// callback function for expandToolButton
    void expandWindow();

    /// callback function for deleteToolButton
    void deleteWindow();

signals:

    /// signal emitted when viewer is to be full screen
    void windowExpanded();

    /// signal emitted when viewer is to be deleted
    void windowDeleted();
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
