#include <QtMainWindow.h>

#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QFileInfo>

#include <QGridLayout>

QtMainWindow::QtMainWindow() {
    splitter = new QSplitter(this);

    listWidget = new QListWidget(splitter);
    listWidget->setMaximumWidth(0.5*width());

    mainViewWidget = new QWidget(splitter);
    QGridLayout *layout = new QGridLayout();
    mainViewWidget->setLayout(layout);

    createActions();
    createMenu();
    createToolBar();

    setCentralWidget(splitter);
    singleViewerInScreen = false;
}

QtMainWindow::~QtMainWindow() {
    clearVectors();
    irtkQtViewer::Destroy();
}

void QtMainWindow::createToolBar() {
    toolbar = addToolBar(tr("View"));
    toolbar->addAction(viewSelectedImageAction);
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
    viewMenu->addAction(clearViewsAction);
}

void QtMainWindow::createActions() {
    openTargetAction = new QAction(tr("&Open image file(s)..."), this);
    openTargetAction->setStatusTip(tr("Load new image file(s)"));
    connect(openTargetAction, SIGNAL(triggered()), this, SLOT(openImage()));

    viewAxialAction = new QAction(tr("Axial View"), this);
    viewAxialAction->setStatusTip(tr("Add axial view"));
    connect(viewAxialAction, SIGNAL(triggered()), this, SLOT(createAxialView()));

    viewCoronalAction = new QAction(tr("Coronal View"), this);
    viewCoronalAction->setStatusTip(tr("Add coronal view"));
    connect(viewCoronalAction, SIGNAL(triggered()), this, SLOT(createCoronalView()));

    viewSagittalAction = new QAction(tr("Sagittal View"), this);
    viewSagittalAction->setStatusTip(tr("Add sagittal view"));
    connect(viewSagittalAction, SIGNAL(triggered()), this, SLOT(createSagittalView()));

    viewOrthogonalAction = new QAction(tr("Orthogonal View"), this);
    viewOrthogonalAction->setStatusTip(tr("Add orthogonal view"));
    connect(viewOrthogonalAction, SIGNAL(triggered()), this, SLOT(createOrthogonalView()));

    clearViewsAction = new QAction(tr("Clear views"), this);
    clearViewsAction->setStatusTip(tr("Delete all views"));
    connect(clearViewsAction, SIGNAL(triggered()), this, SLOT(clearViews()));

    viewSelectedImageAction = new QAction(tr("View image"), this);
    viewSelectedImageAction->setIcon(QIcon(":/icons/brain.png"));
    viewSelectedImageAction->setStatusTip(tr("View selected image"));
    connect(viewSelectedImageAction, SIGNAL(triggered()), this, SLOT(viewImage()));

    zoomInAction = new QAction(tr("Zoom in"), this);
    zoomInAction->setIcon(QIcon(":/icons/zoom_in.png"));
    zoomInAction->setEnabled(false);
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAction = new QAction(tr("Zoom out"), this);
    zoomOutAction->setIcon(QIcon(":/icons/zoom_out.png"));
    zoomOutAction->setEnabled(false);
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
}

void QtMainWindow::disconnectSignals() {
    QtViewerWidget *viewerWidget;
    irtkQtTwoDimensionalViewer *viewer;

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets.at(i);
        viewer = viewers.at(i);

        disconnect(viewerWidget->getGlWidget(), SIGNAL(resized(int, int)),
                   viewer, SLOT(ResizeImage(int, int)));
        disconnect(viewer, SIGNAL(ImageResized(QRgb*)),
                   viewerWidget->getGlWidget(), SLOT(updateDrawable(QRgb*)));

        disconnect(viewerWidget->getSlider(), SIGNAL(valueChanged(int)),
                   viewer, SLOT(ChangeSlice(int)));
        disconnect(viewerWidget->getGlWidget(), SIGNAL(leftButtonPressed(int, int)),
                   viewer, SLOT(ChangeOrigin(int, int)));

        disconnect(viewers.at(i), SIGNAL(OriginChanged(double, double, double)),
                   this, SLOT(updateOrigin(double, double, double)));
    }
}

