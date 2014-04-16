#include <QtViewerWidget.h>

#include <QGridLayout>
#include <QHBoxLayout>

QtViewerWidget::QtViewerWidget(QWidget *parent) : QWidget(parent) {
    linked = true;
    QGridLayout *layout = new QGridLayout;

    glWidget = new QtTwoDimensionalGlWidget(this);
    glWidget->setEnabled(false);
    layout->addWidget(glWidget, 0, 0);

    sliceSlider = new QSlider(Qt::Vertical);
    sliceSlider->setMinimum(1);
    sliceSlider->setEnabled(false);
    layout->addWidget(sliceSlider, 0, 1);

    QWidget *toolWidget = new QWidget();
    createToolButtons();
    connectSignals();

    QHBoxLayout *toolLayout = new QHBoxLayout();
    toolLayout->addWidget(expandToolButton);
    toolLayout->addWidget(linkToolButton);
    toolLayout->addWidget(deleteToolButton);

    toolWidget->setLayout(toolLayout);
    layout->addWidget(toolWidget, 1, 0, Qt::AlignLeft);

    sliceLabel = new QLabel();
    layout->addWidget(sliceLabel, 1, 0, Qt::AlignRight);

    setLayout(layout);
}

void QtViewerWidget::updateLabel() {
    sliceLabel->setText(QString::number(currentSlice) + " of " + QString::number(maximumSlice));
}

void QtViewerWidget::initializeParameters() {
    currentSlice = 0;
    maximumSlice = 0;
}

void QtViewerWidget::createToolButtons() {
    expandToolButton = new QToolButton();
    expandToolButton->setIcon(QIcon(":/icons/expand_window.png"));
    expandToolButton->setToolTip("Expand/collapse window");

    linkToolButton = new QToolButton();
    linkToolButton->setIcon(QIcon(":/icons/link.png"));
    linkToolButton->setToolTip("Link to other viewers");
    linkToolButton->setCheckable(true);
    linkToolButton->setChecked(true);

    deleteToolButton = new QToolButton();
    deleteToolButton->setIcon(QIcon(":/icons/delete.png"));
    deleteToolButton->setToolTip("Delete view");
}

void QtViewerWidget::connectSignals() {
    connect(glWidget, SIGNAL(wheelMoved(int)), this, SLOT(changeSlider(int)));
    connect(sliceSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSlice(int)));

    connect(expandToolButton, SIGNAL(clicked()), this, SLOT(expandWindow()));
    connect(linkToolButton, SIGNAL(toggled(bool)), this, SLOT(changeLinked(bool)));
    connect(deleteToolButton, SIGNAL(clicked()), this, SLOT(deleteWindow()));
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

void QtViewerWidget::changeLinked(bool checked) {
    linked = checked;
}
