#include <QtMainWindow.h>

#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QGridLayout>
#include <QMenuBar>
#include <QColorDialog>
#include <QScrollArea>
#include <QTabWidget>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

/// free function used to calculate the output images
/// in parallel for the different viewers
void CalculateOutputImage(irtkQtBaseViewer* viewer) {
    viewer->CalculateOutputImages();
    viewer->CalculateSegmentationOutput();
}

QtMainWindow::QtMainWindow() {
    mainViewWidget = new QWidget();
    QGridLayout *layout = new QGridLayout;
    mainViewWidget->setLayout(layout);
    setCentralWidget(mainViewWidget);

    createMenuActions();
    createToolBarActions();
    createImageMenuActions();
    createMenu();
    createToolBar();
    createDockWindows();

    connectWindowSignals();
    connectToolSignals();

    singleViewerInScreen = false;
    numDisplayedImages = 0;
    currentImageIndex = -1;
    imageModel = new irtkQtTreeModel;
    imageTreeView->setModel(imageModel);

    // By default add orthogonal and 3D view
    createOrthogonalView();
    create3dView();
    visualToolWidget->onlyTwoImagesVisible(false);
}

QtMainWindow::~QtMainWindow() {
    clearLists();
    irtkQtConfiguration::Destroy();
    delete imageModel;
}

void QtMainWindow::loadImages(const QStringList &fileList) {
    int imageCount = imageModel->rowCount(QModelIndex());
    irtkQtImageObject *image = NULL;

    // Create an irtkImageObject for each new image
    QStringList::const_iterator it;
    for (it = fileList.constBegin(); it != fileList.constEnd(); ++it) {
        if ( !imageInList(*it) ) {
            qDebug("Creating image for file %s", qPrintable(*it));

            image = new irtkQtImageObject();
            image->setImagePath(*it);

            // Update the image model
            if ( !imageModel->insertRows(imageCount, 1) ) {
                delete image;
                continue;
            }

            QModelIndex child = imageModel->index(imageCount++, 0, QModelIndex());
            if ( !imageModel->setData(child, image, Qt::EditRole) ) {
                delete image;
                qCritical("Could not add image %s to list",
                          qPrintable(image->GetPath()));
            }
        }
    }
}

void QtMainWindow::loadSegmentations(const QStringList &segmentationList) {
    QModelIndex currentIndex = imageTreeView->currentIndex();
    int imageCount = imageModel->rowCount(currentIndex);
    irtkQtImageObject *segmentation = NULL;

    QStringList::const_iterator it;
    for (it = segmentationList.constBegin(); it != segmentationList.constEnd(); ++it) {
        if ( !segmentationInList(*it) ) {
            qDebug("Creating segmentation for file %s", qPrintable(*it));

            segmentation = new irtkQtImageObject();
            segmentation->setImagePath(*it);

            if ( !imageModel->insertRows(imageCount, 1, currentIndex)) {
                delete segmentation;
                continue;
            }

            QModelIndex child = imageModel->index(imageCount++, 0, currentIndex);
            if ( !imageModel->setData(child, segmentation, Qt::EditRole) ) {
                delete segmentation;
                qCritical("Could not add segmentation %s to list",
                          qPrintable(segmentation->GetPath()));
            }
        }
    }
}

void QtMainWindow::createDockWindows() {
    // Create image tree dock widget
    QDockWidget *dock = new QDockWidget(tr("Image List"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    imageTreeView = new QTreeView(dock);
    imageTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    imageTreeView->setExpandsOnDoubleClick(false);
    // Show a menu on tree view right click
    imageTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    dock->setWidget(imageTreeView);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    // Create tab dock widget with tools
    dock = new QDockWidget(tr("Tools"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toolsTabWidget = new QTabWidget(dock);
    dock->setWidget(toolsTabWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    // Set up info tool
    infoWidget = new QtInfoWidget;
    toolsTabWidget->addTab(infoWidget, tr("Info"));

    // Set up visualisation tool
    visualToolWidget = new QtToolWidget;
    visualToolWidget->setEnabled(false);

    irtkQtLookupTable::SetColorModeList();
    visualToolWidget->fillColorCombo(irtkQtLookupTable::GetColorModeList());
    irtkQtImageObject::SetInterpolationModeList();
    visualToolWidget->fillInterpolationCombo(irtkQtImageObject::GetInterpolationModeList());

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(visualToolWidget);
    scrollArea->setWidgetResizable(true);
    toolsTabWidget->addTab(scrollArea, tr("Visualisation"));
}

void QtMainWindow::createToolBar() {
    toolbar = addToolBar(tr("View"));

    toolbar->addAction(openImageToolbarAction);
    toolbar->addAction(screenshotToolbarAction);
    toolbar->addSeparator();

    toolbar->addAction(zoomInAction);
    toolbar->addAction(zoomOutAction);
    toolbar->addSeparator();

    toolbar->addAction(moveUpAction);
    toolbar->addAction(moveDownAction);
}

void QtMainWindow::createMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openTargetAction);
    fileMenu->addAction(clearImagesAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveScreenshotAction);
    fileMenu->addSeparator();
    fileMenu->addAction(loadConfigurationAction);
    fileMenu->addAction(saveConfigurationAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    QMenu *addMenu = viewMenu->addMenu(tr("Add View"));
    addMenu->setIcon(QIcon(":/icons/add_view.png"));
    addMenu->addAction(viewOrthogonalAction);
    addMenu->addAction(viewAxialAction);
    addMenu->addAction(viewCoronalAction);
    addMenu->addAction(viewSagittalAction);

    addMenu->addSeparator();
    addMenu->addAction(view3DAction);

    viewMenu->addSeparator();
    viewMenu->addAction(clearViewsAction);
    viewMenu->addSeparator();
}

void QtMainWindow::createToolBarActions() {
    openImageToolbarAction = new QAction(tr("Add files"), this);
    openImageToolbarAction->setIcon(QIcon(":/icons/add_file.png"));

    screenshotToolbarAction = new QAction(tr("Save screenshot"), this);
    screenshotToolbarAction->setIcon(QIcon(":/icons/screenshot.png"));

    zoomInAction = new QAction(tr("Zoom in"), this);
    zoomInAction->setIcon(QIcon(":/icons/zoom_in.png"));
    zoomInAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal));

    zoomOutAction = new QAction(tr("Zoom out"), this);
    zoomOutAction->setIcon(QIcon(":/icons/zoom_out.png"));
    zoomOutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));

    moveUpAction = new QAction(tr("Move image up"), this);
    moveUpAction->setIcon(QIcon(":/icons/arrow_up.png"));

    moveDownAction = new QAction(tr("Move image down"), this);
    moveDownAction->setIcon(QIcon(":/icons/arrow_down.png"));
}

