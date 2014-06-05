#include <QtMainWindow.h>

#include <QApplication>
#include <QFile>
#include <QGLFormat>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if ( !QGLFormat::hasOpenGL() ) {
        cerr << "The window system does not have OpenGL support" << endl;
        return -1;
    }

    QtMainWindow window;
    //window.resize(1000, 800);
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
