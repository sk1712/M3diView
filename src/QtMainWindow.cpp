#include <QtMainWindow.h>

#include <irtkQtViewer.h>

#include <QMenuBar>
#include <QFileDialog>

QtMainWindow::QtMainWindow() {
    QtViewerWidget *qtViewer = new QtViewerWidget();
    this->setCentralWidget(qtViewer);

    createActions();
    createMenu();

    printf("created main widget \n");
}

void QtMainWindow::createMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openTargetAction);
    fileMenu->addAction(openSourceAction);
}

void QtMainWindow::createActions() {
    openTargetAction = new QAction(tr("&Open target..."), this);
    openTargetAction->setStatusTip(tr("Open target image file"));
    connect(openTargetAction, SIGNAL(triggered()), this, SLOT(openTargetImage()));

    openSourceAction = new QAction(tr("&Open source..."), this);
    openSourceAction->setStatusTip(tr("Open source image file"));
    connect(openSourceAction, SIGNAL(triggered()), this, SLOT(openSourceImage()));
}

void QtMainWindow::openTargetImage() {
    QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty())
        irtkQtViewer::Instance()->CreateTargetImage(fileName.toStdString());
}

void QtMainWindow::openSourceImage() {
    QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty())
        irtkQtViewer::Instance()->CreateSourceImage(fileName.toStdString());
}