void QtMainWindow::createMenuActions() {
    openTargetAction = new QAction(tr("Add Image File(s)..."), this);
    openTargetAction->setStatusTip(tr("Load new image file(s)"));
    openTargetAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    openTargetAction->setIcon(QIcon(":/icons/add_file.png"));
    openTargetAction->setShortcutContext(Qt::ApplicationShortcut);

    clearImagesAction = new QAction(tr("Clear Images"), this);
    clearImagesAction->setStatusTip(tr("Clear all images"));

    saveScreenshotAction = new QAction(tr("Save Screenshot"), this);
    saveScreenshotAction->setStatusTip(tr("Save screenshot"));
    saveScreenshotAction->setIcon(QIcon(":/icons/screenshot.png"));

    loadConfigurationAction = new QAction(tr("Load Configuration"), this);
    loadConfigurationAction->setIcon(QIcon(":/icons/open_config.png"));

    saveConfigurationAction = new QAction(tr("Save Configuration"), this);
    saveConfigurationAction->setIcon(QIcon(":/icons/save_config.png"));

    viewAxialAction = new QAction(tr("Axial"), this);
    viewAxialAction->setStatusTip(tr("Add axial view"));

    viewCoronalAction = new QAction(tr("Coronal"), this);
    viewCoronalAction->setStatusTip(tr("Add coronal view"));

    viewSagittalAction = new QAction(tr("Sagittal"), this);
    viewSagittalAction->setStatusTip(tr("Add sagittal view"));

    viewOrthogonalAction = new QAction(tr("Orthogonal"), this);
    viewOrthogonalAction->setStatusTip(tr("Add orthogonal view"));

    view3DAction = new QAction(tr("3D"), this);
    view3DAction->setStatusTip(tr("Add 3D view"));

    clearViewsAction = new QAction(tr("Clear All"), this);
    clearViewsAction->setStatusTip(tr("Delete all views"));
    clearViewsAction->setIcon(QIcon(":/icons/delete_view.png"));
}

void QtMainWindow::createImageMenuActions() {
    toggleVisibleAction = new QAction(tr("Visible"), this);
    toggleVisibleAction->setCheckable(true);

    loadSegmentationAction = new QAction(tr("Load segmentation"), this);
    loadSegmentationAction->setIcon(QIcon(":/icons/add_file.png"));

    changeLabelAction = new QAction(tr("Change label color"), this);
    changeLabelAction->setIcon(QIcon(":/icons/paint_palette.png"));

    deleteImageAction = new QAction(tr("Remove from list"), this);
    deleteImageAction->setIcon(QIcon(":/icons/erase.png"));
}

