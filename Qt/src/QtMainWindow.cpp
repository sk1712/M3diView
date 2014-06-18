#include <QtMainWindow.h>

#include <QDockWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QDir>

#include <QGridLayout>
#include <QVBoxLayout>


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
}

QtMainWindow::~QtMainWindow() {
    clearLists();
    irtkQtViewer::Destroy();
}

void QtMainWindow::createDockWindows() {
    // Create image list dock widget
    QDockWidget *dock = new QDockWidget(tr("Image list"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    imageListView = new QListView(dock);
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
    irtkQtLookupTable::SetColorModeList();
    visualToolWidget = new QtToolWidget;
    visualToolWidget->setEnabled(false);
    visualToolWidget->fillColorCombo(irtkQtLookupTable::GetColorModeList());
    toolsTabWidget->addTab(visualToolWidget, tr("Visualisation"));
}

void QtMainWindow::createToolBar() {
    toolbar = addToolBar(tr("View"));

    toolbar->addAction(moveUpAction);
    toolbar->addAction(moveDownAction);
    toolbar->addSeparator();

    toolbar->addAction(zoomInAction);
    toolbar->addAction(zoomOutAction);
}

void QtMainWindow::createMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openTargetAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(viewOrthogonalAction);
    viewMenu->addAction(viewAxialAction);
    viewMenu->addAction(viewCoronalAction);
    viewMenu->addAction(viewSagittalAction);

    viewMenu->addSeparator();
    viewMenu->addAction(view3DAction);

    viewMenu->addSeparator();
    viewMenu->addAction(clearViewsAction);
    viewMenu->addSeparator();
}

void QtMainWindow::createToolBarActions() {
    zoomInAction = new QAction(tr("Zoom in"), this);
    zoomInAction->setIcon(QIcon(":/icons/zoom_in.png"));

    zoomOutAction = new QAction(tr("Zoom out"), this);
    zoomOutAction->setIcon(QIcon(":/icons/zoom_out.png"));

    moveUpAction = new QAction(tr("Move image up"), this);
    moveUpAction->setIcon(QIcon(":/icons/arrow_up.png"));

    moveDownAction = new QAction(tr("Move image down"), this);
    moveDownAction->setIcon(QIcon(":/icons/arrow_down.png"));
}

void QtMainWindow::createMenuActions() {
    openTargetAction = new QAction(tr("&Open image file(s)..."), this);
    openTargetAction->setStatusTip(tr("Load new image file(s)"));

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

    clearViewsAction = new QAction(tr("Clear all"), this);
    clearViewsAction->setStatusTip(tr("Delete all views"));
}

void QtMainWindow::createImageMenuActions() {
    deleteImageAction = new QAction(tr("Delete"), this);
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
    connect(deleteImageAction, SIGNAL(triggered()), this, SLOT(deleteThisImage()));

    // Toolbar signals
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(moveUpAction, SIGNAL(triggered()), this, SLOT(moveImageUp()));
    connect(moveDownAction, SIGNAL(triggered()), this, SLOT(moveImageDown()));

    // Menu signals
    connect(openTargetAction, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(viewAxialAction, SIGNAL(triggered()), this, SLOT(createAxialView()));
    connect(viewCoronalAction, SIGNAL(triggered()), this, SLOT(createCoronalView()));
    connect(viewSagittalAction, SIGNAL(triggered()), this, SLOT(createSagittalView()));
    connect(viewOrthogonalAction, SIGNAL(triggered()), this, SLOT(createOrthogonalView()));
    connect(view3DAction, SIGNAL(triggered()), this, SLOT(create3dView()));
    connect(clearViewsAction, SIGNAL(triggered()), this, SLOT(clearViews()));
}

void QtMainWindow::connectToolSignals() {
    connect(visualToolWidget, SIGNAL(colormapChanged(int)), this, SLOT(colormapIndexChanged(int)));
    connect(visualToolWidget, SIGNAL(opacityChanged(int)), this, SLOT(opacityValueChanged(int)));
    connect(visualToolWidget, SIGNAL(minChanged(double)), this, SLOT(minDisplayValueChanged(double)));
    connect(visualToolWidget, SIGNAL(maxChanged(double)), this, SLOT(maxDisplayValueChanged(double)));
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

    viewer = irtkQtViewer::Instance()->CreateTwoDimensionalViewer(viewMode);
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

    viewImage();

    return qtViewer;
}

Qt3dViewerWidget* QtMainWindow::createThreeDimensionalView() {
    irtkQtThreeDimensionalViewer* viewer;
    Qt3dViewerWidget *qtViewer;

    viewer = irtkQtViewer::Instance()->CreateThreeDimensionalViewer();
    viewers.push_back(viewer);
    qtViewer = new Qt3dViewerWidget();
    viewerWidgets.push_back(qtViewer);

    // Register signals to expand or delete the viewer
    connect(qtViewer, SIGNAL(windowExpanded()), this, SLOT(showOnlyThisWidget()));
    connect(qtViewer, SIGNAL(windowDeleted()), this, SLOT(deleteThisWidget()));

    viewImage();

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
    QList<irtkQtImageObject*> list = irtkQtViewer::Instance()->GetImageList();
    QList<irtkQtImageObject*>::const_iterator it;
    for (it = list.constBegin(); it != list.constEnd(); it++) {
        if ((*it)->GetPath() == fileName)
            // Image has already been loaded
            return true;
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

    irtkQtViewer* instance = irtkQtViewer::Instance();

    // Create an irtkImageObject for each new image
    QStringList::const_iterator it;
    for (it = fileNames.constBegin(); it != fileNames.constEnd(); it++) {
        if ( !imageInList(*it) ) {
            instance->CreateImage((*it));
        }
    }

    // Update the image model
    delete imageModel;
    imageModel = new irtkImageListModel(instance->GetImageList());
    imageListView->setModel(imageModel);
}

bool QtMainWindow::setDisplayedImages() {
    QtViewerWidget *viewerWidget = viewerWidgets[viewerWidgets.size()-1];
    irtkQtBaseViewer *viewer = viewers[viewers.size()-1];
    QtGlWidget *glWidget = viewerWidget->getGlWidget();

    bool atLeastOneImageVisible = false;

    QList<irtkQtImageObject*> imageList = irtkQtViewer::Instance()->GetImageList();
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

    QList<irtkQtImageObject*> & imageList = irtkQtViewer::Instance()->GetImageList();

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
        viewerWidget->setCurrentSlice(viewer->GetCurrentSlice());
        viewerWidget->getGlWidget()->updateDrawable(
                    QVector<QRgb**>::fromStdVector(viewer->GetDrawable()));
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

void QtMainWindow::viewImage() {
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

    // Set up the viewer widgets
    viewerWidget->setEnabled(true);
    viewerWidget->setMaximumSlice(viewer->GetSliceNumber());
    viewerWidget->setCurrentSlice(viewer->GetCurrentSlice());
    viewerWidget->getGlWidget()->updateDrawable(
                QVector<QRgb**>::fromStdVector(viewer->GetDrawable()));

    // Re-register the viewers' signals
    connectViewerSignals();
}

void QtMainWindow::deleteThisImage() {
    QList<irtkQtImageObject*> & list = irtkQtViewer::Instance()->GetImageList();
    bool visible = list[currentImageIndex]->IsVisible();

    if (visible) {
        numDisplayedImages--;

        if (numDisplayedImages == 0) {
            disableViewerWidgets();
        }

        deleteSingleImage(currentImageIndex);
        setUpViewerWidgets();
    }

    // Delete item from the list
    delete list.takeAt(currentImageIndex);

    // Update the map keys of the images currently displayed
    QList<irtkQtBaseViewer*>::iterator it;
    for (it = viewers.begin(); it != viewers.end(); it++) {
        (*it)->UpdateKeysAfterIndexDeleted(currentImageIndex);
    }

    // Update the model
    delete imageModel;
    imageModel = new irtkImageListModel(list);
    imageListView->setModel(imageModel);
}

/// free function used to calculate the output images
/// in parallel for the different viewers
void CalculateOutputImage(irtkQtBaseViewer* viewer) {
    viewer->CalculateOutputImages();
}

void QtMainWindow::zoomIn() {
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
    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = dynamic_cast<Qt2dViewerWidget*>(viewerWidgets[i]);
        viewer = viewers[i];

        if (viewerWidget != 0) {
            if (viewerWidget->getGlWidget()->isEnabled()) {
                viewerWidget->getGlWidget()->updateDrawable(
                            QVector<QRgb**>::fromStdVector(viewer->GetDrawable()));
            }
        }
    }

    delete [] threads;
}

void QtMainWindow::zoomOut() {
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
    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = dynamic_cast<Qt2dViewerWidget*>(viewerWidgets[i]);
        viewer = viewers[i];

        if (viewerWidget != 0) {
            if (viewerWidget->getGlWidget()->isEnabled()) {
                viewerWidget->getGlWidget()->updateDrawable(
                            QVector<QRgb**>::fromStdVector(viewer->GetDrawable()));
            }
        }
    }

    delete [] threads;
}

void QtMainWindow::showOnlyThisWidget() {
    singleViewerInScreen = !singleViewerInScreen;

    if (!singleViewerInScreen) {
        for (int i = 0; i < viewerWidgets.size(); i++) {
            viewerWidgets[i]->show();
        }
    }
    else {
        QWidget *senderWidget = dynamic_cast<QWidget*>(sender());
        for (int i = 0; i < viewerWidgets.size(); i++) {
            if (viewerWidgets[i] != senderWidget)
                viewerWidgets[i]->hide();
        }
    }
}

void QtMainWindow::deleteThisWidget() {
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
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_AXIAL));
}

