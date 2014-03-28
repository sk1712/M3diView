#include <QtMainWindow.h>

#include <QMenuBar>
#include <QFileDialog>
#include <QStatusBar>

#include <QGridLayout>

#include <QResizeEvent>

#include <QMessageBox>

QtMainWindow::QtMainWindow() {
    clearVectors();

    createActions();
    createMenu();
    createOrthogonalView();

    flag = false;
}

QtMainWindow::~QtMainWindow() {
    irtkQtViewer::Destroy();
    clearVectors();
}

void QtMainWindow::createMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openTargetAction);
    fileMenu->addAction(openSourceAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(viewOrthogonalAction);
}

void QtMainWindow::createActions() {
    openTargetAction = new QAction(tr("&Open target..."), this);
    openTargetAction->setStatusTip(tr("Open target image file"));
    connect(openTargetAction, SIGNAL(triggered()), this, SLOT(openTargetImage()));

    openSourceAction = new QAction(tr("&Open source..."), this);
    openSourceAction->setStatusTip(tr("Open source image file"));
    connect(openSourceAction, SIGNAL(triggered()), this, SLOT(openSourceImage()));

    viewOrthogonalAction = new QAction(tr("&Orthogonal View"), this);
    viewOrthogonalAction->setStatusTip(tr("Get orthogonal view"));
    connect(viewOrthogonalAction, SIGNAL(triggered()), this, SLOT(createOrthogonalView()));
}

void QtMainWindow::showTargetImage() {
    irtkColor *drawn;

    for (int i = 0; i < viewers.size(); i++) {
        viewers.at(i)->SetTarget(irtkQtViewer::Instance()->GetTargetImage());
        viewers.at(i)->SetDimensions(viewerWidgets.at(i)->getGlWidget()->width(),
                                     viewerWidgets.at(i)->getGlWidget()->height());
        viewers.at(i)->InitializeOutputImage();

        viewerWidgets.at(i)->getSlider()->setEnabled(true);
        viewerWidgets.at(i)->setMaximumSlice(viewers.at(i)->GetSliceNumber());

        drawn = viewers.at(i)->GetDrawable();
        viewerWidgets.at(i)->getGlWidget()->setDrawable(drawn);
        viewerWidgets.at(i)->getGlWidget()->updateScene();

        viewerWidgets.at(i)->setCurrentSlice(viewers.at(i)->GetCurrentSlice());

//        if (flag != true) {
            connect(viewerWidgets.at(i)->getGlWidget(), SIGNAL(resized(int, int)),
                    viewers.at(i), SLOT(ResizeImage(int, int)));
            connect(viewers.at(i), SIGNAL(ImageResized(irtkColor*)),
                    viewerWidgets.at(i)->getGlWidget(), SLOT(updateDrawable(irtkColor*)));
            flag = true;

//            QMessageBox msgBox;
//            msgBox.setText("Connecting signals to slots.");
//            msgBox.exec();
//        }
//        connect(viewerWidgets.at(i)->getSlider(), SIGNAL(valueChanged(int)),
//                viewers.at(i), SLOT(ChangeSlice(int)));
//        connect(viewers.at(i), SIGNAL(OriginChanged(double, double, double)),
//                this, SLOT(updateOrigin(double, double, double)));
    }
}

QtViewerWidget* QtMainWindow::createTwoDimensionalView(irtkViewMode viewMode) {
    irtkQtTwoDimensionalViewer* viewer;
    QtViewerWidget *qtViewer;

    viewer = irtkQtViewer::Instance()->CreateTwoDimensionalViewer(viewMode);
    viewers.push_back(viewer);
    qtViewer = new QtViewerWidget();
    viewerWidgets.push_back(qtViewer);

    return qtViewer;
}

void QtMainWindow::clearVectors() {
    //qDeleteAll(viewers);
    //qDeleteAll(viewerWidgets);

    viewers.clear();
    viewerWidgets.clear();
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

void QtMainWindow::openSourceImage() {
    QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty()) {
        irtkQtViewer::Instance()->DestroySourceImage();
        irtkQtViewer::Instance()->CreateSourceImage(fileName.toStdString());
    }
}

void QtMainWindow::createOrthogonalView() {
    clearVectors();

    QWidget *mainWidget = new QWidget();
    QGridLayout *layout = new QGridLayout();

    layout->addWidget(createTwoDimensionalView(VIEW_AXIAL), 0, 0);
    layout->addWidget(createTwoDimensionalView(VIEW_SAGITTAL), 0, 1);
    layout->addWidget(createTwoDimensionalView(VIEW_CORONAL), 1, 0);

    mainWidget->setLayout(layout);
    this->setCentralWidget(mainWidget);
}

void QtMainWindow::updateOrigin(double x, double y, double z) {
    printf("new origin is %f, %f, %f \n", x, y, z);
    irtkColor *drawn;

    for (int i = 0; i < viewers.size(); i++) {
        viewers.at(i)->SetOrigin(x, y, z);
        viewers.at(i)->InitializeOutputImage();

        drawn = viewers.at(i)->GetDrawable();
        viewerWidgets.at(i)->getGlWidget()->setDrawable(drawn);
        viewerWidgets.at(i)->getGlWidget()->updateScene();
    }
}