void QtMainWindow::connectWindowSignals() {
    // List view signals
    connect(imageTreeView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(treeViewDoubleClicked(QModelIndex)));
    connect(imageTreeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(treeViewClicked(QModelIndex)));
    connect(imageTreeView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(treeViewShowContextMenu(QPoint)));

    // Image menu signals
    connect(toggleVisibleAction, SIGNAL(triggered()), this, SLOT(toggleVisible()));
    connect(loadSegmentationAction, SIGNAL(triggered()), this, SLOT(openSegmentation()));
    connect(changeLabelAction, SIGNAL(triggered()), this, SLOT(changeLabelColor()));
    connect(deleteImageAction, SIGNAL(triggered()), this, SLOT(deleteSelectedImages()));

    // Toolbar signals
    connect(openImageToolbarAction, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(screenshotToolbarAction, SIGNAL(triggered()), this, SLOT(saveScreenshot()));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(moveUpAction, SIGNAL(triggered()), this, SLOT(moveImageUp()));
    connect(moveDownAction, SIGNAL(triggered()), this, SLOT(moveImageDown()));

    // Menu signals
    connect(openTargetAction, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(clearImagesAction, SIGNAL(triggered()), this, SLOT(clearImages()));
    connect(saveScreenshotAction, SIGNAL(triggered()), this, SLOT(saveScreenshot()));
    connect(loadConfigurationAction, SIGNAL(triggered()), this, SLOT(loadConfigurationFile()));
    connect(saveConfigurationAction, SIGNAL(triggered()), this, SLOT(saveConfigutationFile()));
    connect(viewAxialAction, SIGNAL(triggered()), this, SLOT(createAxialView()));
    connect(viewCoronalAction, SIGNAL(triggered()), this, SLOT(createCoronalView()));
    connect(viewSagittalAction, SIGNAL(triggered()), this, SLOT(createSagittalView()));
    connect(viewOrthogonalAction, SIGNAL(triggered()), this, SLOT(createOrthogonalView()));
    connect(view3DAction, SIGNAL(triggered()), this, SLOT(create3dView()));
    connect(clearViewsAction, SIGNAL(triggered()), this, SLOT(clearViews()));
}

void QtMainWindow::connectToolSignals() {
    connect(visualToolWidget, SIGNAL(colormapChanged(int)), this, SLOT(colormapIndexChanged(int)));
    connect(visualToolWidget, SIGNAL(interpolationChanged(int)), this, SLOT(interpolationIndexChanged(int)));
    connect(visualToolWidget, SIGNAL(opacityChanged(int)), this, SLOT(opacityValueChanged(int)));
    connect(visualToolWidget, SIGNAL(minChanged(double)), this, SLOT(minDisplayValueChanged(double)));
    connect(visualToolWidget, SIGNAL(maxChanged(double)), this, SLOT(maxDisplayValueChanged(double)));
    connect(visualToolWidget, SIGNAL(blendingOptionChanged(int)), this, SLOT(blendModeChanged(int)));
    connect(visualToolWidget, SIGNAL(blendMixChanged(double)), this, SLOT(displayMixValueChanged(double)));
}

void QtMainWindow::disconnectViewerSignals() {
    QtViewerWidget *viewerWidget;
    irtkQtBaseViewer *viewer;

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets[i];
        viewer = viewers[i];

        disconnect(viewerWidget, SIGNAL(sliderValueChanged(int*)),
                   viewer, SLOT(ChangeSlice(int*)));
        disconnect(viewer, SIGNAL(OriginChanged(double, double, double)),
                   this, SLOT(updateOrigin(double, double, double)));

        // The following do smth only for the 2D viewer
        disconnect(viewerWidget->getGlWidget(), SIGNAL(resized(int, int)),
                   viewer, SLOT(ResizeImage(int, int)));
        disconnect(viewer, SIGNAL(ImageResized(QVector<QRgb**>)),
                   viewerWidget->getGlWidget(), SLOT(updateDrawable(QVector<QRgb**>)));

        disconnect(viewerWidget->getGlWidget(), SIGNAL(leftButtonPressed(int, int)),
                   viewer, SLOT(ChangeOrigin(int, int)));
    }
}

void QtMainWindow::connectViewerSignals() {
    QtViewerWidget *viewerWidget;
    irtkQtBaseViewer *viewer;

    for (int i = 0; i < viewers.size(); i++) {
        // Update drawable when origin is changed
        viewerWidget = viewerWidgets[i];
        viewer = viewers[i];

        connect(viewerWidget, SIGNAL(sliderValueChanged(int*)),
                viewer, SLOT(ChangeSlice(int*)));
        connect(viewer, SIGNAL(OriginChanged(double, double, double)),
                this, SLOT(updateOrigin(double, double, double)));

        // The following do smth only for the 2D viewer
        connect(viewerWidget->getGlWidget(), SIGNAL(resized(int, int)),
                viewer, SLOT(ResizeImage(int, int)));
        connect(viewer, SIGNAL(ImageResized(QVector<QRgb**>)),
                viewerWidget->getGlWidget(), SLOT(updateDrawable(QVector<QRgb**>)));

        connect(viewerWidget->getGlWidget(), SIGNAL(leftButtonPressed(int, int)),
                viewer, SLOT(ChangeOrigin(int, int)));
    }
}

void QtMainWindow::disableViewerWidgets() {
    QList<QtViewerWidget*>::iterator it;
    for (it = viewerWidgets.begin(); it != viewerWidgets.end(); ++it) {
        (*it)->setEnabled(false);
    }
}

Qt2dViewerWidget* QtMainWindow::createTwoDimensionalView(irtkQtBaseViewer::irtkViewMode viewMode) {
    irtkQtTwoDimensionalViewer* viewer;
    Qt2dViewerWidget *qtViewer;

    viewer = new irtkQtTwoDimensionalViewer(viewMode);
    viewers.push_back(viewer);
    qtViewer = new Qt2dViewerWidget();
    viewerWidgets.push_back(qtViewer);

    // Define the labels that appear on the viewer widget for orientation information
    char top, bottom, left, right;
    viewer->GetLabels(top, bottom, left, right);
    qtViewer->setLabels(top, bottom, left, right);
    qtViewer->setObjectName(QString::fromStdString(viewer->GetObjectName()));

    // Register signals to expand or delete the viewer
    connect(qtViewer, SIGNAL(windowExpanded()), this, SLOT(showOnlyThisWidget()));
    connect(qtViewer, SIGNAL(windowDeleted()), this, SLOT(deleteThisWidget()));

    viewer->SetMixValue(visualToolWidget->getDisplayMix());
    viewer->SetBlendMode(visualToolWidget->getBlendingOption());
    viewImages();

    return qtViewer;
}

Qt3dViewerWidget* QtMainWindow::createThreeDimensionalView() {
    irtkQtThreeDimensionalViewer* viewer;
    Qt3dViewerWidget *qtViewer;

    viewer = new irtkQtThreeDimensionalViewer();
    viewers.push_back(viewer);
    qtViewer = new Qt3dViewerWidget();
    viewerWidgets.push_back(qtViewer);

    // Register signals to expand or delete the viewer
    connect(qtViewer, SIGNAL(windowExpanded()), this, SLOT(showOnlyThisWidget()));
    connect(qtViewer, SIGNAL(windowDeleted()), this, SLOT(deleteThisWidget()));

    viewer->SetMixValue(visualToolWidget->getDisplayMix());
    viewer->SetBlendMode(visualToolWidget->getBlendingOption());
    viewImages();

    return qtViewer;
}

void QtMainWindow::clearLists() {
    // Use qDeleteAll macro to delete all elements of the lists
    qDeleteAll(viewers);
    viewers.clear();

    qDeleteAll(viewerWidgets);
    viewerWidgets.clear();
}

bool QtMainWindow::imageInList(const QString fileName) const {
    int imageCount = imageModel->rowCount(QModelIndex());
    for (int i = 0; i < imageCount; ++i) {
        QModelIndex currentIdx = imageModel->index(i, 0, QModelIndex());
        if (imageModel->data(currentIdx, Qt::ToolTipRole) == fileName) {
            // Image has already been loaded
            qDebug() << "Image " << fileName << " already in the list";
            return true;
        }
    }

    return false;
}

bool QtMainWindow::segmentationInList(const QString fileName) const {
    int segCount = imageModel->rowCount(imageTreeView->currentIndex());
    for (int i = 0; i < segCount; ++i) {
        QModelIndex currentIdx = imageModel->index(i, 0, imageTreeView->currentIndex());
        if (imageModel->data(currentIdx, Qt::ToolTipRole) == fileName) {
            // Segmentation has already been loaded
            qDebug() << "Segmentation " << fileName << " already in the list";
            return true;
        }
    }

    return false;
}

bool QtMainWindow::toggleImageVisible() {
    QModelIndex modelIndex = imageModel->index(currentImageIndex, 0);
    irtkQtTreeItem *item = imageModel->getItem(modelIndex);
    bool visible = !item->data()->IsVisible();
    item->data()->SetVisible(visible);

    // Emit signal to update the visible icon in TreeView
    emit imageModel->dataChanged(modelIndex, modelIndex);

    bool clickImage = false;

    // If image becomes visible display it
    if (visible) {
        qDebug("Making image visible");

        try {
            item->data()->CreateImage();
            displaySingleImage(modelIndex);
            numDisplayedImages++;
            clickImage = true;
        }
        catch (irtkException) {
            createMessageBox("Invalid image file " + item->data()->GetPath(), QMessageBox::Critical);
            imageModel->removeRows(currentImageIndex, 1, QModelIndex());
            QList<irtkQtBaseViewer*>::iterator it;
            for (it = viewers.begin(); it != viewers.end(); ++it) {
                (*it)->UpdateKeysAfterIndexDeleted(currentImageIndex);
            }
            clickImage = false;
        }
    }
    // Otherwise delete it and decrease the number of visible images
    else {
        qDebug("Making image invisible");

        numDisplayedImages--;

        if (numDisplayedImages == 0) {
            disableViewerWidgets();
        }

        item->data()->DeleteImage();
        deleteSingleImage(currentImageIndex);
    }

    visualToolWidget->onlyTwoImagesVisible(numDisplayedImages == 2);

    return clickImage;
}

bool QtMainWindow::toggleSegmentationVisible() {
    QModelIndex index = imageTreeView->currentIndex();
    irtkQtTreeItem *item = imageModel->getItem(index);

    bool visible = !item->data()->IsVisible();
    item->data()->SetVisible(visible);

    // Emit signal to update the visible icon in TreeView
    emit imageModel->dataChanged(index, index);

    bool clickImage = false;

    // If image becomes visible display it
    if (visible) {
        qDebug("Making image visible");

        try {
            item->data()->CreateImage();
            displaySingleSegmentation(index);
            clickImage = true;
        }
        catch (irtkException) {
            createMessageBox("Invalid image file " + item->data()->GetPath(), QMessageBox::Critical);
            imageModel->removeRows(currentImageIndex, 1, imageModel->parent(index));
            clickImage = false;
        }
    }
    // Otherwise delete it and decrease the number of visible images
    else {
        qDebug("Making image invisible");
        item->data()->DeleteImage();
        deleteSingleSegmentation(index);
    }

    return clickImage;
}

void QtMainWindow::addToViewWidget(QWidget *widget) {
    QGridLayout *layout = dynamic_cast<QGridLayout*>(mainViewWidget->layout());

    // Add a new widget to the grid layout (every row fits 2 widgets)
    if ( viewerWidgets.size() % 2 == 0 )
        layout->addWidget(widget, layout->rowCount()-1, 1);
    else
        layout->addWidget(widget, layout->rowCount(), 0);

    if (singleViewerInScreen)
        widget->hide();
}

void QtMainWindow::addToViewWidget(QWidget *widget, int index) {
    QGridLayout *layout = dynamic_cast<QGridLayout*>(mainViewWidget->layout());

    // Add widget with specific index to the grid layout
    if ( index % 2 == 0 )
        layout->addWidget(widget, layout->rowCount(), 0);
    else
        layout->addWidget(widget, layout->rowCount()-1, 1);
}

void QtMainWindow::createMessageBox(QString message, QMessageBox::Icon icon) {
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setIcon(icon);
    msgBox.exec();
}

bool QtMainWindow::setDisplayedImages() {
    QtViewerWidget *viewerWidget = viewerWidgets[viewerWidgets.size()-1];
    irtkQtBaseViewer *viewer = viewers[viewers.size()-1];
    QtGlWidget *glWidget = viewerWidget->getGlWidget();

    bool atLeastOneImageVisible = false;

    int imageCount = imageModel->rowCount(QModelIndex());
    viewer->SetDimensions(glWidget->customWidth(), glWidget->customHeight());

    for (int i = 0; i < imageCount; ++i) {
        irtkQtTreeItem *item = imageModel->getItem(
                    imageModel->index(i, 0, QModelIndex()));
        if (item->data()->IsVisible()) {
            viewer->AddToDisplayedImages(item->data(), i);
            atLeastOneImageVisible = true;
        }
    }

    return atLeastOneImageVisible;
}

void QtMainWindow::displaySingleImage(QModelIndex index) {
    irtkQtBaseViewer *viewer;
    QtViewerWidget *viewerWidget;

    // Disconnect the viewers' signals
    disconnectViewerSignals();

    for (int i = 0; i < viewerWidgets.size(); i++) {
        viewer = viewers[i];
        viewerWidget = viewerWidgets[i];

        viewer->AddToDisplayedImages(imageModel->getItem(index)->data(), index.row());
        viewerWidget->setEnabled(true);
        viewer->SetDimensions(viewerWidget->getGlWidget()->customWidth(),
                              viewerWidget->getGlWidget()->customHeight());

        viewer->InitializeCurrentTransformation();
        viewer->CalculateCurrentOutput();
    }

    // Re-register the viewers' signals
    connectViewerSignals();
}

void QtMainWindow::displaySingleSegmentation(QModelIndex index) {
    irtkQtBaseViewer *viewer;

    // Disconnect the viewers' signals
    disconnectViewerSignals();

    QList<irtkQtBaseViewer*>::iterator it;
    for (it = viewers.begin(); it != viewers.end(); ++it) {
        viewer = *it;
        viewer->AddToDisplayedSegmentations(imageModel->getItem(index)->data(),
                                            imageModel->parent(index).row(), index.row());
        viewer->InitializeCurrentSegmentationTransformation();
        viewer->CalculateCurrentSegmentationOutput();
    }

    // Re-register the viewers' signals
    connectViewerSignals();
}

void QtMainWindow::deleteSingleImage(int index) {
    // Unregister the viewers' signals
    disconnectViewerSignals();

    QList<irtkQtBaseViewer*>::iterator it;
    for (it = viewers.begin(); it != viewers.end(); ++it) {
        (*it)->DeleteSingleImage(index);
    }

    // Re-register the viewers' signals
    connectViewerSignals();

    infoWidget->setImage(NULL);
    infoWidget->update();
    visualToolWidget->setEnabled(false);
}

void QtMainWindow::deleteSingleSegmentation(const QModelIndex &index) {
    // Unregister the viewers' signals
    disconnectViewerSignals();

    QList<irtkQtBaseViewer*>::iterator it;
    for (it = viewers.begin(); it != viewers.end(); ++it) {
        (*it)->DeleteSingleSegmentation(imageModel->parent(index).row(), index.row());
    }

    // Re-register the viewers' signals
    connectViewerSignals();

    infoWidget->setImage(NULL);
    infoWidget->update();
    visualToolWidget->setEnabled(false);
}

void QtMainWindow::deleteImages(QModelIndexList rowList) {
    int deleteCount = rowList.size();

    // Start deleting images from bottom to top
    for (int i = 0; i < deleteCount; i++) {
        currentImageIndex = rowList.back().row();

        bool visible = imageModel->getItem(rowList.back())->data()->IsVisible();

        if (imageModel->getItem(rowList.back())->parent()->parent() == 0) {
            if (visible) {
                numDisplayedImages--;

                if (numDisplayedImages == 0) {
                    disableViewerWidgets();
                }

                deleteSingleImage(currentImageIndex);
            }

            // Update the model
            imageModel->removeRows(currentImageIndex, 1, QModelIndex());

            // Update the map keys of the images currently displayed
            QList<irtkQtBaseViewer*>::iterator it;
            for (it = viewers.begin(); it != viewers.end(); ++it) {
                (*it)->UpdateKeysAfterIndexDeleted(currentImageIndex);
            }
        }
        else {
            if (visible) {
                deleteSingleSegmentation(rowList.back());
            }
            imageModel->removeRows(currentImageIndex, 1,
                                   imageModel->parent(rowList.back()));
        }

        rowList.pop_back();
    }

    setUpViewerWidgets();

    visualToolWidget->onlyTwoImagesVisible(numDisplayedImages == 2);
    if (numDisplayedImages == 2) {
        updateDrawables();
    }
}

void QtMainWindow::addChildrenToIndexList(QModelIndexList &deleteList) {
    int listLength = deleteList.length();
    int childCount = 0;
    QModelIndex curIndex = QModelIndex();

    for (int i = 0; i < listLength; ++i) {
        if ( (childCount = imageModel->rowCount(deleteList[i])) > 0 ) {
            for (int j = 0; j < childCount; ++j) {
                curIndex = imageModel->index(j, 0, deleteList[i]);
                if ( deleteList.indexOf(curIndex) == -1 ) {
                    deleteList.push_back(curIndex);
                }
            }
        }
    }
}

void QtMainWindow::setUpViewerWidgets() {
    // Update the viewers
    QtViewerWidget *viewerWidget;
    irtkQtBaseViewer *viewer;

    // Disconnect the viewers' signals
    disconnectViewerSignals();

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets[i];
        viewer = viewers[i];

        viewerWidget->setMaximumSlice(viewer->GetSliceNumber());
        viewerWidget->setInvertedAxes(viewer->GetAxisInverted());
        viewerWidget->setCurrentSlice(viewer->GetCurrentSlice());

        double originX, originY, originZ;
        viewer->GetOrigin(originX, originY, originZ);
        viewerWidget->getGlWidget()->setWorldOrigin(originX, originY, originZ);
    }

    // Re-register the viewers' signals
    connectViewerSignals();
}

