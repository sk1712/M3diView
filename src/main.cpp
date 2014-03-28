#include "QtMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv, true);

    QtMainWindow window;
    window.show();

    return app.exec();
}
