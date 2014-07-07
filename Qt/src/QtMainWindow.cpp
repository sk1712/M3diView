#include <QtMainWindow.h>

#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QGridLayout>
#include <QListView>
#include <QMenuBar>
#include <QScrollArea>
#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>

/// free function used to calculate the output images
/// in parallel for the different viewers
void CalculateOutputImage(irtkQtBaseViewer* viewer) {
    viewer->CalculateOutputImages();
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
    imageModel = NULL;

    // By default add orthogonal and 3D view
    createOrthogonalView();
    create3dView();
    visualToolWidget->onlyTwoImagesVisible(false);
}

QtMainWindow::~QtMainWindow() {
    clearLists();
    irtkQtConfiguration::Destroy();
}

void QtMainWindow::loadImages(const QStringList &fileList) {
    irtkQtConfiguration* instance = irtkQtConfiguration::Instance();

    // Create an irtkImageObject for each new image
    QStringList::const_iterator it;
    for (it = fileList.constBegin(); it != fileList.constEnd(); it++) {
        if ( !imageInList(*it) ) {
            qDebug("Creating image for file %s", qPrintable((*it)));
            instance->CreateImage((*it));
        }
    }

    // Update the image model
    delete imageModel;
    imageModel = new irtkImageListModel(instance->GetImageObjectList());
    imageListView->setModel(imageModel);
}

