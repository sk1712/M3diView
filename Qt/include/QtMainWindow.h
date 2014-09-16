#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <irtkQtTreeModel.h>
#include <irtkQtConfiguration.h>

#include <Qt2dViewerWidget.h>
#include <Qt3dViewerWidget.h>
#include <QtToolWidget.h>
#include <QtInfoWidget.h>

#include <QMainWindow>
#include <QMessageBox>

class QTreeView;
class QTabWidget;


class QtMainWindow : public QMainWindow
{
    Q_OBJECT

    /// Main window widgets
    QTreeView *imageTreeView;
    QTabWidget *toolsTabWidget;
    QWidget *mainViewWidget;

    /// Window menus
    QMenu *fileMenu;
    QMenu *viewMenu;

    /// Window toolbar
    QToolBar *toolbar;

    /// File actions
    QAction *openTargetAction;
    QAction *clearImagesAction;
    QAction *saveScreenshotAction;
    QAction *loadConfigurationAction;
    QAction *saveConfigurationAction;

    /// View actions
    QAction *viewAxialAction;
    QAction *viewCoronalAction;
    QAction *viewSagittalAction;
    QAction *viewOrthogonalAction;
    QAction *view3DAction;
    QAction *clearViewsAction;

    /// Toolbar actions
    QAction *openImageToolbarAction;
    QAction *screenshotToolbarAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *moveUpAction;
    QAction *moveDownAction;

    /// Image actions
    QAction *toggleVisibleAction;
    QAction *loadSegmentationAction;
    QAction *changeLabelAction;
    QAction *deleteImageAction;

    /// Visualisation tool widget
    QtToolWidget *visualToolWidget;

    /// Image info widget
    QtInfoWidget *infoWidget;

    /// List of viewer widgets
    QList<QtViewerWidget*> viewerWidgets;

    /// List of irtk viewers
    QList<irtkQtBaseViewer*> viewers;

    /// Model for list view
    irtkQtTreeModel *imageModel;

    /// Flag for only one viewer visible
    bool singleViewerInScreen;

    /// Number of currently displayed images
    int numDisplayedImages;

    /// Image index currently clicked
    int currentImageIndex;

public:

    /// Class constructor
    QtMainWindow();

    /// Class destructor
    ~QtMainWindow();

    /// Load list of images defined in QStringList
    void loadImages(const QStringList & fileList);

private:

    /// Load list of segmentation files
    void loadSegmentations(const QStringList & segmentationList);

    /// Create dock windows
    void createDockWindows();

    /// Create window toolbar
    void createToolBar();

    /// Create window menu
    void createMenu();

    /// Create toolbar actions
    void createToolBarActions();

    /// Create menu actions
    void createMenuActions();

    /// Create image menu actions
    void createImageMenuActions();

    /// Connect window signals that won't be disconnected
    void connectWindowSignals();

    /// Connect visualisation tools signals to slots
    void connectToolSignals();

    /// Disconnect signals between viewers and viewerWidgets
    void disconnectViewerSignals();

    /// Connect signals between viewers and viewerWidgets
    void connectViewerSignals();

    /// Disable viewer widgets when no image is visible
    void disableViewerWidgets();

    /// Create new 2D viewer
    Qt2dViewerWidget* createTwoDimensionalView(irtkQtBaseViewer::irtkViewMode viewMode);

    /// Create new 3D viewer
    Qt3dViewerWidget* createThreeDimensionalView();

    /// Clear viewers and viewerWidgets
    void clearLists();

    /// Check if image is already loaded
    bool imageInList(const QString fileName) const;

    /// Check if segmentation is already loaded
    bool segmentationInList(const QString fileName) const;

    /// Toggle segmentation visible
    void toggleSegmentationVisible();

    /// Add widget to mainViewWidget
    void addToViewWidget(QWidget *widget);

    void addToViewWidget(QWidget *widget, int index);

    /// Show message box with message and icon
    void createMessageBox(QString message,
                          QMessageBox::Icon icon = QMessageBox::NoIcon);

    /// Set up viewers with images to be displayed
    bool setDisplayedImages();

    /// Add single image to displayed images
    void displaySingleImage(QModelIndex index);

    /// Add single segmentation to displayed segmentations
    void displaySingleSegmentation(QModelIndex index);

    /// Delete single image from displayed images
    void deleteSingleImage(int index);

    /// Delete images with indices provided in the rowList
    void deleteImages(QModelIndexList rowList);

    /// Set up viewer widgets
    void setUpViewerWidgets();

    /// Update all drawables
    void updateDrawables();

    /// Create configuration viewer list
    void createConfigurationViewerList();

    /// Load configuration image list
    void loadConfigurationImageList();

    /// Load configuration viewer list
    void loadConfigurationViewerList();

private slots:

    /// Callback function for openTargetAction
    void openImage();

    /// Callback function for clearImagesAction
    void clearImages();

    /// Callback function for saveScreenshotAction
    void saveScreenshot();

    /// Callback function for viewSelectedImageAction
    void viewImages();

    /// Callback function for deleteImageAction
    void deleteSelectedImages();

    /// Callback function for toggleVisibleAction
    void toggleImageVisible();

    /// Callback function for loadSegmentationAction
    void openSegmentation();

    /// Callback function for changing the segmentation label color
    void changeLabelColor();

    /// Callback function for zoomInAction
    void zoomIn();

    /// Callback function for zoomOutAction
    void zoomOut();

    /// Callback function for expanding viewer widget
    void showOnlyThisWidget();

    /// Callback function for deleting viewer widget
    void deleteThisWidget();

    /// Callback function for adding axial view
    void createAxialView();

    /// Callback function for adding coronal view
    void createCoronalView();

    /// Callback functon for adding sagittal view
    void createSagittalView();

    /// Callback functon for adding orthogonal view
    void createOrthogonalView();

    /// Callback function for adding 3D view
    void create3dView();

    /// Callback function for clearing views
    void clearViews();

    /// Callback function for updating origin
    void updateOrigin(double x, double y, double z);

    /// Callback function for min display image value changed
    void minDisplayValueChanged(double value);

    /// Callback function for max display image value changed
    void maxDisplayValueChanged(double value);

    /// Callback function for colormap change
    void colormapIndexChanged(int mode);

    /// Callback function for interpolation change
    void interpolationIndexChanged(int mode);

    /// Callback function for opacity slider changing value
    void opacityValueChanged(int value);

    /// Callback function for tree view double clicked
    void treeViewDoubleClicked(QModelIndex index);

    /// Callback function for tree view cliked
    void treeViewClicked(QModelIndex index);

    /// Callback function for tree view right click
    void treeViewShowContextMenu(const QPoint& pos);

    /// Callback function for moving image up in the list
    void moveImageUp();

    /// Callback function for moving image down in the list
    void moveImageDown();

    /// Callback function for blending mode changed
    void blendModeChanged(int mode);

    /// Callback function for display mix value changed
    void displayMixValueChanged(double value);

    /// Callback function for loading configuration file
    void loadConfigurationFile();

    /// Callback function for saving configuration file
    void saveConfigurationFile();
};

#endif // QTMAINWINDOW_H
