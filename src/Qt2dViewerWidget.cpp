#include <Qt2dViewerWidget.h>

#include <QGridLayout>

Qt2dViewerWidget::Qt2dViewerWidget(QWidget *parent)
    : QtViewerWidget(parent)
{
    QGridLayout *layout = dynamic_cast<QGridLayout*>(this->layout());

    glWidget = new QtTwoDimensionalGlWidget(this);
    glWidget->setEnabled(false);
    layout->addWidget(glWidget, 0, 0);

    sliceSlider = new QSlider(Qt::Vertical);
    sliceSlider->setMinimum(1);
    sliceSlider->setEnabled(false);
    layout->addWidget(sliceSlider, 0, 1);

    sliceLabel = new QLabel();
    layout->addWidget(sliceLabel, 1, 0, Qt::AlignRight);

    connectSignals();
}

void Qt2dViewerWidget::updateLabel() {
    sliceLabel->setText(QString::number(currentSlice) + " of " + QString::number(maximumSlice));
}

void Qt2dViewerWidget::initializeParameters() {
    currentSlice = 0;
    maximumSlice = 0;
}

void Qt2dViewerWidget::connectSignals() {
    connect(glWidget, SIGNAL(wheelMoved(int)), this, SLOT(changeSlider(int)));
    connect(sliceSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSlice(int)));
}

void Qt2dViewerWidget::changeSlider(int steps) {
    int newSlice = currentSlice + steps;

    if (newSlice < 1)
        newSlice = 1;

    if (newSlice > maximumSlice)
        newSlice = maximumSlice;

    sliceSlider->setValue(newSlice);
}

void Qt2dViewerWidget::updateSlice(int slice) {
    currentSlice = slice;
    updateLabel();
}
