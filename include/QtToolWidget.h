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

    void setMaximumDisplayValue(int maxDisplay);

    void setImageMin(int min);

    void setImageMax(int max);

    void setOpacity(int opacity);

private:

    /// register observers
    void connectSignals();

    /// initialize values
    void initializeValues();

    /// create tools
    void createTools();

    /// fill values of colormap combobox
    void fillColorCombo();

private slots:

    void minValueChanged(int value);

    void maxValueChanged(int value);

    void colormapValueChanged(int value);

    void opacityValueChanged(int value);

signals:

    void opacityChanged(int value);
};

#endif // QTTOOLWIDGET_H