void QtMainWindow::connectSignals() {
    QtViewerWidget *viewerWidget;
    irtkQtTwoDimensionalViewer *viewer;

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets.at(i);
        viewer = viewers.at(i);

        /// update drawable when widgets are resized
        connect(viewerWidget->getGlWidget(), SIGNAL(resized(int, int)),
                viewer, SLOT(ResizeImage(int, int)));
        connect(viewer, SIGNAL(ImageResized(QRgb*)),
                viewerWidget->getGlWidget(), SLOT(updateDrawable(QRgb*)));

        /// update drawable when slice is changed
        connect(viewerWidget->getSlider(), SIGNAL(valueChanged(int)),
                viewer, SLOT(ChangeSlice(int)));
        connect(viewerWidget->getGlWidget(), SIGNAL(leftButtonPressed(int, int)),
                           viewer, SLOT(ChangeOrigin(int, int)));

        connect(viewer, SIGNAL(OriginChanged(double, double, double)),
                this, SLOT(updateOrigin(double, double, double)));
    }
}

void QtMainWindow::showTargetImage(int j) {
    if ( viewers.size() == 0 ) {
        createMessageBox("You need to add viewers first.", QMessageBox::Warning);
        return;
    }

    zoomInAction->setEnabled(true);
    zoomOutAction->setEnabled(true);
    QtTwoDimensionalGlWidget *glWidget;

    disconnectSignals();

    QtViewerWidget *viewerWidget;
    irtkQtTwoDimensionalViewer *viewer;

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets.at(i);
        viewer = viewers.at(i);

        glWidget = viewerWidget->getGlWidget();

        viewer->SetTarget(irtkQtViewer::Instance()->GetImage(j));
        viewer->SetDimensions(glWidget->customWidth(), glWidget->customHeight());
        viewer->InitializeTransformation();
        viewer->InitializeOutputImage();

        glWidget->setEnabled(true);
        viewerWidget->getSlider()->setEnabled(true);
        viewerWidget->setMaximumSlice(viewer->GetSliceNumber());
        viewerWidget->setCurrentSlice(viewer->GetCurrentSlice());

        glWidget->updateDrawable(viewer->GetDrawable());
    }

    connectSignals();
}

QtViewerWidget* QtMainWindow::createTwoDimensionalView(irtkViewMode viewMode) {
    irtkQtTwoDimensionalViewer* viewer;
    QtViewerWidget *qtViewer;

    viewer = irtkQtViewer::Instance()->CreateTwoDimensionalViewer(viewMode);
    viewers.push_back(viewer);
    qtViewer = new QtViewerWidget();
    viewerWidgets.push_back(qtViewer);

    char top, bottom, left, right;
    viewer->GetLabels(top, bottom, left, right);
    qtViewer->getGlWidget()->setLabels(top, bottom, left, right);

    connect(qtViewer, SIGNAL(windowExpanded()), this, SLOT(showOnlyThisWidget()));
    connect(qtViewer, SIGNAL(windowDeleted()), this, SLOT(deleteThisWidget()));

    return qtViewer;
}

void QtMainWindow::clearVectors() {
    qDeleteAll(viewers);
    qDeleteAll(viewerWidgets);
    viewers.clear();
    viewerWidgets.clear();
}

bool QtMainWindow::imageInList(const QString fileName) {
    for (int i = 0; i < listWidget->count(); i++) {
        if (listWidget->item(i)->toolTip() == fileName)
            return true;
    }
    return false;
}

void QtMainWindow::addToViewWidget(QWidget *widget) {
    QGridLayout *layout = dynamic_cast<QGridLayout*>(mainViewWidget->layout());

    if ( viewers.size() % 2 == 0 )
        layout->addWidget(widget, layout->rowCount()-1, 1);
    else
        layout->addWidget(widget, layout->rowCount(), 0);
}

void QtMainWindow::createMessageBox(QString message, QMessageBox::Icon icon) {
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setIcon(icon);
    msgBox.exec();
}

void QtMainWindow::openImage() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this);

    QStringList::const_iterator it;
    for (it = fileNames.constBegin(); it != fileNames.constEnd(); it++) {
        if ( !imageInList(*it) ) {
            try {
                irtkQtViewer::Instance()->CreateImage((*it).toStdString());
                QListWidgetItem *newItem = new QListWidgetItem;
                QFileInfo file(*it);
                newItem->setToolTip(*it);
                newItem->setText(file.fileName());
                listWidget->addItem(newItem);
            }
            catch (irtkException e) {
                createMessageBox("Invalid image file " + (*it), QMessageBox::Critical);
            }
        }
    }
}

