#ifndef QTVIEWERWIDGET_H
#define QTVIEWERWIDGET_H

#include <QSlider>
#include <QToolButton>


class QtGlWidget;

class QtViewerWidget : public QWidget
{
    Q_OBJECT

protected:

    /// toolbutton to expand/collapse viewer
    QToolButton *expandToolButton;

    /// toolbutton to delete viewer
    QToolButton *deleteToolButton;

    /// toolbutton to link to other viewers
    QToolButton *linkToolButton;

    /// flag for viewer linked to other viewers
    bool linked;

public:

    /// class constructor
    QtViewerWidget(QWidget *parent = 0);

    /// check if viewer is linked to the other viewers
    bool isLinked() const;

    /// get OpenGL widget of viewer
    virtual QtGlWidget* getGlWidget() const = 0;

    /// set viewer's current slices
    virtual void setCurrentSlice(int* current) = 0;

    /// set viewer's maximum slices
    virtual void setMaximumSlice(int* maxSlice) = 0;

    /// set viewer enabled
    virtual void setEnabled(bool enabled) = 0;

protected:

    /// create tool buttons
    virtual void createToolButtons();

    /// connect signals to slots
    virtual void connectSignals();

protected slots:

    /// callback function for expandToolButton
    void expandWindow();

    /// callback function for deleteToolButton
    void deleteWindow();

    /// callback function for linkToolButton
    void changeLinked(bool checked);

signals:

    /// signal emitted when viewer is to be full screen
    void windowExpanded();

    /// signal emitted when viewer is to be deleted
    void windowDeleted();
};


inline bool QtViewerWidget::isLinked() const {
    return linked;
}


#endif // QTVIEWERWIDGET_H
