#include <QtToolWidget.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QString>


QtToolWidget::QtToolWidget(QWidget * parent) : QWidget(parent) {
    minImageSlider = new QSlider(Qt::Horizontal);
    minImageLabel = new QLabel;

    maxImageSlider = new QSlider(Qt::Horizontal);
    maxImageLabel = new QLabel;

    opacitySlider = new QSlider(Qt::Horizontal);
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
    minImageSlider->blockSignals(true);
    maxImageSlider->blockSignals(true);
    minImageSlider->setMaximum(maxImage * 10);
    maxImageSlider->setMaximum(maxImage * 10);
    minImageSlider->blockSignals(false);
    maxImageSlider->blockSignals(false);
}

void QtToolWidget::setMinimumImageValue(double minImage) {
    minImageSlider->blockSignals(true);
    maxImageSlider->blockSignals(true);
    minImageSlider->setMinimum(minImage * 10);
    maxImageSlider->setMinimum(minImage * 10);
    minImageSlider->blockSignals(false);
    maxImageSlider->blockSignals(false);
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

void QtToolWidget::onlyTwoImagesVisible(bool flag) {
    onlyAButton->setEnabled(flag);
    onlyBButton->setEnabled(flag);
    hShutterButton->setEnabled(flag);
    vShutterButton->setEnabled(flag);
    subtractButton->setEnabled(flag);

    if (!flag) {
        blendButton->setChecked(true);
    }
}

void QtToolWidget::fillColorCombo(const QStringList &values) {
    colormapCombo->addItems(values);
}

void QtToolWidget::fillInterpolationCombo(const QStringList &values) {
    interpolationCombo->addItems(values);
}

void QtToolWidget::setEnabled(bool enabled) {
    minImageSlider->setEnabled(enabled);
    maxImageSlider->setEnabled(enabled);
    opacitySlider->setEnabled(enabled);
    colormapCombo->setEnabled(enabled);
    interpolationCombo->setEnabled(enabled);
}

void QtToolWidget::addWidgetsToLayout() {
    QVBoxLayout *verticalLayout = new QVBoxLayout;

    QWidget *minImageWidget = new QWidget;
    QHBoxLayout *minImageLayout = new QHBoxLayout(minImageWidget);
    minImageLayout->addWidget(minImageSlider);
    minImageLayout->addWidget(minImageLabel);

    verticalLayout->addWidget(new QLabel("Min. greyvalue:"));
    verticalLayout->addWidget(minImageWidget);

    QWidget *maxImageWidget = new QWidget;
    QHBoxLayout *maxImageLayout = new QHBoxLayout(maxImageWidget);
    maxImageLayout->addWidget(maxImageSlider);
    maxImageLayout->addWidget(maxImageLabel);

    verticalLayout->addWidget(new QLabel("Max. greyvalue:"));
    verticalLayout->addWidget(maxImageWidget);

    QWidget *opacityWidget = new QWidget;
    QHBoxLayout *opacityLayout = new QHBoxLayout(opacityWidget);
    opacityLayout->addWidget(opacitySlider);
    opacityLayout->addWidget(opacityLabel);

    verticalLayout->addWidget(new QLabel("Opacity:"));
    verticalLayout->addWidget(opacityWidget);

    verticalLayout->addWidget(new QLabel("Colormap:"));
    verticalLayout->addWidget(colormapCombo);

    verticalLayout->addWidget(new QLabel("Interpolation:"));
    verticalLayout->addWidget(interpolationCombo);

    verticalLayout->addWidget(createButtonGroup());

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum,
    QSizePolicy::Expanding );
    verticalLayout->addItem( spacer );

    setLayout(verticalLayout);
}

QWidget* QtToolWidget::createButtonGroup() {
    QGridLayout *buttonLayout = new QGridLayout;
    blendingButtonGroup = new QButtonGroup;

    onlyAButton = new QPushButton("Only A", this);
    onlyAButton->setCheckable(true);
    buttonLayout->addWidget(onlyAButton, 0, 0);
    blendingButtonGroup->addButton(onlyAButton, 0);

    onlyBButton = new QPushButton("Only B", this);
    onlyBButton->setCheckable(true);
    buttonLayout->addWidget(onlyBButton, 1, 0);
    blendingButtonGroup->addButton(onlyBButton, 1);

    hShutterButton = new QPushButton("Horizontal shutter", this);
    hShutterButton->setCheckable(true);
    buttonLayout->addWidget(hShutterButton, 0, 1);
    blendingButtonGroup->addButton(hShutterButton, 2);

    vShutterButton = new QPushButton("Vertical shutter", this);
    vShutterButton->setCheckable(true);
    buttonLayout->addWidget(vShutterButton, 1, 1);
    blendingButtonGroup->addButton(vShutterButton, 3);

    subtractButton = new QPushButton("Subtract images", this);
    subtractButton->setCheckable(true);
    buttonLayout->addWidget(subtractButton, 0, 2);
    blendingButtonGroup->addButton(subtractButton, 4);

    blendButton = new QPushButton("Blend images", this);
    blendButton->setCheckable(true);
    buttonLayout->addWidget(blendButton, 1, 2);
    blendingButtonGroup->addButton(blendButton, 5);

    blendingButtonGroup->setExclusive(true);

    QWidget *buttonWidget = new QWidget;
    buttonWidget->setLayout(buttonLayout);

    return buttonWidget;
}

void QtToolWidget::connectSignals() {
    connect(minImageSlider, SIGNAL(valueChanged(int)), this, SLOT(minValueChanged(int)));
    connect(maxImageSlider, SIGNAL(valueChanged(int)), this, SLOT(maxValueChanged(int)));
    connect(colormapCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(colormapIndexChanged(int)));
    connect(interpolationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(interpolationIndexChanged(int)));
    connect(opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(opacityValueChanged(int)));
    connect(blendingButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(blendingButtonClicked(int)));
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

    blockSignals(true);

    if (value > maxDisplay) {
        minValue = maxDisplay;
        minImageSlider->setSliderPosition(maxDisplay);
    }

    minDisplay = minValue;
    minImageLabel->setText(QString::number(minValue/10.0));

    blockSignals(false);

    emit minChanged(minValue/10.0);

}

void QtToolWidget::maxValueChanged(int value) {
    int maxValue = value;

    blockSignals(true);

    if (value < minDisplay) {
        maxValue = minDisplay;
        maxImageSlider->setSliderPosition(minDisplay);
    }

    maxDisplay = maxValue;
    maxImageLabel->setText(QString::number(maxValue/10.0));

    blockSignals(false);

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

void QtToolWidget::blendingButtonClicked(int option) {
    emit blendingOptionChanged(option);
}

