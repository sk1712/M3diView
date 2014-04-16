#include <QtMainWindow.h>

#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QDir>

#include <QGridLayout>
#include <QVBoxLayout>

QtMainWindow::QtMainWindow() {
    splitter = new QSplitter(this);

    imageListView = new QListView(splitter);
    imageListView->setMaximumWidth(0.5*width());
    connect(imageListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(listViewClicked(QModelIndex)));
    connect(imageListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(listViewDoubleClicked(QModelIndex)));

    mainViewWidget = new QWidget(splitter);
    QGridLayout *layout = new QGridLayout();
    mainViewWidget->setLayout(layout);

    createMenuActions();
    createToolBarActions();
    createMenu();
    createToolBar();

    setCentralWidget(splitter);
    singleViewerInScreen = false;
    imageModel = NULL;
}

QtMainWindow::~QtMainWindow() {
    clearVectors();
    irtkQtViewer::Destroy();
}

void QtMainWindow::createToolBar() {
    toolbar = addToolBar(tr("View"));

    toolbar->addAction(viewSelectedImageAction);
    toolbar->addAction(moveUpAction);
    toolbar->addAction(moveDownAction);
    toolbar->addSeparator();

    toolbar->addAction(zoomInAction);
    toolbar->addAction(zoomOutAction);
    toolbar->addSeparator();

    toolbar->addAction(opacityAction);
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
    viewMenu->addAction(clearViewsAction);
}

void QtMainWindow::createToolBarActions() {
    viewSelectedImageAction = new QAction(tr("View image"), this);
    viewSelectedImageAction->setIcon(QIcon(":/icons/view_image.png"));
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

    QMenu *opacityMenu = new QMenu();
    QWidgetAction *widgetAction = new QWidgetAction(opacityMenu);

    QWidget *opacityWidget = new QWidget();
    QVBoxLayout *menuLayout = new QVBoxLayout();
    opacityWidget->setLayout(menuLayout);
    opacitySlider = new QSlider(Qt::Vertical);
    opacitySlider->setRange(0, 255);
    menuLayout->addWidget(opacitySlider);

    opacityLabel = new QLabel();
    opacityLabel->setFixedWidth(30);
    menuLayout->addWidget(opacityLabel);

    widgetAction->setDefaultWidget(opacityWidget);
    opacityMenu->addAction(widgetAction);
    connect(opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(opacityValueChanged(int)));
    opacitySlider->setValue(255);

    opacityAction = new QAction(tr("Opacity"), this);
    opacityAction->setIcon(QIcon(":/icons/opacity.png"));
    opacityAction->setMenu(opacityMenu);

    moveUpAction = new QAction(tr("Move image up"), this);
    moveUpAction->setIcon(QIcon(":/icons/arrow_up.png"));
    connect(moveUpAction, SIGNAL(triggered()), this, SLOT(moveImageUp()));

    moveDownAction = new QAction(tr("Move image down"), this);
    moveDownAction->setIcon(QIcon(":/icons/arrow_down.png"));
    connect(moveDownAction, SIGNAL(triggered()), this, SLOT(moveImageDown()));
}

void QtMainWindow::createMenuActions() {
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
}

void QtMainWindow::disconnectSignals() {
    QtViewerWidget *viewerWidget;
    irtkQtTwoDimensionalViewer *viewer;

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets.at(i);
        viewer = viewers.at(i);

        disconnect(viewerWidget->getGlWidget(), SIGNAL(resized(int, int)),
                   viewer, SLOT(ResizeImage(int, int)));
        disconnect(viewer, SIGNAL(ImageResized(QVector<QRgb*>)),
                   viewerWidget->getGlWidget(), SLOT(updateDrawable(QVector<QRgb*>)));

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
        connect(viewer, SIGNAL(ImageResized(QVector<QRgb*>)),
                viewerWidget->getGlWidget(), SLOT(updateDrawable(QVector<QRgb*>)));

        /// update drawable when slice is changed
        connect(viewerWidget->getSlider(), SIGNAL(valueChanged(int)),
                viewer, SLOT(ChangeSlice(int)));
        connect(viewerWidget->getGlWidget(), SIGNAL(leftButtonPressed(int, int)),
                           viewer, SLOT(ChangeOrigin(int, int)));

        connect(viewer, SIGNAL(OriginChanged(double, double, double)),
                this, SLOT(updateOrigin(double, double, double)));
    }
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
    QList<irtkQtImageObject*> list = irtkQtViewer::Instance()->GetImageList();
    QList<irtkQtImageObject*>::const_iterator it;
    for (it = list.constBegin(); it != list.constEnd(); it++) {
        if ((*it)->GetPath() == fileName)
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

    if (singleViewerInScreen)
        widget->hide();
}

void QtMainWindow::addToViewWidget(QWidget *widget, int index) {
    QGridLayout *layout = dynamic_cast<QGridLayout*>(mainViewWidget->layout());

    if ( index % 2 != 0 )
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
    QString selfilter = tr("IMG (*.gipl *.gipl.z *.hdr *.hdr.gz *.nii *.nii.gz)");
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                          tr("Open File"),
                                                          QDir::homePath(),
                                                          tr("All files (*.*);;IMG (*.gipl *.gipl.z *.hdr *.hdr.gz *.nii *.nii.gz)" ),
                                                          &selfilter);

    QStringList::const_iterator it;
    for (it = fileNames.constBegin(); it != fileNames.constEnd(); it++) {
        if ( !imageInList(*it) ) {
            irtkQtViewer* instance = irtkQtViewer::Instance();
            instance->CreateImage((*it));
            delete imageModel;
            imageModel = new irtkImageListModel(instance->GetImageList());
            imageListView->setModel(imageModel);
        }
    }
}

