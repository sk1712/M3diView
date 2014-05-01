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

    Qt3dViewerWidget(QWidget *parent = 0);

    QtGlWidget* getGlWidget() const;

    /// set image dimensions in image coordinates
    void setDimensions(int *dim);

    /// update currentSlice
    void setCurrentSlice(int* current);

};

inline QtGlWidget* Qt3dViewerWidget::getGlWidget() const {
    return glWidget;
}

inline void Qt3dViewerWidget::setDimensions(int *dim) {
    glWidget->setDimensions(dim);
}

inline void Qt3dViewerWidget::setCurrentSlice(int *current) {
    glWidget->setCurrentSlice(current);
}

#endif // QT3DVIEWERWIDGET_H
