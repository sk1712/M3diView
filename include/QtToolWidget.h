#ifndef QTTOOLWIDGET_H
#define QTTOOLWIDGET_H

#include <QComboBox>
#include <QLabel>
#include <QSlider>


class QtToolWidget : public QWidget
{
    Q_OBJECT

    QLabel *minImageLabel, *maxImageLabel;

    QSlider *minImageSlider, *maxImageSlider;

    QComboBox* colormapCombo;

    QLabel* opacityLabel;

    QSlider* opacitySlider;

public:

    /// class constructor
    QtToolWidget(QWidget * parent = 0);

    /// class destructor
    //~QtToolWidget();

    void setMaximumImageValue(double maxImage);

    void setMinimumImageValue(double minImage);

    void setDisplayMin(double min);

    void setDisplayMax(double max);

    void setOpacity(int opacity);

    void setColormap(int index);

private:

    /// register observers
    void connectSignals();

    void fixWidgetSizes();

    /// initialize values
    void initializeValues();

    /// create tools
    void createTools();

    /// fill values of colormap combobox
    void fillColorCombo();

private slots:

    void minValueChanged(int value);

    void maxValueChanged(int value);

    void colormapIndexChanged(int index);

    void opacityValueChanged(int value);

signals:

    void minChanged(double value);

    void maxChanged(double value);

    void colormapChanged(int index);

    void opacityChanged(int value);
};

#endif // QTTOOLWIDGET_H