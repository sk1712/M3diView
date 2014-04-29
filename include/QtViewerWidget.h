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
    int linked;

public:

    /// class constructor
    QtViewerWidget(QWidget *parent = 0);

    /// get viewer linked
    bool isLinked() const;

    virtual QtGlWidget* getGlWidget() const = 0;

protected:

    /// create tool buttons
    virtual void createToolButtons();

    /// connect signals
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