void QtMainWindow::createCoronalView() {
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_CORONAL));
}

void QtMainWindow::createSagittalView() {
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_SAGITTAL));
}

void QtMainWindow::createOrthogonalView() {
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_AXIAL));
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_CORONAL));
    addToViewWidget(createTwoDimensionalView(irtkQtBaseViewer::VIEW_SAGITTAL));
}

void QtMainWindow::create3dView() {
    addToViewWidget(createThreeDimensionalView());
}

void QtMainWindow::clearViews() {
    clearLists();
    singleViewerInScreen = false;
}

void QtMainWindow::updateOrigin(double x, double y, double z) {
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
        }
    }

    connectViewerSignals();

    delete [] threads;
}

void QtMainWindow::minDisplayValueChanged(double value) {
    QList<irtkQtImageObject*> & list = irtkQtViewer::Instance()->GetImageList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        if (list[index]->IsVisible()) {
            list[index]->SetMinDisplayValue(value);
            updateDrawables();
        }
    }
}

void QtMainWindow::maxDisplayValueChanged(double value) {
    QList<irtkQtImageObject*> & list = irtkQtViewer::Instance()->GetImageList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        if (list[index]->IsVisible()) {
            list[index]->SetMaxDisplayValue(value);
            updateDrawables();
        }
    }
}

