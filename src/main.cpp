#include "QtMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtMainWindow window;
    window.show();

#ifdef Q_OS_MAC
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    return app.exec();
}
