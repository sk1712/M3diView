#include <irtkQtImageObject.h>

#include <QFileInfo>
#include <QDebug>


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
    // Check if image object can be constructed from the file
    try {
        _image = irtkImage::New(_path.toStdString().c_str());
    }
    // If not, throw an exception and return
    catch (irtkException e) {
        qCritical("Invalid file '%s'", qPrintable(_path));
        throw e;
        return;
    }

    double imageMin, imageMax;
    _image->GetMinMaxAsDouble(&imageMin, &imageMax);

    // If image is corrupted the throw irtkException
    if ( (imageMin == 0) && (imageMax == 0) ) {
        delete _image;
        qCritical("Invalid image file '%s'", qPrintable(_path));
        throw irtkException("Invalid image file", _path.toStdString());
        return;
    }

    // Initialize the lookup table corresponding to the image min, max values
    _lookupTable = new irtkQtLookupTable;
    _lookupTable->SetMinMaxImageValues(imageMin, imageMax);
    _lookupTable->SetMinMaxDisplayValues(imageMin, imageMax);
    _lookupTable->Initialize();

    _interpolation = INTERPOLATION_NN;
}

void irtkQtImageObject::DeleteImage() {
    delete _image;
    delete _lookupTable;

    _image = NULL;
    _lookupTable = NULL;
}
