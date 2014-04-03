#include "QtMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtMainWindow window;
    window.show();

    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    return app.exec();
}