void QtMainWindow::createDockWindows() {
    // Create image list dock widget
    QDockWidget *dock = new QDockWidget(tr("Image List"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    imageListView = new QListView(dock);
    imageListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    // Show a menu on list view right click
    imageListView->setContextMenuPolicy(Qt::CustomContextMenu);
    dock->setWidget(imageListView);
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

    deleteImageAction = new QAction(tr("Remove from list"), this);
    deleteImageAction->setIcon(QIcon(":/icons/erase.png"));
}

void QtMainWindow::connectWindowSignals() {
    // List view signals
    connect(imageListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(listViewDoubleClicked(QModelIndex)));
    connect(imageListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(listViewClicked(QModelIndex)));
    connect(imageListView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(listViewShowContextMenu(QPoint)));

    // Image menu signals
    connect(toggleVisibleAction, SIGNAL(triggered()), this, SLOT(toggleImageVisible()));
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
    for (it = viewerWidgets.begin(); it != viewerWidgets.end(); it++) {
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
    QList<irtkQtImageObject*> list = irtkQtConfiguration::Instance()->GetImageObjectList();
    QList<irtkQtImageObject*>::const_iterator it;
    for (it = list.constBegin(); it != list.constEnd(); it++) {
        if ((*it)->GetPath() == fileName) {
            // Image has already been loaded
            qDebug() << "Image " << fileName << " already in the list";
            return true;
        }
    }
    return false;
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

    QList<irtkQtImageObject*> imageList = irtkQtConfiguration::Instance()->GetImageObjectList();
    QList<irtkQtImageObject*>::const_iterator it;

    viewer->SetDimensions(glWidget->customWidth(), glWidget->customHeight());

    for (it = imageList.constBegin(); it != imageList.constEnd(); it++) {
        if ((*it)->IsVisible()) {
            viewer->AddToDisplayedImages(*it, imageList.indexOf(*it));
            atLeastOneImageVisible = true;
        }
    }

    return atLeastOneImageVisible;
}

void QtMainWindow::displaySingleImage(int index) {
    irtkQtBaseViewer *viewer;
    QtViewerWidget *viewerWidget;

    QList<irtkQtImageObject*> & imageList = irtkQtConfiguration::Instance()->GetImageObjectList();

    // Disconnect the viewers' signals
    disconnectViewerSignals();

    for (int i = 0; i < viewerWidgets.size(); i++) {
        viewer = viewers[i];
        viewerWidget = viewerWidgets[i];

        viewer->AddToDisplayedImages(imageList[index], index);
        viewerWidget->setEnabled(true);
        viewer->SetDimensions(viewerWidget->getGlWidget()->customWidth(),
                              viewerWidget->getGlWidget()->customHeight());

        viewer->InitializeCurrentTransformation();
        viewer->CalculateCurrentOutput();
    }

    // Re-register the viewers' signals
    connectViewerSignals();
}

void QtMainWindow::deleteSingleImage(int index) {
    // Unregister the viewers' signals
    disconnectViewerSignals();

    QList<irtkQtBaseViewer*>::iterator it;
    for (it = viewers.begin(); it != viewers.end(); it++) {
        (*it)->DeleteSingleImage(index);
    }

    // Re-register the viewers' signals
    connectViewerSignals();

    infoWidget->setImage(NULL);
    infoWidget->update();
    visualToolWidget->setEnabled(false);
}

void QtMainWindow::deleteImages(QList<int> rowList) {
     QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();

     // Start deleting images from bottom to top
     for (int i = 0; i < rowList.size(); i++) {
         currentImageIndex = rowList[i];
         bool visible = list[currentImageIndex]->IsVisible();

         if (visible) {
             numDisplayedImages--;

             if (numDisplayedImages == 0) {
                 disableViewerWidgets();
             }

             deleteSingleImage(currentImageIndex);
         }

         // Delete item from the list
         delete list.takeAt(currentImageIndex);

         // Update the map keys of the images currently displayed
         QList<irtkQtBaseViewer*>::iterator it;
         for (it = viewers.begin(); it != viewers.end(); it++) {
             (*it)->UpdateKeysAfterIndexDeleted(currentImageIndex);
         }
     }

     setUpViewerWidgets();
     visualToolWidget->onlyTwoImagesVisible(numDisplayedImages == 2);
     if (numDisplayedImages == 2) {
         updateDrawables();
     }

     // Update the model
     delete imageModel;
     imageModel = new irtkImageListModel(list);
     imageListView->setModel(imageModel);
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
    }

    // Re-register the viewers' signals
    connectViewerSignals();
}

void QtMainWindow::updateDrawables() {
    for (int i = 0; i < viewers.size(); i++) {
        viewerWidgets[i]->getGlWidget()->updateDrawable(
                    QVector<QRgb**>::fromStdVector(viewers[i]->GetDrawable()));
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
    for (it = imageList.begin(); it != imageList.end(); it++) {
        fileList.push_back(it->fileName);
    }

    // Load list of images in imageListView
    loadImages(fileList);

    QStringList modeList = irtkQtLookupTable::GetColorModeList();
    QStringList interpolationList = irtkQtImageObject::GetInterpolationModeList();

    QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();
    for (int i = 0; i < list.size(); i++) {
        if (imageList[i].visible) {
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
    for (it = viewerList.begin(); it != viewerList.end(); it++) {
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
    if (imageModel) {
        QList<int> rowList;

        for (int i = 0; i < imageModel->rowCount(); i++) {
            rowList.push_front(i);
        }

        deleteImages(rowList);
    }
}

void QtMainWindow::saveScreenshot() {
    QList<QImage> images;
    QImage currentImage;

    int maxWidth = 0, maxHeight = 0;
    // Get the maximum viewer width and height and store viewer images in list
    QList<QtViewerWidget*>::iterator it;
    for (it = viewerWidgets.begin(); it != viewerWidgets.end(); it++) {
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
    viewerWidget->getGlWidget()->updateDrawable(
                QVector<QRgb**>::fromStdVector(viewer->GetDrawable()));

    // Re-register the viewers' signals
    connectViewerSignals();
}

void QtMainWindow::deleteSelectedImages() {
    qDebug("Deleting images");

    QModelIndexList indexList = imageListView->selectionModel()->selectedIndexes();
    QList<int> rowList;

    // Create list with rows to be deleted
    for (int i = 0; i < indexList.size(); i++) {
        rowList.push_back(indexList[i].row());
    }

    // Sort the rows in descending order
    qSort(rowList.begin(), rowList.end(), qGreater<int>());

    deleteImages(rowList);
}

void QtMainWindow::toggleImageVisible() {
    QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();

    bool visible = !list[currentImageIndex]->IsVisible();
    list[currentImageIndex]->SetVisible(visible);

    bool clickImage = false;

    // If image becomes visible display it
    if (visible) {
        qDebug("Making image visible");

        try {
            list[currentImageIndex]->CreateImage();
            displaySingleImage(currentImageIndex);
            numDisplayedImages++;
            clickImage = true;
        }
        catch (irtkException) {
            createMessageBox("Invalid image file " + list[currentImageIndex]->GetPath(), QMessageBox::Critical);
            list.removeAt(currentImageIndex);
            QList<irtkQtBaseViewer*>::iterator it;
            for (it = viewers.begin(); it != viewers.end(); it++) {
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

        list[currentImageIndex]->DeleteImage();
        deleteSingleImage(currentImageIndex);
    }

    setUpViewerWidgets();
    visualToolWidget->onlyTwoImagesVisible(numDisplayedImages == 2);
    if (numDisplayedImages == 2) {
        updateDrawables();
    }

    delete imageModel;
    imageModel = new irtkImageListModel(list);
    imageListView->setModel(imageModel);

    // Emulate click to show image information
    if (clickImage) {
        QModelIndex idx = imageModel->index(currentImageIndex, 0);
        imageListView->setCurrentIndex(idx);
        emit imageListView->clicked(idx);
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
            viewerWidget->setCurrentSlice(viewer->GetCurrentSlice());
            viewerWidget->getGlWidget()->updateDrawable(
                        QVector<QRgb**>::fromStdVector(viewer->GetDrawable()));

            double originX, originY, originZ;
            viewer->GetOrigin(originX, originY, originZ);
            viewerWidget->setWorldOrigin(originX, originY, originZ);
        }
    }

    connectViewerSignals();

    delete [] threads;
}

void QtMainWindow::minDisplayValueChanged(double value) {
    qDebug("Minimum display value changed to %s",
           qPrintable(QString::number(value)));

    QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        if (list[index]->IsVisible()) {
            list[index]->SetMinDisplayValue(value);
            updateDrawables();
        }
    }
}

void QtMainWindow::maxDisplayValueChanged(double value) {
    qDebug("Maximum display value changed to %s",
           qPrintable(QString::number(value)));

    QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        if (list[index]->IsVisible()) {
            list[index]->SetMaxDisplayValue(value);
            updateDrawables();
        }
    }
}

void QtMainWindow::colormapIndexChanged(int mode) {
    qDebug("Colormap changed");

    QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        if (list[index]->IsVisible()) {
            list[index]->SetColormap(static_cast<irtkQtLookupTable::irtkColorMode>(mode));
            updateDrawables();
        }
    }
}

void QtMainWindow::interpolationIndexChanged(int mode) {
    qDebug("Interpolation changed");

    QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();
    irtkQtImageObject::irtkQtInterpolationMode md =
            static_cast<irtkQtImageObject::irtkQtInterpolationMode>(mode);

    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        if (list[index]->IsVisible()) {
            // Update interpolation info in irtkQtImageObject
            list[index]->SetInterpolation(md);

            // Update the interpolation method in the viewers
            QList<irtkQtBaseViewer*>::iterator it;
            for (it = viewers.begin(); it != viewers.end(); it++) {
                (*it)->SetInterpolationMethod(index, md);
            }

            updateDrawables();
        }
    }
}

void QtMainWindow::opacityValueChanged(int value) {
    qDebug("Opacity value changed");

    QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        if (list[index]->IsVisible()) {
            list[index]->SetOpacity(value);
            updateDrawables();
        }
    }
}

void QtMainWindow::listViewDoubleClicked(QModelIndex index) {
    qDebug("List view double clicked");
    currentImageIndex = index.row();
    toggleImageVisible();
}

void QtMainWindow::listViewClicked(QModelIndex index) {
    int i = index.row();
    QList<irtkQtImageObject*> list = irtkQtConfiguration::Instance()->GetImageObjectList();
    irtkQtImageObject* imageObject = list[i];

    visualToolWidget->blockSignals(true);

    if (imageObject->IsVisible()) {
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
}

void QtMainWindow::listViewShowContextMenu(const QPoint &pos) {
    if (imageModel) {
        QMenu imageMenu;
        QPoint globalPos = imageListView->mapToGlobal(pos);

        int selectedCount = imageListView->selectionModel()->selectedIndexes().size();

        if ( selectedCount < 1 ) {
            return;
        }
        if ( selectedCount == 1) {
            currentImageIndex = imageListView->indexAt(pos).row();
            imageMenu.addAction(toggleVisibleAction);
            if (irtkQtConfiguration::Instance()->GetImageObjectList().at(currentImageIndex)->IsVisible()) {
                toggleVisibleAction->setChecked(true);
            }
            else {
                toggleVisibleAction->setChecked(false);
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

    QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index > 0) {
        list.move(index, index-1);

        delete imageModel;
        imageModel = new irtkImageListModel(list);
        imageListView->setModel(imageModel);

        imageListView->setCurrentIndex(imageModel->index(index-1, 0));

        for (int i = 0; i < viewers.size(); i++) {
            viewers[i]->MoveImage(index, index-1);
        }

        setUpViewerWidgets();
        updateDrawables();
    }
}

void QtMainWindow::moveImageDown() {
    qDebug("Moving image down");

    QList<irtkQtImageObject*> & list = irtkQtConfiguration::Instance()->GetImageObjectList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0 && (index < list.size() - 1)) {
        list.move(index, index+1);

        delete imageModel;
        imageModel = new irtkImageListModel(list);
        imageListView->setModel(imageModel);

        imageListView->setCurrentIndex(imageModel->index(index+1, 0));

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

void QtMainWindow::saveConfigutationFile() {
    // Save image in the specified file
    QString fileName = QFileDialog::getSaveFileName(
                this,tr("Save screenshot as"),
                "", tr("XML (*.xml)"));

    createConfigurationViewerList();
    irtkQtConfiguration::Instance()->Write(fileName);
}
