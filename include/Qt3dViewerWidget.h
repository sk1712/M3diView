#ifndef QT3DVIEWERWIDGET_H
#define QT3DVIEWERWIDGET_H

#include <QtViewerWidget.h>
#include <QtThreeDimensionalGlWidget.h>

class Qt3dViewerWidget : public QtViewerWidget
{
    Q_OBJECT

    /// OpenGL widget
    QtThreeDimensionalGlWidget *glWidget;

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
};

inline QtGlWidget* Qt3dViewerWidget::getGlWidget() const {
    return glWidget;
}

inline void Qt3dViewerWidget::setCurrentSlice(int *current) {
    glWidget->setCurrentSlice(current);
}

inline void Qt3dViewerWidget::setMaximumSlice(int *dim) {
    glWidget->setDimensions(dim);
}

inline void Qt3dViewerWidget::setEnabled(bool enabled) {
    glWidget->setEnabled(enabled);
}

#endif // QT3DVIEWERWIDGET_H
