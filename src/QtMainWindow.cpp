#include <QtMainWindow.h>

#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>

#include <QGridLayout>

QtMainWindow::QtMainWindow() {
    splitter = new QSplitter(this);

    listWidget = new QListWidget(splitter);
    mainViewWidget = new QWidget(splitter);
    QGridLayout *layout = new QGridLayout();
    mainViewWidget->setLayout(layout);

    createActions();
    createMenu();
    createToolBar();

    setCentralWidget(splitter);
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
    for (int i = 0; i < viewers.size(); i++) {
        disconnect(viewerWidgets.at(i)->getGlWidget(), SIGNAL(resized(int, int)),
                viewers.at(i), SLOT(ResizeImage(int, int)));
        disconnect(viewers.at(i), SIGNAL(ImageResized(irtkColor*)),
                viewerWidgets.at(i)->getGlWidget(), SLOT(updateDrawable(irtkColor*)));

        disconnect(viewerWidgets.at(i)->getSlider(), SIGNAL(valueChanged(int)),
                viewers.at(i), SLOT(ChangeSlice(int)));
        disconnect(viewerWidgets.at(i)->getGlWidget(), SIGNAL(leftButtonPressed(int, int)),
                   viewers.at(i), SLOT(ChangeOrigin(int, int)));
        disconnect(viewers.at(i), SIGNAL(OriginChanged(double, double, double, int)),
                this, SLOT(updateOrigin(double, double, double, int)));
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
        connect(viewers.at(i), SIGNAL(OriginChanged(double, double, double, int)),
                this, SLOT(updateOrigin(double, double, double, int)));
    }
}

void QtMainWindow::showTargetImage(int j) {
    if ( viewers.size() == 0 ) {
        createMessageBox("You need to add viewers first.", QMessageBox::Warning);
        return;
    }

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
    viewer->SetId(viewers.size());
    viewers.push_back(viewer);
    qtViewer = new QtViewerWidget();
    viewerWidgets.push_back(qtViewer);

    char top, bottom, left, right;
    viewer->GetLabels(top, bottom, left, right);
    qtViewer->getGlWidget()->setLabels(top, bottom, left, right);

    return qtViewer;
}

void QtMainWindow::clearVectors() {
    qDeleteAll(viewers);
    qDeleteAll(mainViewWidget->children());
}

bool QtMainWindow::imageInList(const QString fileName) {
    for (int i = 0; i < listWidget->count(); i++) {
        if (listWidget->item(i)->text() == fileName)
            return true;
    }
    return false;
}

void QtMainWindow::addToViewWidget(QWidget *widget) {
    QGridLayout *layout = dynamic_cast<QGridLayout*>(mainViewWidget->layout());

    if ( viewers.size() % 2 == 0 ) {
        layout->addWidget(widget, layout->rowCount()-1, 1);
    }
    else {
        layout->addWidget(widget, layout->rowCount(), 0);
    }
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
                newItem->setText(*it);
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
    foreach(QModelIndex index, indexes)
    {
        indexList.push_back(index.row());
    }

    if ( indexList.size() > 0 ) {
        showTargetImage(indexList.at(0));
        zoomInAction->setEnabled(true);
        zoomOutAction->setEnabled(true);
    }
    else
        createMessageBox("You need to select an image to view.", QMessageBox::Warning);
}

void QtMainWindow::zoomIn() {
    for (int i = 0; i < viewers.size(); i++) {
        if (viewerWidgets.at(i)->getGlWidget()->isEnabled()) {
            viewers.at(i)->IncreaseResolution();
            viewers.at(i)->InitializeOutputImage();
            viewerWidgets.at(i)->getGlWidget()->updateDrawable(viewers.at(i)->GetDrawable());
        }
    }
}

void QtMainWindow::zoomOut() {
    for (int i = 0; i < viewers.size(); i++) {
        if (viewerWidgets.at(i)->getGlWidget()->isEnabled()) {
            viewers.at(i)->DecreaseResolution();
            viewers.at(i)->InitializeOutputImage();
            viewerWidgets.at(i)->getGlWidget()->updateDrawable(viewers.at(i)->GetDrawable());
        }
    }
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

void QtMainWindow::updateOrigin(double x, double y, double z, int id) {
    QtTwoDimensionalGlWidget *glWidget;
    irtkViewMode vmode = viewers.at(id)->GetViewMode();

    disconnectSignals();

    for (int i = 0; i < viewers.size(); i++) {
        if ( viewerWidgets.at(i)->getGlWidget()->isEnabled() &&
             ( (viewers.at(i)->GetViewMode() != vmode) || (i == id) ) ) {
                glWidget = viewerWidgets.at(i)->getGlWidget();
                viewers.at(i)->SetOrigin(x, y, z);
                viewers.at(i)->InitializeOutputImage();

                viewerWidgets.at(i)->setCurrentSlice(viewers.at(i)->GetCurrentSlice());
                glWidget->updateDrawable(viewers.at(i)->GetDrawable());
        }
    }

    connectSignals();
}