void QtMainWindow::updateDrawables() {
    QList<QtViewerWidget*>::iterator wit = viewerWidgets.begin();
    QList<irtkQtBaseViewer*>::iterator bit;

    for (bit = viewers.begin(); bit != viewers.end(); ++bit, ++wit) {
        (*wit)->getGlWidget()->updateSegmentation(
                    QVector<QRgb*>::fromStdVector( (*bit)->GetSegmentationDrawable()) );
        (*wit)->getGlWidget()->updateDrawable(
                    QVector<QRgb**>::fromStdVector( (*bit)->GetDrawable() ) );
    }
}

void QtMainWindow::createConfigurationViewerList() {
    QList<irtkQtConfigurationViewer> viewerList;
    bool is2D;

    for (int i = 0; i < viewerWidgets.size(); i++) {
        irtkQtConfigurationViewer viewer;
        is2D = true;

        irtkQtBaseViewer::irtkViewMode viewMode = viewers[i]->GetViewMode();
        switch (viewMode) {
        case irtkQtBaseViewer::VIEW_AXIAL:
            viewer.type = "Axial";
            break;
        case irtkQtBaseViewer::VIEW_SAGITTAL:
            viewer.type = "Sagittal";
            break;
        case irtkQtBaseViewer::VIEW_CORONAL:
            viewer.type = "Coronal";
            break;
        case irtkQtBaseViewer::VIEW_3D:
            viewer.type = "3D";
            is2D = false;
            break;
        };

        if (is2D) {
            Qt2dViewerWidget *twoDWidget = dynamic_cast<Qt2dViewerWidget*>(viewerWidgets[i]);
            viewer.labelsVisible = twoDWidget->labelsVisible();
            viewer.cursorVisible = twoDWidget->cursorVisible();
        }
        else {
            viewer.labelsVisible = viewer.cursorVisible = false;
        }

        viewer.fullScreen = singleViewerInScreen && viewerWidgets[i]->isVisible();
        viewer.imageOriginVisible = viewerWidgets[i]->imageOriginVisible();
        viewer.linked = viewerWidgets[i]->isLinked();

        viewers[i]->GetOrigin(viewer.origin[0], viewer.origin[1], viewer.origin[2]);
        viewers[i]->GetResolution(viewer.resolution[0], viewer.resolution[1], viewer.resolution[2]);

        viewerList.push_back(viewer);
    }

    irtkQtConfiguration::Instance()->SetViewerList(viewerList);
}

