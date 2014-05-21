#ifndef QTTOOLWIDGET_H
#define QTTOOLWIDGET_H

#include <QComboBox>
#include <QLabel>
#include <QSlider>


class QtToolWidget : public QWidget
{
    Q_OBJECT

    /// labels for min/max display values
    QLabel *minImageLabel, *maxImageLabel;

    /// sliders for min/max display values
    QSlider *minImageSlider, *maxImageSlider;

    /// combobox for colormap choice
    QComboBox* colormapCombo;

    /// label for opacity value
    QLabel* opacityLabel;

    /// slider for opacity value
    QSlider* opacitySlider;

public:

    /// class constructor
    QtToolWidget(QWidget * parent = 0);

    /// set maximum image value
    void setMaximumImageValue(double maxImage);

    /// set minimum image value
    void setMinimumImageValue(double minImage);

    /// set minimum display value
    void setDisplayMin(double min);

    /// set maximum display value
    void setDisplayMax(double max);

    /// set opacity value
    void setOpacity(int opacity);

    /// set colormap
    void setColormap(int index);

private:

    /// create tool widgets
    void addWidgetsToLayout();

    /// register observers
    void connectSignals();

    /// set widgets' size
    void fixWidgetSizes();

    /// initialize values
    void initializeValues();

    /// create tools
    void createTools();

    /// fill values of colormap combobox
    void fillColorCombo();

private slots:

    /// callback function for min display value changed
    void minValueChanged(int value);

    /// callback function for max display value changed
    void maxValueChanged(int value);

    /// callback function for colormap index changed
    void colormapIndexChanged(int index);

    /// callback function for opacity value changed
    void opacityValueChanged(int value);

signals:

    /// signal emitted when min display value changes
    void minChanged(double value);

    /// signal emitted when max display value changes
    void maxChanged(double value);

    /// signal emitted when colormap index changes
    void colormapChanged(int index);

    /// signal emitted when opacity value changes
    void opacityChanged(int value);
};

#endif // QTTOOLWIDGET_H
