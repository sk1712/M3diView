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

    void axialSliceChanged(int value);

    void sagittalSliceChanged(int value);

    void coronalSliceChanged(int value);

signals:

    void sliderValueChanged(int* value);
};

inline QtGlWidget* Qt3dViewerWidget::getGlWidget() const {
    return glWidget;
}

inline void Qt3dViewerWidget::setCurrentSlice(int *current) {
    glWidget->setCurrentSlice(current);

    axialSlider->setValue(current[2]);
    sagittalSlider->setValue(current[0]);
    coronalSlider->setValue(current[1]);
}

inline void Qt3dViewerWidget::setMaximumSlice(int *dim) {
    glWidget->setDimensions(dim);

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
