#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QtViewerWidget.h>

#include <irtkQtViewer.h>

#include <QMainWindow>
#include <QListWidget>
#include <QSplitter>
#include <QMessageBox>


class QtMainWindow : public QMainWindow
{
    Q_OBJECT

    /// main window widgets
    QSplitter *splitter;

    QListWidget *listWidget;

    QWidget *mainViewWidget;

    /// window menus
    QMenu *fileMenu;

    QMenu *viewMenu;

    /// window toolbar
    QToolBar *toolbar;

    /// file actions
    QAction *openTargetAction;

    /// view actions
    QAction *viewAxialAction;

    QAction *viewCoronalAction;

    QAction *viewSagittalAction;

    QAction *viewOrthogonalAction;

    QAction *clearViewsAction;

    /// toolbar actions
    QAction *viewSelectedImageAction;

    QAction *zoomInAction;

    QAction *zoomOutAction;

    /// vector of viewer widgets
    QVector<QtViewerWidget*> viewerWidgets;

    /// vector of irtk 2D viewers
    QVector<irtkQtTwoDimensionalViewer*> viewers;

    /// flag for only one viewer visible
    bool singleViewerInScreen;

public:

    /// class constructor
    QtMainWindow();

    /// class destructor
    ~QtMainWindow();

private:

    /// create window toolbar
    void createToolBar();

    /// create window menu
    void createMenu();

    /// create window actions
    void createActions();

    /// disconnect signals between viewers and viewerWidgets
    void disconnectSignals();

    /// connect signals between viewers and viewerWidgets
    void connectSignals();

    /// show image(s) on screen
    void showTargetImage(int i);

    /// create new 2D viewer
    QtViewerWidget* createTwoDimensionalView(irtkViewMode viewMode);

    /// clear viewers and viewerWidgets
    void clearVectors();

    /// check if image is already loaded
    bool imageInList(const QString fileName);

    /// add widget to mainViewWidget
    void addToViewWidget(QWidget *widget);

    /// show message box
    void createMessageBox(QString message,
                          QMessageBox::Icon icon = QMessageBox::NoIcon);

private slots:

    /// callback function for openTargetAction
    void openImage();

    /// callback function for viewSelectedImageAction
    void viewImage();

    /// callback function for zoomInAction
    void zoomIn();

    /// callback function for zoomOutAction
    void zoomOut();

    /// callback function for expanding viewer widget
    void showOnlyThisWidget();

    /// callback function for deleting viewer widget
    void deleteThisWidget();

    /// callback function for adding axial view
    void createAxialView();

    /// callback function for adding coronal view
    void createCoronalView();

    /// callback functon for adding sagittal view
    void createSagittalView();

    /// callback functon for adding
    void createOrthogonalView();

    /// callback function for clearing views
    void clearViews();

    /// callback function for updated origin
    void updateOrigin(double x, double y, double z);
};

#endif // QTMAINWINDOW_H
