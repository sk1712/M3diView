#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <irtkQtViewer.h>
#include <irtkImageListModel.h>

#include <Qt2dViewerWidget.h>
#include <Qt3dViewerWidget.h>
#include <QtToolWidget.h>

#include <QMainWindow>
#include <QListView>
#include <QTabWidget>
#include <QSplitter>
#include <QMessageBox>
#include <QWidgetAction>


class QtMainWindow : public QMainWindow
{
    Q_OBJECT

    /// main window widgets
    QSplitter *horizontalSplitter;
    QSplitter *verticalSplitter;
    QListView *imageListView;
    QTabWidget *toolsTabWidget;
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
    QAction *view3DAction;
    QAction *clearViewsAction;

    /// toolbar actions
    QAction *viewSelectedImageAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *moveUpAction;
    QAction *moveDownAction;

    /// visualisation tool widget
    QtToolWidget *visualToolWidget;

    /// list of viewer widgets
    QList<QtViewerWidget*> viewerWidgets;

    /// list of irtk viewers
    QList<irtkQtBaseViewer*> viewers;

    /// model for list view
    irtkImageListModel *imageModel;

    /// flag for only one viewer visible
    bool singleViewerInScreen;

    /// number of currently displayed images
    int numDisplayedImages;

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

    /// create toolbar actions
    void createToolBarActions();

    /// create menu actions
    void createMenuActions();

    /// define the size of splitter elements
    void fixSplitterGeometry();

    /// connect window signals that won't be disconnected
    void connectWindowSignals();

    /// connect visualisation tools signals to slots
    void connectToolSignals();

    /// disconnect signals between viewers and viewerWidgets
    void disconnectViewerSignals();

    /// connect signals between viewers and viewerWidgets
    void connectViewerSignals();

    /// disable viewer widgets when no image is visible
    void disableViewerWidgets();

    /// create new 2D viewer
    Qt2dViewerWidget* createTwoDimensionalView(irtkQtBaseViewer::irtkViewMode viewMode);

    /// create new 3D viewer
    Qt3dViewerWidget* createThreeDimensionalView();

    /// clear viewers and viewerWidgets
    void clearLists();

    /// check if image is already loaded
    bool imageInList(const QString fileName) const;

    /// add widget to mainViewWidget
    void addToViewWidget(QWidget *widget);

    void addToViewWidget(QWidget *widget, int index);

    /// show message box with message and icon
    void createMessageBox(QString message,
                          QMessageBox::Icon icon = QMessageBox::NoIcon);

    /// set up viewers with images to be displayed
    bool setDisplayedImages();

    /// add single image to displayed images
    void displaySingleImage(int index);

    /// delete signle image from displayed images
    void deleteSingleImage(int index);

    /// set up viewer widgets
    void setUpViewerWidgets();

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

    /// callback functon for adding orthogonal view
    void createOrthogonalView();

    /// callback function for adding 3D view
    void create3dView();

    /// callback function for clearing views
    void clearViews();

    /// callback function for updating origin
    void updateOrigin(double x, double y, double z);

    /// callback function for colormap change
    void colormapIndexChanged(int mode);

    /// callback function for opacity slider changing value
    void opacityValueChanged(int value);

    /// callback function for list view double clicked
    void listViewDoubleClicked(QModelIndex index);

    /// callback function for list view cliked
    void listViewClicked(QModelIndex index);

    /// callback function for moving image up in the list
    void moveImageUp();

    /// callback function for moving image down in the list
    void moveImageDown();
};

#endif // QTMAINWINDOW_H
