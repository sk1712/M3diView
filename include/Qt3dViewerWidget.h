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

};

inline QtGlWidget* Qt3dViewerWidget::getGlWidget() const {
    return glWidget;
}

#endif // QT3DVIEWERWIDGET_H
