#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QtViewerWidget.h>

#include <irtkQtViewer.h>

#include <QMainWindow>
#include <QListWidget>
#include <QSplitter>
#include <QMessageBox>

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
    QAction *viewAxialAction;
    QAction *viewCoronalAction;
    QAction *viewSagittalAction;
    QAction *viewOrthogonalAction;
    QAction *clearViewsAction;

    QAction *viewSelectedImageAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;

    QVector<QtViewerWidget*> viewerWidgets;
    QVector<irtkQtTwoDimensionalViewer*> viewers;

    bool singleViewerInScreen;

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

    bool imageInList(const QString fileName);

    void addToViewWidget(QWidget *widget);
    void createMessageBox(QString message,
                          QMessageBox::Icon icon = QMessageBox::NoIcon);

private slots:
    void openImage();
    void viewImage();

    void zoomIn();
    void zoomOut();

    void showOnlyThisWidget();
    void deleteThisWidget();

    void createAxialView();
    void createCoronalView();
    void createSagittalView();
    void createOrthogonalView();
    void clearViews();

    void updateOrigin(double x, double y, double z);
};

#endif // QTMAINWINDOW_H
