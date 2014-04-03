#include <QtViewerWidget.h>

#include <QGridLayout>

QtViewerWidget::QtViewerWidget(QWidget *parent) : QWidget(parent) {
    QGridLayout *layout = new QGridLayout;

    glWidget = new QtTwoDimensionalGlWidget(this);
    glWidget->setEnabled(false);
    layout->addWidget(glWidget, 0, 0);
    connect(glWidget, SIGNAL(wheelMoved(int)), this, SLOT(changeSlider(int)));

    sliceSlider = new QSlider(Qt::Vertical);
    sliceSlider->setMinimum(1);
    sliceSlider->setEnabled(false);
    layout->addWidget(sliceSlider, 0, 1);
    connect(sliceSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSlice(int)));

    expandToolButton = new QToolButton();
    expandToolButton->setIcon(QIcon(":/icons/expand_window.png"));
    expandToolButton->setToolTip("Expand window");
    layout->addWidget(expandToolButton, 1, 0, Qt::AlignLeft);
    connect(expandToolButton, SIGNAL(clicked()), this, SLOT(expandWindow()));

    sliceLabel = new QLabel();
    layout->addWidget(sliceLabel, 1, 0, Qt::AlignRight);

    this->setLayout(layout);
}

void QtViewerWidget::updateLabel() {
    sliceLabel->setText(QString::number(currentSlice) + " out of " + QString::number(maximumSlice));
}

void QtViewerWidget::initializeParameters() {
    currentSlice = 0;
    maximumSlice = 0;
}

void QtViewerWidget::changeSlider(int steps) {
    int newSlice = currentSlice + steps;

    if (newSlice < 1)
        newSlice = 1;

    if (newSlice > maximumSlice)
        newSlice = maximumSlice;

    sliceSlider->setValue(newSlice);
}

void QtViewerWidget::updateSlice(int slice) {
    currentSlice = slice;
    updateLabel();
}

void QtViewerWidget::expandWindow() {
    emit windowExpanded();
}