void QtMainWindow::colormapIndexChanged(int mode) {
    QList<irtkQtImageObject*> & list = irtkQtViewer::Instance()->GetImageList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        if (list[index]->IsVisible()) {
            list[index]->SetColormap(static_cast<irtkQtLookupTable::irtkColorMode>(mode));
            updateDrawables();
        }
    }
}

void QtMainWindow::opacityValueChanged(int value) {
    QList<irtkQtImageObject*> & list = irtkQtViewer::Instance()->GetImageList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        if (list[index]->IsVisible()) {
            list[index]->SetOpacity(value);
            updateDrawables();
        }
    }
}

void QtMainWindow::listViewDoubleClicked(QModelIndex index) {
    int i = index.row();
    QList<irtkQtImageObject*> & list = irtkQtViewer::Instance()->GetImageList();
    bool visible = !list[i]->IsVisible();
    list[i]->SetVisible(visible);

    // If image becomes visible display it
    if (visible) {
        try {
            list[i]->CreateImage();
            displaySingleImage(i);
            numDisplayedImages++;
            setUpViewerWidgets();
        }
        catch (irtkException) {
            createMessageBox("Invalid image file " + list[i]->GetPath(), QMessageBox::Critical);
            list.removeAt(i);
            QList<irtkQtBaseViewer*>::iterator it;
            for (it = viewers.begin(); it != viewers.end(); it++) {
                (*it)->UpdateKeysAfterIndexDeleted(i);
            }
        }
    }
    // Otherwise delete it and decrease the number of visible images
    else {
        numDisplayedImages--;

        if (numDisplayedImages == 0) {
            disableViewerWidgets();
        }

        list[i]->DeleteImage();
        deleteSingleImage(i);
        setUpViewerWidgets();
    }

    delete imageModel;
    imageModel = new irtkImageListModel(list);
    imageListView->setModel(imageModel);
}

void QtMainWindow::listViewClicked(QModelIndex index) {
    int i = index.row();
    QList<irtkQtImageObject*> list = irtkQtViewer::Instance()->GetImageList();
    irtkQtImageObject* imageObject = list[i];

    if (imageObject->IsVisible()) {
        visualToolWidget->setEnabled(true);
        visualToolWidget->setColormap(imageObject->GetColormap());
        visualToolWidget->setOpacity(imageObject->GetOpacity());
        visualToolWidget->setMinimumImageValue(imageObject->GetMinImageValue());
        visualToolWidget->setMaximumImageValue(imageObject->GetMaxImageValue());
        visualToolWidget->setDisplayMax(imageObject->GetMaxDisplayValue());
        visualToolWidget->setDisplayMin(imageObject->GetMinDisplayValue());
    }
    else {
        visualToolWidget->setEnabled(false);
    }

    infoWidget->setImage(imageObject->GetImage());
    infoWidget->update();
}

void QtMainWindow::listViewShowContextMenu(const QPoint &pos) {
    QMenu imageMenu;
    imageMenu.addAction(deleteImageAction);

    QPoint globalPos = imageListView->mapToGlobal(pos);
    currentImageIndex = imageListView->indexAt(pos).row();

    // Show the menu only if the current image index is valid
    if (currentImageIndex >= 0 && currentImageIndex < imageModel->rowCount()) {
        imageMenu.exec( globalPos );
    }
}

void QtMainWindow::moveImageUp() {
    QList<irtkQtImageObject*> & list = irtkQtViewer::Instance()->GetImageList();
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
    }
}

void QtMainWindow::moveImageDown() {
    QList<irtkQtImageObject*> & list = irtkQtViewer::Instance()->GetImageList();
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
    }
}
