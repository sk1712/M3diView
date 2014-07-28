#ifndef QT2DVIEWERWIDGET_H
#define QT2DVIEWERWIDGET_H

#include <QtViewerWidget.h>
#include <QtTwoDimensionalGlWidget.h>


class QLabel;

class Qt2dViewerWidget : public QtViewerWidget
{
    Q_OBJECT

    /// OpenGL widget
    QtTwoDimensionalGlWidget *glWidget;

    /// Slider for going through the slices
    QSlider *sliceSlider;

    /// Label showing number of slice
    QLabel *sliceLabel;

    /// Show viewer labels action
    QAction *showLabelsAction;

    /// Show cursor action
    QAction *showCursorAction;

    /// Slice currently shown
    int currentSlice;

    /// Maximum number of slices
    int maximumSlice;

public:

    /// Class constructor
    Qt2dViewerWidget(QWidget *parent = 0);

    /// Class destructor
    QtGlWidget* getGlWidget() const;

    /// Update currentSlice
    void setCurrentSlice(int* current);

    /// Update maximumSlice
    void setMaximumSlice(int* maximum);

    /// Set whether viewer is enabled
    void setEnabled(bool enabled);

    /// Set viewer labels
    void setLabels(const char t, const char b, const char l, const char r);

    /// Set object name (used for styling the widgets
    void setObjectName(QString objectName);

    /// Get whether labels are visible
    bool labelsVisible();

    /// Set whether labels are visible
    void setLabelsVisible(const bool visible);

    /// Get whether cursor is visible
    bool cursorVisible();

    /// Set whether cursor is visible
    void setCursorVisible(const bool visible);

protected:

    /// Update slice label
    void updateLabel();

private:

    /// Initialize current and maximum slice
    void initializeParameters();

    /// Create component widgets
    void createWidgets();

    /// Connect signals
    void connectSignals();

    /// Create options menu
    void createOptionsMenu();

private slots:

    /// Callback function for mouse wheel over OpenGL widget
    void changeSlider(int slice);

    /// Callback function for slice slider change
    void updateSlice(int slice);

    /// Toggle labels visible
    void toggleLabelsVisible(bool checked);

    /// Toggle cursor visible
    void toggleCursorVisible(bool checked);

signals:

    /// Signal emitted when slider value changes
    void sliderValueChanged(int* value);

};


inline QtGlWidget* Qt2dViewerWidget::getGlWidget() const {
    return glWidget;
}

inline void Qt2dViewerWidget::setCurrentSlice(int* current) {
    sliceSlider->setValue(current[0]);
}

inline void Qt2dViewerWidget::setMaximumSlice(int* maximum) {
    maximumSlice = maximum[0];
    sliceSlider->setMaximum(maximumSlice);
}

inline void Qt2dViewerWidget::setEnabled(bool enabled) {
    glWidget->setEnabled(enabled);
    sliceSlider->setEnabled(enabled);
}

inline void Qt2dViewerWidget::setLabels(const char t, const char b, const char l, const char r) {
    glWidget->setLabels(t, b, l, r);
}

inline void Qt2dViewerWidget::setObjectName(QString objectName) {
    sliceSlider->setObjectName(objectName);
}

#endif // QT2DVIEWERWIDGET_H
