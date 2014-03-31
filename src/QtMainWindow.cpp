#include <QtMainWindow.h>

#include <QMenuBar>
#include <QFileDialog>
#include <QStatusBar>

#include <QGridLayout>

#include <QResizeEvent>

QtMainWindow::QtMainWindow() {
    clearVectors();

    createActions();
    createMenu();
    createOrthogonalView();
}

QtMainWindow::~QtMainWindow() {
    clearVectors();
    irtkQtViewer::Destroy();
}

void QtMainWindow::createMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openTargetAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(viewOrthogonalAction);
}

void QtMainWindow::createActions() {
    openTargetAction = new QAction(tr("&Open target..."), this);
    openTargetAction->setStatusTip(tr("Open target image file"));
    connect(openTargetAction, SIGNAL(triggered()), this, SLOT(openTargetImage()));

    viewOrthogonalAction = new QAction(tr("&Orthogonal View"), this);
    viewOrthogonalAction->setStatusTip(tr("Get orthogonal view"));
    connect(viewOrthogonalAction, SIGNAL(triggered()), this, SLOT(createOrthogonalView()));
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

void QtMainWindow::showTargetImage() {
    QtTwoDimensionalGlWidget *glWidget;

    disconnectSignals();

    for (int i = 0; i < viewers.size(); i++) {
        glWidget = viewerWidgets.at(i)->getGlWidget();

        viewers.at(i)->SetTarget(irtkQtViewer::Instance()->GetTargetImage());
        viewers.at(i)->SetDimensions(glWidget->width(), glWidget->height());
        viewers.at(i)->InitializeTransformation();
        viewers.at(i)->InitializeOutputImage();

        glWidget->setEnabled(true);
        viewerWidgets.at(i)->getSlider()->setEnabled(true);
        viewerWidgets.at(i)->setMaximumSlice(viewers.at(i)->GetSliceNumber());
        viewerWidgets.at(i)->setCurrentSlice(viewers.at(i)->GetCurrentSlice());

        glWidget->setDrawable(viewers.at(i)->GetDrawable());
        glWidget->updateScene();
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
    qDeleteAll(viewerWidgets.begin(), viewerWidgets.end());
}

void QtMainWindow::openTargetImage() {
    QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty()) {
        irtkQtViewer::Instance()->DestroyTargetImage();
        irtkQtViewer::Instance()->CreateTargetImage(fileName.toStdString());
        // to do check if image file is valid
        showTargetImage();
    }
}

void QtMainWindow::createOrthogonalView() {
    clearVectors();

    QWidget *mainWidget = new QWidget();
    QGridLayout *layout = new QGridLayout();

    layout->addWidget(createTwoDimensionalView(VIEW_AXIAL), 0, 0);
    layout->addWidget(createTwoDimensionalView(VIEW_CORONAL), 0, 1);
        layout->addWidget(createTwoDimensionalView(VIEW_SAGITTAL), 1, 0);

    mainWidget->setLayout(layout);
    this->setCentralWidget(mainWidget);
}

void QtMainWindow::updateOrigin(double x, double y, double z) {
    QtTwoDimensionalGlWidget *glWidget;

    disconnectSignals();

    for (int i = 0; i < viewers.size(); i++) {
        glWidget = viewerWidgets.at(i)->getGlWidget();
        viewers.at(i)->SetOrigin(x, y, z);
        viewers.at(i)->InitializeOutputImage();

        viewerWidgets.at(i)->setCurrentSlice(viewers.at(i)->GetCurrentSlice());
        glWidget->setDrawable(viewers.at(i)->GetDrawable());
        glWidget->updateScene();
    }

    connectSignals();
}
