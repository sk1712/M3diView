#ifndef QTTOOLWIDGET_H
#define QTTOOLWIDGET_H

#include <QComboBox>
#include <QLabel>
#include <QSlider>


class QtToolWidget : public QWidget
{
    Q_OBJECT

    /// Labels for min/max display values
    QLabel *minImageLabel, *maxImageLabel;

    /// Sliders for min/max display values
    QSlider *minImageSlider, *maxImageSlider;

    /// Combobox for colormap choice
    QComboBox* colormapCombo;

    /// Label for opacity value
    QLabel* opacityLabel;

    /// Slider for opacity value
    QSlider* opacitySlider;

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

    /// Fill values of colormap combobox
    void fillColorCombo(const QStringList &values);

private:

    /// Create tool widgets
    void addWidgetsToLayout();

    /// Register observers
    void connectSignals();

    /// Unregister observers
    void disconnectSignals();

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

    /// Callback function for opacity value changed
    void opacityValueChanged(int value);

signals:

    /// Signal emitted when min display value changes
    void minChanged(double value);

    /// Signal emitted when max display value changes
    void maxChanged(double value);

    /// Signal emitted when colormap index changes
    void colormapChanged(int index);

    /// Signal emitted when opacity value changes
    void opacityChanged(int value);
};

#endif // QTTOOLWIDGET_H
