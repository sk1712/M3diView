#include <QtToolWidget.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QString>


QtToolWidget::QtToolWidget(QWidget * parent) : QWidget(parent) {
    minImageSlider = new QSlider(Qt::Horizontal);
    //minImageSlider->setTracking(false);
    minImageLabel = new QLabel;

    maxImageSlider = new QSlider(Qt::Horizontal);
    //maxImageSlider->setTracking(false);
    maxImageLabel = new QLabel;

    opacitySlider = new QSlider(Qt::Horizontal);
    opacitySlider->setTracking(false);
    opacityLabel = new QLabel;

    colormapCombo = new QComboBox;
    interpolationCombo = new QComboBox;

    // Add the widgets to the layout
    addWidgetsToLayout();
    // Connect signals to slots
    connectSignals();
    // Fix size of specific widgets
    fixWidgetSizes();
    // Initialize the values
    initializeValues();
}

void QtToolWidget::setMaximumImageValue(double maxImage) {
    minImageSlider->setMaximum(maxImage * 10);
    maxImageSlider->setMaximum(maxImage * 10);
}

void QtToolWidget::setMinimumImageValue(double minImage) {
    minImageSlider->setMinimum(minImage * 10);
    maxImageSlider->setMinimum(minImage * 10);
}

void QtToolWidget::setDisplayMin(double min) {
    minImageSlider->setValue(min * 10);
    minDisplay = min * 10;
}

void QtToolWidget::setDisplayMax(double max) {
    maxImageSlider->setValue(max * 10);
    maxDisplay = max * 10;
}

void QtToolWidget::setOpacity(int opacity) {
    opacitySlider->setValue(opacity);
}

void QtToolWidget::setColormap(int index) {
    if (index >= 0) {
        colormapCombo->setCurrentIndex(index);
    }
}

void QtToolWidget::setInterpolation(int index) {
    if (index >= 0) {
        interpolationCombo->setCurrentIndex(index);
    }
}

void QtToolWidget::fillColorCombo(const QStringList &values) {
    colormapCombo->addItems(values);
}

void QtToolWidget::fillInterpolationCombo(const QStringList &values) {
    interpolationCombo->addItems(values);
}

void QtToolWidget::addWidgetsToLayout() {
    QVBoxLayout *verticalLayout = new QVBoxLayout;

    QWidget *minImageWidget = new QWidget;
    QHBoxLayout *minImageLayout = new QHBoxLayout(minImageWidget);
    minImageLayout->addWidget(minImageSlider);
    minImageLayout->addWidget(minImageLabel);

    verticalLayout->addWidget(new QLabel("Min. greyvalue"));
    verticalLayout->addWidget(minImageWidget);

    QWidget *maxImageWidget = new QWidget;
    QHBoxLayout *maxImageLayout = new QHBoxLayout(maxImageWidget);
    maxImageLayout->addWidget(maxImageSlider);
    maxImageLayout->addWidget(maxImageLabel);

    verticalLayout->addWidget(new QLabel("Max. greyvalue"));
    verticalLayout->addWidget(maxImageWidget);

    QWidget *opacityWidget = new QWidget;
    QHBoxLayout *opacityLayout = new QHBoxLayout(opacityWidget);
    opacityLayout->addWidget(opacitySlider);
    opacityLayout->addWidget(opacityLabel);

    verticalLayout->addWidget(new QLabel("Opacity"));
    verticalLayout->addWidget(opacityWidget);

    verticalLayout->addWidget(new QLabel("Colormap"));
    verticalLayout->addWidget(colormapCombo);

    verticalLayout->addWidget(new QLabel("Interpolation"));
    verticalLayout->addWidget(interpolationCombo);

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum,
    QSizePolicy::Expanding );
    verticalLayout->addItem( spacer );

    setLayout(verticalLayout);
}

void QtToolWidget::connectSignals() {
    connect(minImageSlider, SIGNAL(valueChanged(int)), this, SLOT(minValueChanged(int)));
    connect(maxImageSlider, SIGNAL(valueChanged(int)), this, SLOT(maxValueChanged(int)));
    connect(colormapCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(colormapIndexChanged(int)));
    connect(interpolationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(interpolationIndexChanged(int)));
    connect(opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(opacityValueChanged(int)));
}

void QtToolWidget::disconnectSignals() {
    disconnect(minImageSlider, SIGNAL(valueChanged(int)), this, SLOT(minValueChanged(int)));
    disconnect(maxImageSlider, SIGNAL(valueChanged(int)), this, SLOT(maxValueChanged(int)));
    disconnect(colormapCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(colormapIndexChanged(int)));
    disconnect(interpolationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(interpolationIndexChanged(int)));
    disconnect(opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(opacityValueChanged(int)));
}

void QtToolWidget::fixWidgetSizes() {
    minImageLabel->setFixedWidth(60);
    maxImageLabel->setFixedWidth(60);
    opacityLabel->setFixedWidth(60);
}

void QtToolWidget::initializeValues() {
    opacitySlider->setMaximum(255);
    opacitySlider->setValue(255);
    minImageSlider->setRange(0, 0);
    maxImageSlider->setRange(0, 0);
}

void QtToolWidget::minValueChanged(int value) {
    int minValue = value;

    disconnectSignals();

    if (value > maxDisplay) {
        minValue = maxDisplay;
        minImageSlider->setSliderPosition(maxDisplay);
    }

    minDisplay = minValue;
    minImageLabel->setText(QString::number(minValue/10.0));

    connectSignals();

    emit minChanged(minValue/10.0);

}

void QtToolWidget::maxValueChanged(int value) {
    int maxValue = value;

    disconnectSignals();

    if (value < minDisplay) {
        maxValue = minDisplay;
        maxImageSlider->setSliderPosition(minDisplay);
    }

    maxDisplay = maxValue;
    maxImageLabel->setText(QString::number(maxValue/10.0));

    connectSignals();

    emit maxChanged(maxValue/10.0);
}

void QtToolWidget::colormapIndexChanged(int index) {
    emit colormapChanged(index);
}

void QtToolWidget::interpolationIndexChanged(int index) {
    emit interpolationChanged(index);
}

void QtToolWidget::opacityValueChanged(int value) {
    opacityLabel->setText(QString::number(value));
    emit opacityChanged(value);
}

