#ifndef QT3DVIEWERWIDGET_H
#define QT3DVIEWERWIDGET_H

#include <QtViewerWidget.h>
#include <QtThreeDimensionalGlWidget.h>


class Qt3dViewerWidget : public QtViewerWidget
{
    Q_OBJECT

    /// OpenGL widget
    QtThreeDimensionalGlWidget *glWidget;

    /// slice sliders
    QSlider *axialSlider, *sagittalSlider, *coronalSlider;

    /// slices currently shown
    int currentSlice[3];

public:

    /// class constructor
    Qt3dViewerWidget(QWidget *parent = 0);

    /// class destructor
    QtGlWidget* getGlWidget() const;

    /// update currentSlice
    void setCurrentSlice(int* current);

    /// set image dimensions in image coordinates
    void setMaximumSlice(int *dim);

    /// set viewer enabled
    void setEnabled(bool enabled);

protected:

    /// connect signals to slots
    void connectSignals();

private slots:

    /// callback function when axial slider value changes
    void axialSliceChanged(int value);

    /// callback function when sagittal slider value changes
    void sagittalSliceChanged(int value);

    /// callback function when coronal slider value changes
    void coronalSliceChanged(int value);

signals:

    /// signal emitted when one of the sliders' value changes
    void sliderValueChanged(int* value);
};

inline QtGlWidget* Qt3dViewerWidget::getGlWidget() const {
    return glWidget;
}

inline void Qt3dViewerWidget::setCurrentSlice(int *current) {
    glWidget->setCurrentSlice(current);

    // current = current slice in X([0]), Y([1]) and Z([2]) direction
    axialSlider->setValue(current[2]);
    sagittalSlider->setValue(current[0]);
    coronalSlider->setValue(current[1]);
}

inline void Qt3dViewerWidget::setMaximumSlice(int *dim) {
    glWidget->setDimensions(dim);

    // dim = number of slices in X([0]), Y([1]) and Z([2]) direction
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
