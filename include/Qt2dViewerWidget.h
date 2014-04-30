#ifndef QT2DVIEWERWIDGET_H
#define QT2DVIEWERWIDGET_H

#include <QtViewerWidget.h>
#include <QtTwoDimensionalGlWidget.h>

#include <QLabel>

class Qt2dViewerWidget : public QtViewerWidget
{
    Q_OBJECT

    /// OpenGL widget
    QtTwoDimensionalGlWidget *glWidget;

    /// slider for going through the slices
    QSlider *sliceSlider;

    /// label showing number of slice
    QLabel *sliceLabel;

protected:

    /// slice currently shown
    int currentSlice;

    /// maximum number of slices
    int maximumSlice;

public:

    Qt2dViewerWidget(QWidget *parent = 0);

    QtGlWidget* getGlWidget() const;

    /// getter for slice slider
    QSlider* getSlider() const;

    /// getter for slice label
    QLabel* getLabel() const;

    /// update currentSlice
    void setCurrentSlice(int* current);

    /// update maximumSlice
    void setMaximumSlice(int maximum);

    void setLabels(const char t, const char b, const char l, const char r);

protected:

    /// update slice label
    void updateLabel();

private:

    /// initialize current and maximum slice
    void initializeParameters();

    /// connect signals
    virtual void connectSignals();

private slots:

    /// callback function for mouse wheel over OpenGL widget
    void changeSlider(int slice);

    /// callback function for slice slider change
    void updateSlice(int slice);

};


inline QtGlWidget* Qt2dViewerWidget::getGlWidget() const {
    return glWidget;
}

inline QSlider* Qt2dViewerWidget::getSlider() const {
    return sliceSlider;
}

inline QLabel* Qt2dViewerWidget::getLabel() const {
    return sliceLabel;
}

inline void Qt2dViewerWidget::setCurrentSlice(int* current) {
    sliceSlider->setValue(current[0]);
}

inline void Qt2dViewerWidget::setMaximumSlice(int maximum) {
    maximumSlice = maximum;
    sliceSlider->setMaximum(maximumSlice);
}

inline void Qt2dViewerWidget::setLabels(const char t, const char b, const char l, const char r) {
    glWidget->setLabels(t, b, l, r);
}

#endif // QT2DVIEWERWIDGET_H