void QtMainWindow::loadConfigurationImageList() {
    QList<irtkQtConfigurationImage> imageList;
    imageList = irtkQtConfiguration::Instance()->GetImageList();

    QStringList fileList;
    QList<irtkQtConfigurationImage>::iterator it;
    for (it = imageList.begin(); it != imageList.end(); ++it) {
        fileList.push_back(it->fileName);
    }

    // Load list of images in imageListView
    loadImages(fileList);

    QStringList modeList = irtkQtLookupTable::GetColorModeList();
    QStringList interpolationList = irtkQtImageObject::GetInterpolationModeList();

    int imageCount = imageModel->rowCount();
    for (int i = 0; i < imageCount; i++) {
        irtkQtImageObject *image = imageModel->getItem(
                    imageModel->index(i, 0))->data();
        if (image->IsVisible()) {
            currentImageIndex = i;
            toggleImageVisible();

            int index = -1;
            for (int j = 0; j < modeList.size(); j++) {
                if (modeList[j] == imageList[i].colormap) {
                    index = j;
                    break;
                }
            }
            visualToolWidget->setColormap(index);

            index = -1;
            for (int j = 0; j < interpolationList.size(); j++) {
                if (interpolationList[j] == imageList[i].interpolation) {
                    index = j;
                    break;
                }
            }
            visualToolWidget->setInterpolation(index);

            visualToolWidget->setOpacity(imageList[i].opacity);
            visualToolWidget->setDisplayMin(imageList[i].minDisplay);
            visualToolWidget->setDisplayMax(imageList[i].maxDisplay);
        }
    }
}

void QtMainWindow::loadConfigurationViewerList() {
    QList<irtkQtConfigurationViewer> viewerList;
    viewerList = irtkQtConfiguration::Instance()->GetViewerList();

    QList<irtkQtConfigurationViewer>::iterator it;
    for (it = viewerList.begin(); it != viewerList.end(); ++it) {
        if (it->type == "3D") {
            create3dView();
        }
        else {
            if (it->type == "Axial") {
                createAxialView();
            }
            else if (it->type == "Sagittal") {
                createSagittalView();
            }
            else if (it->type == "Coronal") {
                createCoronalView();
            }

            Qt2dViewerWidget *viewerWidget = dynamic_cast<Qt2dViewerWidget*>(viewerWidgets.back());
            viewerWidget->setCursorVisible(it->cursorVisible);
            viewerWidget->setLabelsVisible(it->labelsVisible);
        }

        viewerWidgets.back()->setImageOriginVisible(it->imageOriginVisible);
        viewerWidgets.back()->setLinked(it->linked);
        if (it->fullScreen) viewerWidgets.back()->expandWindow();

        viewers.back()->SetOrigin(it->origin[0], it->origin[1], it->origin[2]);
        viewers.back()->SetResolution(it->resolution[0], it->resolution[1],
                it->resolution[2]);
    }

    setUpViewerWidgets();
}

void QtMainWindow::openImage() {
#ifdef Q_OS_MAC
    QString selfilter = tr("IMG (*.gipl *.z *.hdr *.gz *.nii)");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), QDir::homePath(),
                                                          tr("All files (*.*);;IMG (*.gipl *.z *.hdr *.gz *.nii)" ),
                                                          &selfilter);
#else
    QString selfilter = tr("IMG (*.gipl *.gipl.z *.hdr *.hdr.gz *.nii *.nii.gz)");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), QDir::homePath(),
                                                          tr("All files (*.*);;IMG (*.gipl *.gipl.z *.hdr *.hdr.gz *.nii *.nii.gz)" ),
                                                          &selfilter);
