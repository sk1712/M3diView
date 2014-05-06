#include <QtMainWindow.h>

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtMainWindow window;
    window.setWindowState(Qt::WindowMaximized);
    window.show();

#ifdef Q_OS_MAC
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QFile styleFile( ":/stylesheets/stylesheet.qss" );
    styleFile.open( QFile::ReadOnly );
    // Apply the loaded stylesheet
    QString style( styleFile.readAll() );
    app.setStyleSheet(style);

    return app.exec();
}
