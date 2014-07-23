#include <QtToolWidget.h>

#include <QButtonGroup>
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpacerItem>
#include <QVBoxLayout>

QtToolWidget::QtToolWidget(QWidget * parent) : QWidget(parent) {
    createWidgets();
    connectSignals();
    fixWidgetSizes();
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

int QtToolWidget::getBlendingOption() const {
    return blendingButtonGroup->checkedId();
}

double QtToolWidget::getDisplayMix() const {
    return (double) mixSlider->value() / 100.0;
}

void QtToolWidget::onlyTwoImagesVisible(bool flag) {
    onlyAButton->setEnabled(flag);
    onlyBButton->setEnabled(flag);
    hShutterButton->setEnabled(flag);
    vShutterButton->setEnabled(flag);
    subtractButton->setEnabled(flag);

    if (!flag) {
        blendButton->setChecked(true);
        mixSlider->setEnabled(false);
        emit blendingOptionChanged(blendingButtonGroup->id(blendButton));
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

void QtToolWidget::createWidgets() {
    QVBoxLayout *verticalLayout = new QVBoxLayout;

    QWidget *minImageWidget = new QWidget;
    QHBoxLayout *minImageLayout = new QHBoxLayout(minImageWidget);
    minImageSlider = new QSlider(Qt::Horizontal);
    minImageLabel = new QLabel;
    minImageLayout->addWidget(minImageSlider);
    minImageLayout->addWidget(minImageLabel);

    verticalLayout->addWidget(new QLabel("Min. greyvalue:"));
    verticalLayout->addWidget(minImageWidget);

    QWidget *maxImageWidget = new QWidget;
    QHBoxLayout *maxImageLayout = new QHBoxLayout(maxImageWidget);
    maxImageSlider = new QSlider(Qt::Horizontal);
    maxImageLabel = new QLabel;
    maxImageLayout->addWidget(maxImageSlider);
    maxImageLayout->addWidget(maxImageLabel);

    verticalLayout->addWidget(new QLabel("Max. greyvalue:"));
    verticalLayout->addWidget(maxImageWidget);

    QWidget *opacityWidget = new QWidget;
    QHBoxLayout *opacityLayout = new QHBoxLayout(opacityWidget);
    opacitySlider = new QSlider(Qt::Horizontal);
    opacityLabel = new QLabel;
    opacityLayout->addWidget(opacitySlider);
    opacityLayout->addWidget(opacityLabel);

    verticalLayout->addWidget(new QLabel("Opacity:"));
    verticalLayout->addWidget(opacityWidget);

    verticalLayout->addWidget(new QLabel("Colormap:"));
    colormapCombo = new QComboBox;
    verticalLayout->addWidget(colormapCombo);

    verticalLayout->addWidget(new QLabel("Interpolation:"));
    interpolationCombo = new QComboBox;
    verticalLayout->addWidget(interpolationCombo);

    QFrame *horizontalFrame = new QFrame;
    horizontalFrame->setFrameShape(QFrame::HLine);
    horizontalFrame->setFrameShadow(QFrame::Sunken);
    verticalLayout->addWidget(horizontalFrame);
    verticalLayout->addWidget(createButtonGroup());

    QWidget *blendMixWidget = new QWidget;
    QHBoxLayout *blendMixLayout = new QHBoxLayout(blendMixWidget);
    mixSlider = new QSlider(Qt::Horizontal);
    mixLabel = new QLabel;
    blendMixLayout->addWidget(mixSlider);
    blendMixLayout->addWidget(mixLabel);

    verticalLayout->addWidget(new QLabel("Display mix:"));
    verticalLayout->addWidget(blendMixWidget);

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum,
                                                   QSizePolicy::Expanding );
    verticalLayout->addItem(spacer);

    setLayout(verticalLayout);
}

QWidget* QtToolWidget::createButtonGroup() {
    QGridLayout *buttonLayout = new QGridLayout;
    blendingButtonGroup = new QButtonGroup;

    onlyAButton = new QPushButton("", this);
    onlyAButton->setCheckable(true);
    onlyAButton->setIcon(QIcon(":/icons/imageA.png"));
    onlyAButton->setIconSize(QSize(64,64));
    onlyAButton->setToolTip("Only A");
    buttonLayout->addWidget(onlyAButton, 0, 0);
    blendingButtonGroup->addButton(onlyAButton, 0);

    onlyBButton = new QPushButton("", this);
    onlyBButton->setCheckable(true);
    onlyBButton->setIcon(QIcon(":/icons/imageB.png"));
    onlyBButton->setIconSize(QSize(64,64));
    onlyBButton->setToolTip("Only B");
    buttonLayout->addWidget(onlyBButton, 1, 0);
    blendingButtonGroup->addButton(onlyBButton, 1);

    hShutterButton = new QPushButton("", this);
    hShutterButton->setCheckable(true);
    hShutterButton->setIcon(QIcon(":/icons/imageHShutter.png"));
    hShutterButton->setIconSize(QSize(64,64));
    hShutterButton->setToolTip("Horizontal Shutter");
    buttonLayout->addWidget(hShutterButton, 0, 1);
    blendingButtonGroup->addButton(hShutterButton, 2);

    vShutterButton = new QPushButton("", this);
    vShutterButton->setCheckable(true);
    vShutterButton->setIcon(QIcon(":/icons/imageVShutter.png"));
    vShutterButton->setIconSize(QSize(64,64));
    vShutterButton->setToolTip("Vertical Shutter");
    buttonLayout->addWidget(vShutterButton, 1, 1);
    blendingButtonGroup->addButton(vShutterButton, 3);

    subtractButton = new QPushButton("", this);
    subtractButton->setCheckable(true);
    subtractButton->setIcon(QIcon(":/icons/subtract.png"));
    subtractButton->setIconSize(QSize(64,64));
    subtractButton->setToolTip("Subtract Images");
    buttonLayout->addWidget(subtractButton, 0, 2);
    blendingButtonGroup->addButton(subtractButton, 4);

    blendButton = new QPushButton("", this);
    blendButton->setCheckable(true);
    blendButton->setIcon(QIcon(":/icons/blend.png"));
    blendButton->setIconSize(QSize(64,64));
    blendButton->setToolTip("Blend Images");
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
    connect(mixSlider, SIGNAL(valueChanged(int)), this, SLOT(blendMixValueChanged(int)));
}

void QtToolWidget::fixWidgetSizes() {
    minImageLabel->setFixedWidth(60);
    maxImageLabel->setFixedWidth(60);
    opacityLabel->setFixedWidth(60);
    mixLabel->setFixedWidth(30);
}

void QtToolWidget::initializeValues() {
    opacitySlider->setMaximum(255);
    opacitySlider->setValue(255);
    minImageSlider->setRange(0, 0);
    maxImageSlider->setRange(0, 0);
    mixSlider->setRange(0, 100);
    mixSlider->setValue(50);
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
    if ( (option == blendingButtonGroup->id(hShutterButton)) ||
         (option == blendingButtonGroup->id(vShutterButton)) )
    {
        mixSlider->setEnabled(true);
    }
    else {
        mixSlider->setEnabled(false);
    }

    emit blendingOptionChanged(option);
}

void QtToolWidget::blendMixValueChanged(int value) {
    double blendMix = (double) value / 100.0;
    mixLabel->setText(QString::number(blendMix, 'g', 2));
    emit blendMixChanged(blendMix);
}

