#include <Qt2dViewerWidget.h>

#include <QGridLayout>
#include <QLabel>

Qt2dViewerWidget::Qt2dViewerWidget(QWidget *parent)
    : QtViewerWidget(parent)
{
    createWidgets();
    createOptionsMenu();
    connectSignals();
    setEnabled(false);
}

void Qt2dViewerWidget::updateLabel() {
    sliceLabel->setText(QString::number(currentSlice) + " of " + QString::number(maximumSlice));
}

void Qt2dViewerWidget::initializeParameters() {
    currentSlice = 0;
    maximumSlice = 0;
}

void Qt2dViewerWidget::createWidgets() {
    QGridLayout *layout = dynamic_cast<QGridLayout*>(this->layout());

    glWidget = new QtTwoDimensionalGlWidget(this);
    layout->addWidget(glWidget, 0, 0);
    layout->setColumnStretch(0, 1);

    sliceSlider = new QSlider(Qt::Vertical);
    sliceSlider->setMinimum(1);
    sliceSlider->setTracking(false);
    layout->addWidget(sliceSlider, 0, 1);

    sliceLabel = new QLabel();
    layout->addWidget(sliceLabel, 1, 0, 1, 2, Qt::AlignRight);
}

void Qt2dViewerWidget::connectSignals() {
    connect(glWidget, SIGNAL(wheelMoved(int)), this, SLOT(changeSlider(int)));
    connect(sliceSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSlice(int)));

    connect(showLabelsAction, SIGNAL(toggled(bool)), this, SLOT(toggleLabelsVisible(bool)));
    connect(showCursorAction, SIGNAL(toggled(bool)), this, SLOT(toggleCursorVisible(bool)));
}

void Qt2dViewerWidget::createOptionsMenu() {
    showLabelsAction = new QAction(tr("Show labels"), this);
    showLabelsAction->setCheckable(true);
    showLabelsAction->setChecked(true);
    settingsMenu->addAction(showLabelsAction);

    showCursorAction = new QAction(tr("Show cursor"), this);
    showCursorAction->setCheckable(true);
    showCursorAction->setChecked(true);
    settingsMenu->addAction(showCursorAction);
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

    emit sliderValueChanged(&currentSlice);
}

void Qt2dViewerWidget::toggleLabelsVisible(bool checked) {
    glWidget->setLabelsVisible(checked);
}

void Qt2dViewerWidget::toggleCursorVisible(bool checked) {
    glWidget->setCursorVisible(checked);
}
