#include <QtToolWidget.h>

#include <QFormLayout>
#include <QHBoxLayout>
#include <QString>

#include <QDebug>

QtToolWidget::QtToolWidget(QWidget * parent) : QWidget(parent) {
    QFormLayout *formLayout = new QFormLayout;

    QWidget *minImageWidget = new QWidget;
    QHBoxLayout *minImageLayout = new QHBoxLayout(minImageWidget);
    minImageSlider = new QSlider(Qt::Horizontal);
    minImageLayout->addWidget(minImageSlider);
    minImageLabel = new QLabel();
    minImageLayout->addWidget(minImageLabel);
    formLayout->addRow(tr("Min. greyvalue"), minImageWidget);

    QWidget *maxImageWidget = new QWidget;
    QHBoxLayout *maxImageLayout = new QHBoxLayout(maxImageWidget);
    maxImageSlider = new QSlider(Qt::Horizontal);
    maxImageLayout->addWidget(maxImageSlider);
    maxImageLabel = new QLabel();
    maxImageLayout->addWidget(maxImageLabel);
    formLayout->addRow(tr("Max. greyvalue"), maxImageWidget);

    QWidget *opacityWidget = new QWidget;
    QHBoxLayout *opacityLayout = new QHBoxLayout(opacityWidget);
    opacitySlider = new QSlider(Qt::Horizontal);
    opacityLayout->addWidget(opacitySlider);
    opacityLabel = new QLabel(opacityWidget);
    opacityLayout->addWidget(opacityLabel);
    formLayout->addRow(tr("Opacity"), opacityWidget);

    colormapCombo = new QComboBox();
    formLayout->addRow(tr("Colormap"), colormapCombo);
    fillColorCombo();

    connectSignals();
    initializeValues();

    setLayout(formLayout);
}

void QtToolWidget::setMaximumImageValue(double maxImage) {
    minImageSlider->setMaximum(maxImage * 10);
    maxImageSlider->setMaximum(maxImage * 10);
}

void QtToolWidget::setMinimumImageValue(double minImage) {
    minImageSlider->setMinimum(minImage * 10);
    maxImageSlider->setMinimum(minImage * 10);
}

void QtToolWidget::setDisplayMin(int min) {
    minImageSlider->setValue(min);
}

void QtToolWidget::setDisplayMax(int max) {
    maxImageSlider->setValue(max);
}

void QtToolWidget::setOpacity(int opacity) {
    opacitySlider->setValue(opacity);
}

void QtToolWidget::setColormap(int index) {
    if (index >= 0) {
        colormapCombo->setCurrentIndex(index);
    }
}

void QtToolWidget::connectSignals() {
    connect(minImageSlider, SIGNAL(valueChanged(int)), this, SLOT(minValueChanged(int)));
    connect(maxImageSlider, SIGNAL(valueChanged(int)), this, SLOT(maxValueChanged(int)));
    connect(colormapCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(colormapIndexChanged(int)));
    connect(opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(opacityValueChanged(int)));
}

void QtToolWidget::initializeValues() {
    opacitySlider->setMaximum(255);
    opacitySlider->setValue(255);
    minImageSlider->setValue(0);
    maxImageSlider->setValue(0);
    colormapCombo->setCurrentIndex(3);
}

void QtToolWidget::fillColorCombo() {
    colormapCombo->addItem("Red");
    colormapCombo->addItem("Green");
    colormapCombo->addItem("Blue");
    colormapCombo->addItem("Grey");
}

void QtToolWidget::minValueChanged(int value) {
    minImageLabel->setText(QString::number(value/10.0));
}

void QtToolWidget::maxValueChanged(int value) {
    maxImageLabel->setText(QString::number(value/10.0));
}

void QtToolWidget::colormapIndexChanged(int index) {
    emit colormapChanged(index);
}

void QtToolWidget::opacityValueChanged(int value) {
    opacityLabel->setText(QString::number(value));
    emit opacityChanged(value);
}