void QtMainWindow::viewImage() {
    QModelIndexList indexes = listWidget->selectionModel()->selectedIndexes();

    vector<int> indexList;
    foreach(QModelIndex index, indexes) {
        indexList.push_back(index.row());
    }

    if ( indexList.size() == 0 ) {
        createMessageBox("You need to select an image to view.", QMessageBox::Warning);
        return;
    }
    else if ( indexList.size() > 0 ) {
        showTargetImage(indexList.at(0));
    }
}

void QtMainWindow::zoomIn() {
    QtViewerWidget *viewerWidget;
    irtkQtTwoDimensionalViewer *viewer;

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets.at(i);
        viewer = viewers.at(i);

        if (viewerWidget->getGlWidget()->isEnabled()) {
            viewer->IncreaseResolution();
            viewer->InitializeOutputImage();
            viewerWidget->getGlWidget()->updateDrawable(viewer->GetDrawable());
        }
    }
}

void QtMainWindow::zoomOut() {
    QtViewerWidget *viewerWidget;
    irtkQtTwoDimensionalViewer *viewer;

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets.at(i);
        viewer = viewers.at(i);

        if (viewerWidget->getGlWidget()->isEnabled()) {
            viewer->DecreaseResolution();
            viewer->InitializeOutputImage();
            viewerWidget->getGlWidget()->updateDrawable(viewer->GetDrawable());
        }
    }
}

void QtMainWindow::showOnlyThisWidget() {
    if (singleViewerInScreen) {
        for (int i = 0; i < viewerWidgets.size(); i++) {
            viewerWidgets.at(i)->show();
        }
    }
    else {
        QWidget *senderWidget = dynamic_cast<QWidget*>(sender());
        for (int i = 0; i < viewerWidgets.size(); i++) {
            if (viewerWidgets.at(i) != senderWidget)
                viewerWidgets.at(i)->hide();
        }
    }
    singleViewerInScreen = !singleViewerInScreen;
}

void QtMainWindow::deleteThisWidget() {
    QWidget *senderWidget = dynamic_cast<QWidget*>(sender());

    int i = 0;
    while ( senderWidget != viewerWidgets.at(i) ) {
        i++;
    }
    viewerWidgets.at(i)->hide();
    viewerWidgets.remove(i);
    viewers.remove(i);
}

void QtMainWindow::createAxialView() {
    addToViewWidget(createTwoDimensionalView(VIEW_AXIAL));
}

void QtMainWindow::createCoronalView() {
    addToViewWidget(createTwoDimensionalView(VIEW_CORONAL));
}

void QtMainWindow::createSagittalView() {
    addToViewWidget(createTwoDimensionalView(VIEW_SAGITTAL));
}

void QtMainWindow::createOrthogonalView() {
    addToViewWidget(createTwoDimensionalView(VIEW_AXIAL));
    addToViewWidget(createTwoDimensionalView(VIEW_CORONAL));
    addToViewWidget(createTwoDimensionalView(VIEW_SAGITTAL));
}

void QtMainWindow::clearViews() {
    clearVectors();
}

void QtMainWindow::updateOrigin(double x, double y, double z) {
    QtTwoDimensionalGlWidget *glWidget;
    irtkQtTwoDimensionalViewer *senderViewer, *viewer;
    QtViewerWidget *viewerWidget;

    senderViewer = dynamic_cast<irtkQtTwoDimensionalViewer*>(sender());
    irtkViewMode vmode = senderViewer->GetViewMode();

    disconnectSignals();

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets.at(i);
        viewer = viewers.at(i);
        glWidget = viewerWidget->getGlWidget();

        if ( glWidget->isEnabled() &&
             ( (viewer->GetViewMode() != vmode) || (viewer == senderViewer) ) ) {
                viewer->SetOrigin(x, y, z);
                viewer->InitializeOutputImage();

                viewerWidget->setCurrentSlice(viewer->GetCurrentSlice());
                glWidget->updateDrawable(viewer->GetDrawable());
        }
    }

    connectSignals();
}
