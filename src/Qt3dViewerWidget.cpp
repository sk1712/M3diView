#include <Qt3dViewerWidget.h>

#include <QGridLayout>

Qt3dViewerWidget::Qt3dViewerWidget(QWidget *parent)
    : QtViewerWidget(parent) {
    QGridLayout *layout = dynamic_cast<QGridLayout*>(this->layout());

    glWidget = new QtThreeDimensionalGlWidget(this);
    glWidget->setEnabled(true);
    layout->addWidget(glWidget, 0, 0);
    layout->setRowStretch(0, 1);
}
