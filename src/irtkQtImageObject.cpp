#include <irtkQtImageObject.h>

#include <QFileInfo>


irtkQtImageObject::irtkQtImageObject(const QString &path)
    : _path(path)
{
    QFileInfo file(path);
    _fileName = file.fileName();

    _visible = false;

    _image = NULL;
    _lookupTable = NULL;
}

irtkQtImageObject::~irtkQtImageObject() {
    delete _image;
    delete _lookupTable;
}

void irtkQtImageObject::CreateImage() {
    // check if image object can be constructed from the file
    try {
        _image = irtkImage::New(_path.toStdString().c_str());
    }
    // if not, throw an exception and return
    catch (irtkException e) {
        throw e;
        return;
    }

    _lookupTable = new irtkQtLookupTable;

    double imageMin, imageMax;
    _image->GetMinMaxAsDouble(&imageMin, &imageMax);
    _lookupTable->SetMinMaxImageValues(imageMin, imageMax);
    _lookupTable->SetMinMaxDisplayValues(imageMin, imageMax);
}

void irtkQtImageObject::DeleteImage() {
    delete _image;
    delete _lookupTable;

    _image = NULL;
    _lookupTable = NULL;
}
