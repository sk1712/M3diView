#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QtViewerWidget.h>

#include <irtkQtViewer.h>

#include <QMainWindow>
#include <QListWidget>
#include <QSplitter>

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

    QSplitter *splitter;
    QListWidget *listWidget;
    QWidget *mainViewWidget;

    QMenu *fileMenu;
    QMenu *viewMenu;

    QToolBar *toolbar;

    QAction *openTargetAction;
    QAction *viewOrthogonalAction;
    QAction *viewSelectedImageAction;

    QVector<QtViewerWidget*> viewerWidgets;
    QVector<irtkQtTwoDimensionalViewer*> viewers;

public:
    QtMainWindow();
    ~QtMainWindow();

private:
    void createToolBar();
    void createMenu();
    void createActions();
    void disconnectSignals();
    void connectSignals();
    void showTargetImage(int i);
    QtViewerWidget* createTwoDimensionalView(irtkViewMode viewMode);
    void clearVectors();

private slots:
    void openImage();
    void viewImage();

    void createOrthogonalView();
    void updateOrigin(double x, double y, double z);
};

#endif // QTMAINWINDOW_H
