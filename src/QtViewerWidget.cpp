#include <QtViewerWidget.h>

#include <QGridLayout>
#include <QHBoxLayout>

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

    QWidget *toolWidget = new QWidget();

    QHBoxLayout *toolLayout = new QHBoxLayout();
    expandToolButton = new QToolButton();
    expandToolButton->setIcon(QIcon(":/icons/expand_window.png"));
    expandToolButton->setToolTip("Expand window");
    connect(expandToolButton, SIGNAL(clicked()), this, SLOT(expandWindow()));
    toolLayout->addWidget(expandToolButton);

    deleteToolButton = new QToolButton();
    deleteToolButton->setIcon(QIcon(":/icons/delete.png"));
    deleteToolButton->setToolTip("Delete view");
    connect(deleteToolButton, SIGNAL(clicked()), this, SLOT(deleteWindow()));
    toolLayout->addWidget(deleteToolButton);

    toolWidget->setLayout(toolLayout);
    layout->addWidget(toolWidget, 1, 0, Qt::AlignLeft);

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

void QtViewerWidget::deleteWindow() {
    emit windowDeleted();
}