void InitializeViewers(irtkQtTwoDimensionalViewer* &viewer) {
    viewer->InitializeTransformation();
    viewer->InitializeOutputImage();
}

void QtMainWindow::viewImage() {
    if ( viewers.size() == 0 ) {
        createMessageBox("You need to add viewers first.", QMessageBox::Warning);
        return;
    }

    disconnectSignals();

    QtViewerWidget *viewerWidget;
    irtkQtTwoDimensionalViewer *viewer;
    QtTwoDimensionalGlWidget *glWidget;

    zoomInAction->setEnabled(true);
    zoomOutAction->setEnabled(true);

    QList<irtkQtImageObject*> imageList = irtkQtViewer::Instance()->GetImageList();
    QList<irtkQtImageObject*>::const_iterator it;

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets[i];
        viewer = viewers[i];
        glWidget = viewerWidget->getGlWidget();

        viewer->ClearDisplayedImages();
        for (it = imageList.constBegin(); it != imageList.constEnd(); it++) {
            if ((*it)->IsVisible()) {
                try {
                    viewer->AddToDisplayedImages(*it);
                }
                catch (irtkException e) {
                    createMessageBox("Invalid image file " + (*it)->GetPath(), QMessageBox::Critical);
                }
            }
        }

        viewer->SetDimensions(glWidget->customWidth(), glWidget->customHeight());
    }

    QtConcurrent::blockingMap(viewers, &InitializeViewers);

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets[i];
        viewer = viewers[i];
        glWidget = viewerWidget->getGlWidget();

        glWidget->setEnabled(true);
        viewerWidget->getSlider()->setEnabled(true);
        viewerWidget->setMaximumSlice(viewer->GetSliceNumber());
        viewerWidget->setCurrentSlice(viewer->GetCurrentSlice());

        vector<QRgb*> drawables = viewer->GetDrawable();
        glWidget->updateDrawable(QVector<QRgb*>::fromStdVector(drawables));
    }

    connectSignals();
}

void QtMainWindow::zoomIn() {
    QtViewerWidget *viewerWidget;
    irtkQtTwoDimensionalViewer *viewer;

    for (int i = 0; i < viewers.size(); i++) {
        viewerWidget = viewerWidgets[i];
        viewer = viewers[i];

        if (viewerWidget->getGlWidget()->isEnabled()) {
            viewer->IncreaseResolution();
            viewer->InitializeOutputImage();
            viewerWidget->getGlWidget()->updateDrawable(
                        QVector<QRgb*>::fromStdVector(viewer->GetDrawable()));
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
            viewerWidget->getGlWidget()->updateDrawable(
                        QVector<QRgb*>::fromStdVector(viewer->GetDrawable()));
        }
    }
}

void QtMainWindow::showOnlyThisWidget() {
    if (singleViewerInScreen) {
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
    singleViewerInScreen = !singleViewerInScreen;
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

void InitializeImage(irtkQtTwoDimensionalViewer* &viewer) {
    viewer->InitializeOutputImage();
}

void QtMainWindow::updateOrigin(double x, double y, double z) {
    clock_t t, t2;

    irtkQtTwoDimensionalViewer *senderViewer;
    senderViewer = dynamic_cast<irtkQtTwoDimensionalViewer*>(sender());

    disconnectSignals();
    t = clock();

    int index = 0;
    while (viewers[index] != senderViewer)
        index++;
    bool isSenderLinked = viewerWidgets[index]->isLinked();

    for (int i = 0; i < viewers.size(); i++) {
        if ( viewerWidgets[i]->getGlWidget()->isEnabled() &&
             ( (isSenderLinked && viewerWidgets[i]->isLinked()) || (viewers[i] == senderViewer) ) ) {
            viewers[i]->SetOrigin(x, y, z);
            viewers[i]->InitializeOutputImage();
        }
    }

    //QtConcurrent::blockingMap(viewers, &InitializeImage);

    for (int i = 0; i < viewers.size(); i++) {
        if ( viewerWidgets[i]->getGlWidget()->isEnabled() &&
             ( (isSenderLinked && viewerWidgets[i]->isLinked()) || (viewers[i] == senderViewer) ) ) {
            viewerWidgets[i]->setCurrentSlice(viewers[i]->GetCurrentSlice());
            viewerWidgets[i]->getGlWidget()->updateDrawable(
                        QVector<QRgb*>::fromStdVector(viewers[i]->GetDrawable()));
        }
    }

    t2 = clock() - t;
    qDebug() << "Main thread: " << QThread::currentThreadId()
             << "It took me " << ((float)t2)/CLOCKS_PER_SEC << " seconds";

    connectSignals();
}

void QtMainWindow::opacityValueChanged(int value) {
    QList<irtkQtImageObject*> list = irtkQtViewer::Instance()->GetImageList();
    int index = imageListView->currentIndex().row();
    if (!list.empty() && index >= 0) {
        list.at(index)->SetOpacity(value);
    }
    opacityLabel->setText(QString::number(value));
}

void QtMainWindow::listViewClicked(QModelIndex index) {
    QList<irtkQtImageObject*> list = irtkQtViewer::Instance()->GetImageList();
    opacitySlider->setValue(list.at(index.row())->GetOpacity());
}

void QtMainWindow::listViewDoubleClicked(QModelIndex index) {
    int i = index.row();
    QList<irtkQtImageObject*> list = irtkQtViewer::Instance()->GetImageList();
    bool visible = list.at(i)->IsVisible();
    list.at(i)->SetVisible(!visible);

    delete imageModel;
    imageModel = new irtkImageListModel(list);
    imageListView->setModel(imageModel);
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
    }
}
