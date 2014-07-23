#include <Qt3dViewerWidget.h>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QSlider>

Qt3dViewerWidget::Qt3dViewerWidget(QWidget *parent)
    : QtViewerWidget(parent) {
    createWidgets();
    connectSignals();
    setEnabled(false);
}

void Qt3dViewerWidget::setInvertedAxes(bool *inverted) {
    glWidget->setInvertedAxis(inverted);
}

void Qt3dViewerWidget::createWidgets() {
    QGridLayout *layout = dynamic_cast<QGridLayout*>(this->layout());

    glWidget = new QtThreeDimensionalGlWidget(this);
    layout->addWidget(glWidget, 0, 0);
    layout->setColumnStretch(0, 1);

    QWidget* sliderWidget = new QWidget();
    QVBoxLayout* sliderLayout = new QVBoxLayout();
    sliderWidget->setLayout(sliderLayout);

    axialSlider = new QSlider(Qt::Vertical);
    axialSlider->setMinimum(1);
    axialSlider->setObjectName("axial");
    axialSlider->setTracking(false);
    sliderLayout->addWidget(axialSlider);

    sagittalSlider = new QSlider(Qt::Vertical);
    sagittalSlider->setMinimum(1);
    sagittalSlider->setObjectName("sagittal");
    sagittalSlider->setTracking(false);
    sliderLayout->addWidget(sagittalSlider);

    coronalSlider = new QSlider(Qt::Vertical);
    coronalSlider->setMinimum(1);
    coronalSlider->setObjectName("coronal");
    coronalSlider->setTracking(false);
    sliderLayout->addWidget(coronalSlider);

    layout->addWidget(sliderWidget, 0, 1);
}

void Qt3dViewerWidget::connectSignals() {
    connect(axialSlider, SIGNAL(valueChanged(int)), this, SLOT(axialSliceChanged(int)));
    connect(sagittalSlider, SIGNAL(valueChanged(int)), this, SLOT(sagittalSliceChanged(int)));
    connect(coronalSlider, SIGNAL(valueChanged(int)), this, SLOT(coronalSliceChanged(int)));
}

void Qt3dViewerWidget::axialSliceChanged(int value) {
    currentSlice[2] = value;

    emit sliderValueChanged(currentSlice);
}

void Qt3dViewerWidget::sagittalSliceChanged(int value) {
    currentSlice[0] = value;

    emit sliderValueChanged(currentSlice);
}

void Qt3dViewerWidget::coronalSliceChanged(int value) {
    currentSlice[1] = value;

    emit sliderValueChanged(currentSlice);
}