#endif

    loadImages(fileNames);
}

void QtMainWindow::clearImages() {
    QModelIndexList rowList;

    for (int i = 0; i < imageModel->rowCount(); i++) {
        rowList.push_back(imageModel->index(i, 0, QModelIndex()));
    }

    addChildrenToIndexList(rowList);
    deleteImages(rowList);
}

void QtMainWindow::saveScreenshot() {
    QList<QImage> images;
    QImage currentImage;

    int maxWidth = 0, maxHeight = 0;
    // Get the maximum viewer width and height and store viewer images in list
    QList<QtViewerWidget*>::iterator it;
    for (it = viewerWidgets.begin(); it != viewerWidgets.end(); ++it) {
        currentImage = (*it)->getGlWidget()->getDisplayedImage();
        images.push_back(currentImage);
        if (currentImage.width() > maxWidth) maxWidth = currentImage.width();
        if (currentImage.height() > maxHeight) maxHeight = currentImage.height();
    }

    int width = maxWidth;
    int height = maxHeight;
    // Specify the collage width and height
    if (viewerWidgets.size() > 1) {
        width = maxWidth * 2;
        height = maxHeight * ((int) (viewerWidgets.size()-1) / 2 + 1);
    }

    // Initialize the collage image and fill it with black
    QImage collage(width, height, QImage::Format_RGB32);
    collage.fill(Qt::black);
    QPainter paint;
    paint.begin(&collage);

    // Draw each individual image in the collage image
    for (int i = 0; i < images.size(); i++) {
        int x = (i % 2) * maxWidth;
        int y = (i / 2) * maxHeight;

        currentImage = images[i];
        paint.drawImage(QRect(x, y, currentImage.width(),
                              currentImage.height()), currentImage);
    }

    paint.end();

    // Save image in the specified file
    QString fileName = QFileDialog::getSaveFileName(
                this,tr("Save screenshot as"),
                "", tr("Images (*.png *.xpm *.jpg)"));
    if (collage.save(fileName)) {
        createMessageBox("Image successfully saved in " + fileName, QMessageBox::Information);
    }
    else {
        qCritical("Could not save screenshot in %s", qPrintable(fileName));
    }
}

void QtMainWindow::viewImages() {
    QtViewerWidget *viewerWidget = viewerWidgets[viewerWidgets.size()-1];
    irtkQtBaseViewer *viewer = viewers[viewers.size()-1];

    // Disconnect the viewers' signals
    disconnectViewerSignals();

    // Load the images to be displayed and add them to the viewers
    if ( !setDisplayedImages() ) {
        return;
    }

    // Initialize the transformations and calculate output images
    viewer->InitializeTransformation();
    viewer->CalculateOutputImages();

    // Set up the viewer widget
    viewerWidget->setEnabled(true);
    viewerWidget->setInvertedAxes(viewer->GetAxisInverted());
    viewerWidget->setMaximumSlice(viewer->GetSliceNumber());
    viewerWidget->setCurrentSlice(viewer->GetCurrentSlice());

    double originX, originY, originZ;
    viewer->GetOrigin(originX, originY, originZ);
    viewerWidget->getGlWidget()->setWorldOrigin(originX, originY, originZ);
    viewerWidget->getGlWidget()->updateDrawable(
                QVector<QRgb**>::fromStdVector(viewer->GetDrawable()));

    // Re-register the viewers' signals
    connectViewerSignals();
}

void QtMainWindow::deleteSelectedImages() {
    qDebug("Deleting images");

    // Create list with rows to be deleted
    QModelIndexList indexList = imageTreeView->selectionModel()->selectedIndexes();
    QModelIndexList deleteList;

    for (int i = indexList.size() - 1; i >= 0 ; --i) {
        if ( imageModel->parent(indexList[i]) != QModelIndex() ) {
            deleteList.push_front(indexList.takeAt(i));
        }
    }

    QModelIndexList::const_iterator it;
    for (it = indexList.constEnd() - 1; it >= indexList.constBegin(); --it) {
        deleteList.push_front(*it);
    }

    addChildrenToIndexList(deleteList);
    deleteImages(deleteList);
}

void QtMainWindow::toggleVisible() {
    QModelIndex curIndex = imageTreeView->currentIndex();
    irtkQtTreeItem *itemParent = imageModel->getItem(curIndex)->parent();
    bool clickImage;

    if ( itemParent->parent() == 0 ) {
        clickImage = toggleImageVisible();
    }
    else {
        if ( !itemParent->data()->IsVisible() &&
             !imageModel->getItem(curIndex)->data()->IsVisible() ) {
            currentImageIndex = imageModel->parent(curIndex).row();
            toggleImageVisible();
        }
        clickImage = toggleSegmentationVisible();
    }

    // Update the viewers
    setUpViewerWidgets();
    updateDrawables();

    // Emulate click to show image information
    if (clickImage) {
        imageTreeView->setCurrentIndex(curIndex);
        emit imageTreeView->clicked(curIndex);
    }
}

void QtMainWindow::openSegmentation() {
#ifdef Q_OS_MAC
    QString selfilter = tr("IMG (*.gipl *.z *.hdr *.gz *.nii *.vtk)");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), QDir::homePath(),
                                                          tr("All files (*.*);; SEG(*.gipl *.z *.hdr *.gz *.nii *.vtk)"),
                                                          &selfilter);
#else
    QString selfilter = tr("IMG (*.gipl *.gipl.z *.hdr *.hdr.gz *.nii *.nii.gz *.vtk");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), QDir::homePath(),
                                                          tr("All files (*.*);;IMG (*.gipl *.gipl.z *.hdr *.hdr.gz *.nii *.nii.gz *.vtk)" ),
                                                          &selfilter);
#endif

    loadSegmentations(fileNames);
}

void QtMainWindow::changeLabelColor() {
    irtkQtTreeItem *item = imageModel->getItem(imageTreeView->currentIndex());

    QColor selectedColor = QColorDialog::getColor(item->data()->GetLabelColor(), this,
                                                  "Choose label color",
                                                  QColorDialog::ShowAlphaChannel);

    item->data()->SetLabelColor(selectedColor);

    if (item->data()->IsVisible()) {
        QList<irtkQtBaseViewer*>::iterator it;
        for (it = viewers.begin(); it != viewers.end(); ++it) {
            (*it)->SetLabelColor(imageModel->parent(imageTreeView->currentIndex()).row(),
                                 imageTreeView->currentIndex().row(), selectedColor);
        }

        updateDrawables();
    }
}

