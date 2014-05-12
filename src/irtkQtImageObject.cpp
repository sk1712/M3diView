#include <irtkQtImageObject.h>

#include <QFileInfo>


irtkQtImageObject::irtkQtImageObject(const QString &path)
    : _path(path)
{
    QFileInfo file(path);
    _fileName = file.fileName();

    _visible = false;
}

irtkQtImageObject::~irtkQtImageObject() {

}
