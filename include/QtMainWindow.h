#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QtViewerWidget.h>

#include <QMainWindow>

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

    QMenu *fileMenu;
    QAction *openTargetAction;
    QAction *openSourceAction;

public:
    QtMainWindow();

private:
    void createMenu();
    void createActions();

private slots:
    void openTargetImage();
    void openSourceImage();
};

#endif // QTMAINWINDOW_H
