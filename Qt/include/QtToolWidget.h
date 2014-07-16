#ifndef QTTOOLWIDGET_H
#define QTTOOLWIDGET_H

#include <QComboBox>
#include <QLabel>
#include <QSlider>
#include <QButtonGroup>
#include <QPushButton>


class QtToolWidget : public QWidget
{
    Q_OBJECT

    /// Labels for min/max display values
    QLabel *minImageLabel, *maxImageLabel;

    /// Sliders for min/max display values
    QSlider *minImageSlider, *maxImageSlider;

    /// Combobox for colormap choice
    QComboBox* colormapCombo;

    /// Combobox for interpolation options
    QComboBox* interpolationCombo;

    /// Label for opacity value
    QLabel* opacityLabel;

    /// Slider for opacity value
    QSlider* opacitySlider;

    /// Button group for blending options
    QButtonGroup* blendingButtonGroup;

    /// Push buttons for image blending
    QPushButton *onlyAButton, *onlyBButton,
                *hShutterButton, *vShutterButton,
                *subtractButton, *blendButton;

    /// Minimum, maximum display values
    int minDisplay, maxDisplay;

public:

    /// Class constructor
    QtToolWidget(QWidget * parent = 0);

    /// Set maximum image value
    void setMaximumImageValue(double maxImage);

    /// Set minimum image value
    void setMinimumImageValue(double minImage);

    /// Set minimum display value
    void setDisplayMin(double min);

    /// Set maximum display value
    void setDisplayMax(double max);

    /// Set opacity value
    void setOpacity(int opacity);

    /// Set colormap
    void setColormap(int index);

    /// Set interpolation
    void setInterpolation(int index);

    /// Set if only two image are chosen as visible
    void onlyTwoImagesVisible(bool flag);

    /// Fill values of colormap combobox
    void fillColorCombo(const QStringList &values);

    /// Fill values of interpolaton combobox
    void fillInterpolationCombo(const QStringList &values);

    /// Set if widget is enabled
    void setEnabled(bool enabled);

private:

    /// Create tool widgets
    void addWidgetsToLayout();

    /// Create button group
    QWidget *createButtonGroup();

    /// Register widget signals
    void connectSignals();

    /// Set widgets' size
    void fixWidgetSizes();

    /// Initialize values
    void initializeValues();

    /// Create tools
    void createTools();

private slots:

    /// Callback function for min display value changed
    void minValueChanged(int value);

    /// Callback function for max display value changed
    void maxValueChanged(int value);

    /// Callback function for colormap index changed
    void colormapIndexChanged(int index);

    /// Callback function for interpolation index changed
    void interpolationIndexChanged(int index);

    /// Callback function for opacity value changed
    void opacityValueChanged(int value);

    /// Callback function for blending option chabged
    void blendingButtonClicked(int option);

signals:

    /// Signal emitted when min display value changes
    void minChanged(double value);

    /// Signal emitted when max display value changes
    void maxChanged(double value);

    /// Signal emitted when colormap index changes
    void colormapChanged(int index);

    /// Signal emitted when interpolation index changes
    void interpolationChanged(int index);

    /// Signal emitted when opacity value changes
    void opacityChanged(int value);

    /// Signal emitted when blending option changes
    void blendingOptionChanged(int option);
};

#endif // QTTOOLWIDGET_H
