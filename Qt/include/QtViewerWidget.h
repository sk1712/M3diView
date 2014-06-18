#ifndef QTVIEWERWIDGET_H
#define QTVIEWERWIDGET_H

#include <QSlider>
#include <QToolButton>


class QtGlWidget;

class QtViewerWidget : public QWidget
{
    Q_OBJECT

    QWidget *toolWidget;

protected:

    /// Toolbutton to expand/collapse viewer
    QToolButton *expandToolButton;

    /// Toolbutton to delete viewer
    QToolButton *deleteToolButton;

    /// Toolbutton to link to other viewers
    QToolButton *linkToolButton;

    /// Flag for viewer linked to other viewers
    bool linked;

public:

    /// Class constructor
    QtViewerWidget(QWidget *parent = 0);

    /// Check if viewer is linked to the other viewers
    bool isLinked() const;

    /// Get OpenGL widget of viewer
    virtual QtGlWidget* getGlWidget() const = 0;

    /// Set viewer's current slices
    virtual void setCurrentSlice(int* current) = 0;

    /// Set viewer's maximum slices
    virtual void setMaximumSlice(int* maxSlice) = 0;

    /// Set viewer enabled
    virtual void setEnabled(bool enabled) = 0;

protected:

    /// Subclassing repaint event
    void paintEvent(QPaintEvent *);

    /// Create tool buttons
    virtual void createToolButtons();

    /// Connect signals to slots
    virtual void connectSignals();

protected slots:

    /// Callback function for expandToolButton
    void expandWindow();

    /// Callback function for deleteToolButton
    void deleteWindow();

    /// Callback function for linkToolButton
    void changeLinked(bool checked);

signals:

    /// Signal emitted when viewer is to be full screen
    void windowExpanded();

    /// Signal emitted when viewer is to be deleted
    void windowDeleted();
};


inline bool QtViewerWidget::isLinked() const {
    return linked;
}


#endif // QTVIEWERWIDGET_H
