#include <QtMainWindow.h>

#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>

#include <QGridLayout>

QtMainWindow::QtMainWindow() {
//    mainViewWidget = NULL;
//    clearVectors();

    splitter = new QSplitter(this);

    listWidget = new QListWidget(splitter);
    mainViewWidget = new QWidget(splitter);

    createOrthogonalView();
    createActions();
    createMenu();
    createToolBar();

    setCentralWidget(splitter);
}

QtMainWindow::~QtMainWindow() {
    //clearVectors();
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
}

void QtMainWindow::createActions() {
    openTargetAction = new QAction(tr("&Load image..."), this);
    openTargetAction->setStatusTip(tr("Load new image file"));
    connect(openTargetAction, SIGNAL(triggered()), this, SLOT(openImage()));

    viewOrthogonalAction = new QAction(tr("&Orthogonal View"), this);
    viewOrthogonalAction->setStatusTip(tr("Get orthogonal view"));
    connect(viewOrthogonalAction, SIGNAL(triggered()), this, SLOT(createOrthogonalView()));

    viewSelectedImageAction = new QAction(tr("View image"), this);
    viewSelectedImageAction->setStatusTip(tr("View selected image"));
    connect(viewSelectedImageAction, SIGNAL(triggered()), this, SLOT(viewImage()));

    zoomInAction = new QAction(tr("Zoom in"), this);
    zoomInAction->setEnabled(false);
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAction = new QAction(tr("Zoom out"), this);
    zoomOutAction->setEnabled(false);
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
}

void QtMainWindow::disconnectSignals() {
    for (int i = 0; i < viewers.size(); i++) {
        disconnect(viewerWidgets.at(i)->getGlWidget(), SIGNAL(resized(int, int)),
                viewers.at(i), SLOT(ResizeImage(int, int)));
        disconnect(viewers.at(i), SIGNAL(ImageResized(irtkColor*)),
                viewerWidgets.at(i)->getGlWidget(), SLOT(updateDrawable(irtkColor*)));

        disconnect(viewerWidgets.at(i)->getSlider(), SIGNAL(valueChanged(int)),
                viewers.at(i), SLOT(ChangeSlice(int)));
        disconnect(viewerWidgets.at(i)->getGlWidget(), SIGNAL(leftButtonPressed(int, int)),
                   viewers.at(i), SLOT(ChangeOrigin(int, int)));
        disconnect(viewers.at(i), SIGNAL(OriginChanged(double, double, double)),
                this, SLOT(updateOrigin(double, double, double)));
    }
}

void QtMainWindow::connectSignals() {
    for (int i = 0; i < viewers.size(); i++) {
        /// update drawable when widgets are resized
        connect(viewerWidgets.at(i)->getGlWidget(), SIGNAL(resized(int, int)),
                viewers.at(i), SLOT(ResizeImage(int, int)));
        connect(viewers.at(i), SIGNAL(ImageResized(irtkColor*)),
                viewerWidgets.at(i)->getGlWidget(), SLOT(updateDrawable(irtkColor*)));

        /// update drawable when slice is changed
        connect(viewerWidgets.at(i)->getSlider(), SIGNAL(valueChanged(int)),
                viewers.at(i), SLOT(ChangeSlice(int)));
        connect(viewerWidgets.at(i)->getGlWidget(), SIGNAL(leftButtonPressed(int, int)),
                           viewers.at(i), SLOT(ChangeOrigin(int, int)));
        connect(viewers.at(i), SIGNAL(OriginChanged(double, double, double)),
                this, SLOT(updateOrigin(double, double, double)));
    }
}

void QtMainWindow::showTargetImage(int j) {
    QtTwoDimensionalGlWidget *glWidget;

    disconnectSignals();

    for (int i = 0; i < viewers.size(); i++) {
        glWidget = viewerWidgets.at(i)->getGlWidget();

        viewers.at(i)->SetTarget(irtkQtViewer::Instance()->GetImage(j));
        viewers.at(i)->SetDimensions(glWidget->customWidth(), glWidget->customHeight());
        viewers.at(i)->InitializeTransformation();
        viewers.at(i)->InitializeOutputImage();

        glWidget->setEnabled(true);
        viewerWidgets.at(i)->getSlider()->setEnabled(true);
        viewerWidgets.at(i)->setMaximumSlice(viewers.at(i)->GetSliceNumber());
        viewerWidgets.at(i)->setCurrentSlice(viewers.at(i)->GetCurrentSlice());

        glWidget->updateDrawable(viewers.at(i)->GetDrawable());
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

    return qtViewer;
}

void QtMainWindow::clearVectors() {
    qDeleteAll(viewers.begin(), viewers.end());
    qDeleteAll(mainViewWidget->children());
}

bool QtMainWindow::imageInList(const QString fileName) {
    for (int i = 0; i < listWidget->count(); i++) {
        if (listWidget->item(i)->text() == fileName)
            return true;
    }
    return false;
}

void QtMainWindow::openImage() {
    QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty() && !imageInList(fileName)) {
        try {
            irtkQtViewer::Instance()->CreateImage(fileName.toStdString());
            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(fileName);
            listWidget->addItem(newItem);
        }
        catch (irtkException e)
        {
            QMessageBox msgBox;
            msgBox.setText("Invalid image file.");
            msgBox.exec();
        }
    }
}

void QtMainWindow::viewImage() {
    QModelIndexList indexes = listWidget->selectionModel()->selectedIndexes();

    vector<int> indexList;
    foreach(QModelIndex index, indexes)
    {
        indexList.push_back(index.row());
    }

    if ( indexList.size() > 0 )
        showTargetImage(indexList.at(0));

    zoomInAction->setEnabled(true);
    zoomOutAction->setEnabled(true);
}

void QtMainWindow::zoomIn() {
    for (int i = 0; i < viewers.size(); i++) {
        viewers.at(i)->IncreaseResolution();
        viewers.at(i)->InitializeOutputImage();
        viewerWidgets.at(i)->getGlWidget()->updateDrawable(viewers.at(i)->GetDrawable());
    }
}

void QtMainWindow::zoomOut() {
    for (int i = 0; i < viewers.size(); i++) {
        viewers.at(i)->DecreaseResolution();
        viewers.at(i)->InitializeOutputImage();
        viewerWidgets.at(i)->getGlWidget()->updateDrawable(viewers.at(i)->GetDrawable());
    }
}

void QtMainWindow::createOrthogonalView() {
    clearVectors();

    QGridLayout *layout = new QGridLayout();

    layout->addWidget(createTwoDimensionalView(VIEW_AXIAL), 0, 0);
    layout->addWidget(createTwoDimensionalView(VIEW_CORONAL), 0, 1);
        layout->addWidget(createTwoDimensionalView(VIEW_SAGITTAL), 1, 0);

    mainViewWidget->setLayout(layout);
}

void QtMainWindow::updateOrigin(double x, double y, double z) {
    QtTwoDimensionalGlWidget *glWidget;

    disconnectSignals();

    for (int i = 0; i < viewers.size(); i++) {
        glWidget = viewerWidgets.at(i)->getGlWidget();
        viewers.at(i)->SetOrigin(x, y, z);
        viewers.at(i)->InitializeOutputImage();

        viewerWidgets.at(i)->setCurrentSlice(viewers.at(i)->GetCurrentSlice());
        glWidget->updateDrawable(viewers.at(i)->GetDrawable());
    }

    connectSignals();
}
