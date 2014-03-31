#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QtViewerWidget.h>

#include <irtkQtViewer.h>

#include <QMainWindow>

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

    QMenu *fileMenu;
    QMenu *viewMenu;

    QAction *openTargetAction;
    QAction *viewOrthogonalAction;

    QVector<QtViewerWidget*> viewerWidgets;
    QVector<irtkQtTwoDimensionalViewer*> viewers;

public:
    QtMainWindow();
    ~QtMainWindow();

private:
    void createMenu();
    void createActions();
    void disconnectSignals();
    void connectSignals();
    void showTargetImage();
    QtViewerWidget* createTwoDimensionalView(irtkViewMode viewMode);
    void clearVectors();

private slots:
    void openTargetImage();

    void createOrthogonalView();
    void updateOrigin(double x, double y, double z);
};

#endif // QTMAINWINDOW_H
