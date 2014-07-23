#ifndef QT3DVIEWERWIDGET_H
#define QT3DVIEWERWIDGET_H

#include <QtViewerWidget.h>
#include <QtThreeDimensionalGlWidget.h>


class Qt3dViewerWidget : public QtViewerWidget
{
    Q_OBJECT

    /// OpenGL widget
    QtThreeDimensionalGlWidget *glWidget;

    /// Slice sliders
    QSlider *axialSlider, *sagittalSlider, *coronalSlider;

    /// Slices currently shown
    int currentSlice[3];

public:

    /// Class constructor
    Qt3dViewerWidget(QWidget *parent = 0);

    /// Class destructor
    QtGlWidget* getGlWidget() const;

    /// Update currentSlice
    void setCurrentSlice(int* current);

    /// Set image dimensions in image coordinates
    void setMaximumSlice(int *dim);

    /// Set inverted axes attribute
    void setInvertedAxes(bool *inverted);

    /// Set viewer enabled
    void setEnabled(bool enabled);

protected:

    /// Create component widgets
    void createWidgets();

    /// Connect signals to slots
    void connectSignals();

private slots:

    /// Callback function when axial slider value changes
    void axialSliceChanged(int value);

    /// Callback function when sagittal slider value changes
    void sagittalSliceChanged(int value);

    /// Callback function when coronal slider value changes
    void coronalSliceChanged(int value);

signals:

    /// Signal emitted when one of the sliders' value changes
    void sliderValueChanged(int* value);
};

inline QtGlWidget* Qt3dViewerWidget::getGlWidget() const {
    return glWidget;
}

inline void Qt3dViewerWidget::setCurrentSlice(int *current) {
    glWidget->setCurrentSlice(current);

    // current = current slice in sagittal([0]), coronal([1])
    // and axial([2]) direction
    axialSlider->setValue(current[2]);
    sagittalSlider->setValue(current[0]);
    coronalSlider->setValue(current[1]);
}

inline void Qt3dViewerWidget::setMaximumSlice(int *dim) {
    glWidget->setDimensions(dim);

    // dim = number of slices
    axialSlider->setMaximum(dim[2]);
    sagittalSlider->setMaximum(dim[0]);
    coronalSlider->setMaximum(dim[1]);
}

inline void Qt3dViewerWidget::setEnabled(bool enabled) {
    glWidget->setEnabled(enabled);
    axialSlider->setEnabled(enabled);
    sagittalSlider->setEnabled(enabled);
    coronalSlider->setEnabled(enabled);
}

#endif // QT3DVIEWERWIDGET_H
