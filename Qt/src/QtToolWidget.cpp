#include <QtToolWidget.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QString>

#include <QDebug>

QtToolWidget::QtToolWidget(QWidget * parent) : QWidget(parent) {
    minImageSlider = new QSlider(Qt::Horizontal);
    minImageLabel = new QLabel();

    maxImageSlider = new QSlider(Qt::Horizontal);
    maxImageLabel = new QLabel();

    opacitySlider = new QSlider(Qt::Horizontal);
    opacityLabel = new QLabel();

    colormapCombo = new QComboBox();

    // add the widgets to the layout
    addWidgetsToLayout();
    // connect signals to slots
    connectSignals();
    // fix size of specific widgets
    fixWidgetSizes();
    // initialize the values
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
}

void QtToolWidget::setDisplayMax(double max) {
    maxImageSlider->setValue(max * 10);
}

void QtToolWidget::setOpacity(int opacity) {
    opacitySlider->setValue(opacity);
}

void QtToolWidget::setColormap(int index) {
    if (index >= 0) {
        colormapCombo->setCurrentIndex(index);
    }
}

void QtToolWidget::fillColorCombo(const QStringList &values) {
    colormapCombo->addItems(values);
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

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum,
    QSizePolicy::Expanding );
    verticalLayout->addItem( spacer );

    setLayout(verticalLayout);
}

void QtToolWidget::connectSignals() {
    connect(minImageSlider, SIGNAL(valueChanged(int)), this, SLOT(minValueChanged(int)));
    connect(maxImageSlider, SIGNAL(valueChanged(int)), this, SLOT(maxValueChanged(int)));
    connect(colormapCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(colormapIndexChanged(int)));
    connect(opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(opacityValueChanged(int)));
}

void QtToolWidget::fixWidgetSizes() {
    minImageSlider->setFixedWidth(120);
    minImageLabel->setFixedWidth(60);
    maxImageSlider->setFixedWidth(120);
    maxImageLabel->setFixedWidth(60);
    opacitySlider->setFixedWidth(120);
    opacityLabel->setFixedWidth(60);
}

void QtToolWidget::initializeValues() {
    opacitySlider->setMaximum(255);
    opacitySlider->setValue(255);
    minImageSlider->setRange(0, 0);
    maxImageSlider->setRange(0, 0);
}

void QtToolWidget::minValueChanged(int value) {
    minImageLabel->setText(QString::number(value/10.0));
    maxImageSlider->setMinimum(value);
    emit minChanged(value/10.0);
}

void QtToolWidget::maxValueChanged(int value) {
    maxImageLabel->setText(QString::number(value/10.0));
    minImageSlider->setMaximum(value);
    emit maxChanged(value/10.0);
}

void QtToolWidget::colormapIndexChanged(int index) {
    emit colormapChanged(index);
}

void QtToolWidget::opacityValueChanged(int value) {
    opacityLabel->setText(QString::number(value));
    emit opacityChanged(value);
}