void QtMainWindow::zoomIn() {
    qDebug("Zooming in");

    Qt2dViewerWidget *viewerWidget;
    irtkQtBaseViewer *viewer;

    QFuture<void> *threads = new QFuture<void>[viewers.size()];
    int t_index = 0;

    // Increase pixel size in parallel for the different viewers
    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = dynamic_cast<Qt2dViewerWidget*>(viewerWidgets[i]);
        viewer = viewers[i];

        if (viewerWidget != 0) {
            if (viewerWidget->getGlWidget()->isEnabled()) {
                viewer->IncreaseResolution();
                threads[t_index] = QtConcurrent::run(CalculateOutputImage, viewer);
                t_index++;
            }
        }
    }

    // Wait until all threads are finished
    for (int i = 0; i < t_index; i++) {
        threads[i].waitForFinished();
    }

    // Update the viewers
    updateDrawables();

    delete [] threads;
}

void QtMainWindow::zoomOut() {
    qDebug("Zooming out");

    Qt2dViewerWidget *viewerWidget;
    irtkQtBaseViewer *viewer;

    QFuture<void> *threads = new QFuture<void>[viewers.size()];
    int t_index = 0;

    // Decrease pixel size in parallel for the different viewers
    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = dynamic_cast<Qt2dViewerWidget*>(viewerWidgets[i]);
        viewer = viewers.at(i);

        if (viewerWidget != 0) {
            if (viewerWidget->getGlWidget()->isEnabled()) {
                viewer->DecreaseResolution();
                threads[t_index] = QtConcurrent::run(CalculateOutputImage, viewer);
                t_index++;
            }
        }
    }

    // Wait until all threads are finished
    for (int i = 0; i < t_index; i++) {
        threads[i].waitForFinished();
    }

    // Update the viewers
    updateDrawables();

    delete [] threads;
}

void QtMainWindow::showOnlyThisWidget() {
    singleViewerInScreen = !singleViewerInScreen;

    if (!singleViewerInScreen) {
        qDebug("Exiting single viewer mode");
        saveScreenshotAction->setEnabled(true);
        screenshotToolbarAction->setEnabled(true);

        for (int i = 0; i < viewerWidgets.size(); i++) {
            viewerWidgets[i]->show();
        }
    }
    else {
        qDebug("Entering single viewer mode");
        saveScreenshotAction->setEnabled(false);
        screenshotToolbarAction->setEnabled(false);

        QWidget *senderWidget = dynamic_cast<QWidget*>(sender());
        for (int i = 0; i < viewerWidgets.size(); i++) {
            if (viewerWidgets[i] != senderWidget)
                viewerWidgets[i]->hide();
        }
    }
}

void QtMainWindow::deleteThisWidget() {
    qDebug("Deleting viewer widget");

    QWidget *senderWidget = dynamic_cast<QWidget*>(sender());
    QLayout *layout = mainViewWidget->layout();

    for (int i = 0; i < viewerWidgets.size(); i++) {
        layout->removeWidget(viewerWidgets[i]);
        if (senderWidget == viewerWidgets[i]) {
            delete viewerWidgets.takeAt(i);
            delete viewers.takeAt(i);
        }
    }
    for (int i = 0; i < viewerWidgets.size(); i++) {
        addToViewWidget(viewerWidgets[i], i);
        viewerWidgets[i]->show();
    }
}

void QtMainWindow::createAxialView() {
    qDebug("Creating axial view");
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_AXIAL));
}

void QtMainWindow::createCoronalView() {
    qDebug("Creating coronal view");
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_CORONAL));
}

void QtMainWindow::createSagittalView() {
    qDebug("Creating saggital view");
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_SAGITTAL));
}

void QtMainWindow::createOrthogonalView() {
    qDebug("Creating orthogonal view");

    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_AXIAL));
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_CORONAL));
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_SAGITTAL));
}

void QtMainWindow::create3dView() {
    qDebug("Creating 3D view");
    addToViewWidget(createThreeDimensionalView());
}

void QtMainWindow::clearViews() {
    qDebug("Clearing views");

    clearLists();
    singleViewerInScreen = false;
}

void QtMainWindow::updateOrigin(double x, double y, double z) {
    qDebug("Updating origin");

    irtkQtBaseViewer *senderViewer, *viewer;
    QtViewerWidget *viewerWidget;
    senderViewer = dynamic_cast<irtkQtBaseViewer*>(sender());

    disconnectViewerSignals();

    int index = 0;
    while (viewers[index] != senderViewer)
        index++;
    bool isSenderLinked = viewerWidgets[index]->isLinked();

    QFuture<void> *threads = new QFuture<void>[viewers.size()];
    int t_index = 0;

    // Calculate the new output images
    for (int i = 0; i < viewers.size(); i++) {
        viewer = viewers[i];
        viewerWidget = viewerWidgets[i];

        if ( viewerWidget->getGlWidget()->isEnabled() &&
             ( (isSenderLinked && viewerWidget->isLinked()) || (viewer == senderViewer) ) ) {
            viewer->SetOrigin(x, y, z);
            threads[t_index] = QtConcurrent::run(CalculateOutputImage, viewer);
            t_index++;
        }
    }

    for (int i = 0; i < t_index; i++) {
        threads[i].waitForFinished();
    }

    // Update the viewers with the new output images
    for (int i = 0; i < viewers.size(); i++) {
        viewer = viewers[i];
        viewerWidget = viewerWidgets[i];

        if ( viewerWidget->getGlWidget()->isEnabled() &&
             ( (isSenderLinked && viewerWidget->isLinked()) || (viewer == senderViewer) ) ) {
            viewerWidget->getGlWidget()->setWorldOrigin(x, y, z);
            viewerWidget->setCurrentSlice(viewer->GetCurrentSlice());
            viewerWidget->getGlWidget()->updateSegmentation(
                        QVector<QRgb*>::fromStdVector(viewer->GetSegmentationDrawable()));
            viewerWidget->getGlWidget()->updateDrawable(
                        QVector<QRgb**>::fromStdVector(viewer->GetDrawable()));
        }
    }

    connectViewerSignals();

    delete [] threads;
}

void QtMainWindow::minDisplayValueChanged(double value) {
    qDebug("Minimum display value changed to %s",
           qPrintable(QString::number(value)));

    QModelIndex index = imageTreeView->currentIndex();
    irtkQtImageObject *item = imageModel->getItem(index)->data();
    if ( index.isValid() && (item != NULL) ) {
        if (item->IsVisible()) {
            item->SetMinDisplayValue(value);
            updateDrawables();
        }
    }
}

void QtMainWindow::maxDisplayValueChanged(double value) {
    qDebug("Maximum display value changed to %s",
           qPrintable(QString::number(value)));

    QModelIndex index = imageTreeView->currentIndex();
    irtkQtImageObject *item = imageModel->getItem(index)->data();
    if ( index.isValid() && (item != NULL) ) {
        if (item->IsVisible()) {
            item->SetMaxDisplayValue(value);
            updateDrawables();
        }
    }
}

