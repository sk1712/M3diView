#include <irtkQtImageObject.h>

#include <QFileInfo>

irtkQtImageObject::irtkQtImageObject(const QString &path)
    : image(NULL), _path(path)
{
    image = irtkImage::New(path.toStdString().c_str());
    QFileInfo file(path);
    _fileName = file.fileName();

    _visible = false;
    _opacity = 255;
}

irtkQtImageObject::~irtkQtImageObject() {
    delete image;
}
