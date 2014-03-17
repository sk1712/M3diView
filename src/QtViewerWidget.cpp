#include <QtViewerWidget.h>

#include <QGridLayout>

QtViewerWidget::QtViewerWidget(QWidget *parent) : QWidget(parent) {
    QGridLayout *layout = new QGridLayout;

    glWidget = new QtTwoDimensionalGlWidget(this);
    layout->addWidget(glWidget, 0, 0);
    connect(glWidget, SIGNAL(wheelMoved(int)), this, SLOT(changeSlice(int)));

    sliceSlider = new QSlider(Qt::Vertical);
    sliceSlider->setMinimum(1);
    //sliceSlider->setEnabled(false);
    layout->addWidget(sliceSlider, 0, 1);
    connect(sliceSlider, SIGNAL(valueChanged(int)), this, SLOT(changeSlice(int)));

    sliceLabel = new QLabel();
    layout->addWidget(sliceLabel, 1, 0, 1, 2, Qt::AlignRight);

    this->setLayout(layout);
}

void QtViewerWidget::initializeParameters() {
    currentSlice = 0;
    maximumSlice = 0;
}

void QtViewerWidget::changeSlice(int slice) {
    sliceLabel->setText(QString::number(slice));
}
