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

    /// Slider for going through the slices
    QSlider *sliceSlider;

    /// Label showing number of slice
    QLabel *sliceLabel;

    /// Slice currently shown
    int currentSlice;

    /// Maximum number of slices
    int maximumSlice;

public:

    /// Class constructor
    Qt2dViewerWidget(QWidget *parent = 0);

    /// Class destructor
    QtGlWidget* getGlWidget() const;

    /// Update currentSlice
    void setCurrentSlice(int* current);

    /// Update maximumSlice
    void setMaximumSlice(int* maximum);

    /// Set whether viewer is enabled
    void setEnabled(bool enabled);

    /// Set viewer labels
    void setLabels(const char t, const char b, const char l, const char r);

    /// Set object name (used for styling the widgets
    void setObjectName(QString objectName);

protected:

    /// Update slice label
    void updateLabel();

private:

    /// Initialize current and maximum slice
    void initializeParameters();

    /// Connect signals
    void connectSignals();

private slots:

    /// Callback function for mouse wheel over OpenGL widget
    void changeSlider(int slice);

    /// Callback function for slice slider change
    void updateSlice(int slice);

signals:

    /// Signal emitted when slider value changes
    void sliderValueChanged(int* value);

};


inline QtGlWidget* Qt2dViewerWidget::getGlWidget() const {
    return glWidget;
}

inline void Qt2dViewerWidget::setCurrentSlice(int* current) {
    sliceSlider->setValue(current[0]);
}

inline void Qt2dViewerWidget::setMaximumSlice(int* maximum) {
    maximumSlice = maximum[0];
    sliceSlider->setMaximum(maximumSlice);
}

inline void Qt2dViewerWidget::setEnabled(bool enabled) {
    glWidget->setEnabled(enabled);
    sliceSlider->setEnabled(enabled);
}

inline void Qt2dViewerWidget::setLabels(const char t, const char b, const char l, const char r) {
    glWidget->setLabels(t, b, l, r);
}

inline void Qt2dViewerWidget::setObjectName(QString objectName) {
    sliceSlider->setObjectName(objectName);
}

#endif // QT2DVIEWERWIDGET_H
