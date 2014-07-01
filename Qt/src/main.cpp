#include <QtMainWindow.h>

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QGLFormat>
#include <QDateTime>


// Callback function for formatting log messages
void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message;
    // Get the current date and time
    QDateTime currentDateTime = QDateTime::currentDateTime();

    switch (type) {
    case QtDebugMsg:
        message = QString("[%1] Debug: %2 (%3:%4, %5)").
                arg(currentDateTime.toString(Qt::ISODate)).arg(msg).arg(context.file).
                    arg(context.line).arg(context.function);
        break;
    case QtWarningMsg:
        message = QString("[%1] Warning: %2 (%3:%4, %5)").
                arg(currentDateTime.toString(Qt::ISODate)).arg(msg).arg(context.file).
                    arg(context.line).arg(context.function);
        break;
    case QtCriticalMsg:
        message = QString("[%1] Critical: %2 (%3:%4, %5)").
                arg(currentDateTime.toString(Qt::ISODate)).arg(msg).arg(context.file).
                    arg(context.line).arg(context.function);
        break;
    case QtFatalMsg:
        message = QString("[%1] Fatal: %2 (%3:%4, %5)").
                arg(currentDateTime.toString(Qt::ISODate)).arg(msg).arg(context.file).
                    arg(context.line).arg(context.function);
        break;
    }

    // Print to the output log file
    QFile outFile("viewer.log");
    QFileInfo info(outFile);

    // Set limit to log file size of 200MB
    if (info.size() > 200*1024*1024)
        outFile.open(QIODevice::WriteOnly);
    else
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream ts(&outFile);
    ts << message << endl;
}

int main(int argc, char *argv[])
{
    // Set the callback for message handler
    qInstallMessageHandler(messageOutput);

    QApplication app(argc, argv);

    if ( !QGLFormat::hasOpenGL() ) {
        qFatal("The window system does not have OpenGL support");
        return -1;
    }

    QtMainWindow window;
    window.setWindowState(Qt::WindowMaximized);
    window.show();

    // Load image files from command line
    QStringList args = app.arguments();
    if (args.length() > 1) {
        args.removeAt(0);
        window.loadImages(args);
    }

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