void QtMainWindow::colormapIndexChanged(int mode) {
    qDebug("Colormap changed");

    QModelIndex index = imageTreeView->currentIndex();
    irtkQtImageObject *item = imageModel->getItem(index)->data();
    if ( index.isValid() && (item != NULL) ) {
        if (item->IsVisible()) {
            item->SetColormap(static_cast<irtkQtLookupTable::irtkColorMode>(mode));
            updateDrawables();
        }
    }
}

void QtMainWindow::interpolationIndexChanged(int mode) {
    qDebug("Interpolation changed");

    irtkQtImageObject::irtkQtInterpolationMode md =
            static_cast<irtkQtImageObject::irtkQtInterpolationMode>(mode);

    QModelIndex index = imageTreeView->currentIndex();
    irtkQtImageObject *item = imageModel->getItem(index)->data();
    if ( index.isValid() && (item != NULL) ) {
        if (item->IsVisible()) {
            // Update interpolation info in irtkQtImageObject
            item->SetInterpolation(md);

            // Update the interpolation method in the viewers
            QList<irtkQtBaseViewer*>::iterator it;
            for (it = viewers.begin(); it != viewers.end(); ++it) {
                (*it)->SetInterpolationMethod(index.row(), md);
            }

            updateDrawables();
        }
    }
}

void QtMainWindow::opacityValueChanged(int value) {
    qDebug("Opacity value changed");

    QModelIndex index = imageTreeView->currentIndex();
    irtkQtImageObject *item = imageModel->getItem(index)->data();
    if ( index.isValid() && (item != NULL) ) {
        if (item->IsVisible()) {
            item->SetOpacity(value);
            updateDrawables();
        }
    }
}

void QtMainWindow::treeViewDoubleClicked(QModelIndex index) {
    qDebug("List view double clicked");
    currentImageIndex = index.row();
    toggleVisible();
}

void QtMainWindow::treeViewClicked(QModelIndex index) {
    irtkQtImageObject* imageObject = imageModel->getItem(index)->data();
    bool isImage = (imageModel->getItem(imageTreeView->currentIndex())->parent()->parent() == 0);

    visualToolWidget->blockSignals(true);

    if (imageObject->IsVisible() && isImage) {
        visualToolWidget->setEnabled(true);
        visualToolWidget->setColormap(imageObject->GetColormap());
        visualToolWidget->setInterpolation(imageObject->GetInterpolation());
        visualToolWidget->setOpacity(imageObject->GetOpacity());
        visualToolWidget->setMinimumImageValue(imageObject->GetMinImageValue());
        visualToolWidget->setMaximumImageValue(imageObject->GetMaxImageValue());
        visualToolWidget->setDisplayMin(imageObject->GetMinDisplayValue());
        visualToolWidget->setDisplayMax(imageObject->GetMaxDisplayValue());
    }
    else {
        visualToolWidget->setEnabled(false);
    }

    visualToolWidget->blockSignals(false);

    infoWidget->setImage(imageObject->GetImage());
    infoWidget->update();

    moveUpAction->setEnabled(isImage);
    moveDownAction->setEnabled(isImage);
}

void QtMainWindow::treeViewShowContextMenu(const QPoint &pos) {
    if (imageModel) {
        QMenu imageMenu;
        QPoint globalPos = imageTreeView->mapToGlobal(pos);

        int selectedCount = imageTreeView->selectionModel()->selectedIndexes().size();

        if ( selectedCount < 1 ) {
            return;
        }
        if ( selectedCount == 1) {
            currentImageIndex = imageTreeView->indexAt(pos).row();

            imageMenu.addAction(toggleVisibleAction);
            if (imageModel->getItem(imageTreeView->indexAt(pos))->data()->IsVisible()) {
                toggleVisibleAction->setChecked(true);
            }
            else {
                toggleVisibleAction->setChecked(false);
            }

            // Add loadSegmentationAction if the parent of the item is the root
            if (imageModel->getItem(imageTreeView->indexAt(pos))->parent()->parent() == 0) {
                imageMenu.addAction(loadSegmentationAction);
            }
            // Else add the option to change the label color
            else {
                imageMenu.addAction(changeLabelAction);
            }
        }
        if ( selectedCount > 0 ) {
            imageMenu.addAction(deleteImageAction);
            imageMenu.exec( globalPos );
        }
    }
}

void QtMainWindow::moveImageUp() {
    qDebug("Moving image up");

    int index = imageTreeView->currentIndex().row();
    if (imageModel && index > 0) {
        imageModel->moveRow(QModelIndex(), index, QModelIndex(), index - 1);
        imageTreeView->setCurrentIndex(imageModel->index(index-1, 0));

        for (int i = 0; i < viewers.size(); i++) {
            viewers[i]->MoveImage(index, index-1);
        }

        setUpViewerWidgets();
        updateDrawables();
    }
}

void QtMainWindow::moveImageDown() {
    qDebug("Moving image down");

    int index = imageTreeView->currentIndex().row();
    if (imageModel && index >= 0 && (index < imageModel->rowCount() - 1)) {
        imageModel->moveRow(QModelIndex(), index + 1, QModelIndex(), index);
        imageTreeView->setCurrentIndex(imageModel->index(index+1, 0));

        for (int i = 0; i < viewers.size(); i++) {
            viewers[i]->MoveImage(index, index+1);
        }

        setUpViewerWidgets();
        updateDrawables();
    }
}

void QtMainWindow::blendModeChanged(int mode) {
    for (int i = 0; i < viewers.size(); i++) {
        viewers[i]->SetBlendMode(mode);
    }
    updateDrawables();
}

void QtMainWindow::displayMixValueChanged(double value) {
    for (int i = 0; i < viewers.size(); i++) {
        viewers[i]->SetMixValue(value);
    }
    updateDrawables();
}

void QtMainWindow::loadConfigurationFile() {
    // Load configuration file
    QString selfilter = tr("XML (*.xml)");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), QDir::homePath(),
                                                    tr("All files (*.*);;XML (*.xml)" ),
                                                    &selfilter);

    if (irtkQtConfiguration::Instance()->Read(fileName)) {
        clearImages();
        clearViews();
        loadConfigurationImageList();
        loadConfigurationViewerList();
    }
    else {
        createMessageBox("Could not read configuration file", QMessageBox::Critical);
    }
}

void QtMainWindow::saveConfigurationFile() {
    // Save image in the specified file
    QString fileName = QFileDialog::getSaveFileName(
                this,tr("Save screenshot as"),
                "", tr("XML (*.xml)"));

    createConfigurationViewerList();
    irtkQtConfiguration::Instance()->Write(fileName);
}
