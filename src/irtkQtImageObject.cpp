#include <irtkQtImageObject.h>

#include <QFileInfo>

// Initialize static interpolation string list
QStringList irtkQtImageObject::_interpolationStringList;

irtkQtImageObject::irtkQtImageObject()
    : _visible(false), _image(NULL), _lookupTable(NULL)
{
    _labelColor = QColor(255, 255, 255);
}

irtkQtImageObject::~irtkQtImageObject() {
    delete _image;
    delete _lookupTable;
}

void irtkQtImageObject::setImagePath(const QString &path) {
    _path = path;
    QFileInfo file(path);
    _fileName = file.fileName();
}

void irtkQtImageObject::SetInterpolationModeList() {
    _interpolationStringList << "Nearest-neighbor"
                             << "Linear"
                             << "C-spline"
                             << "B-spline"
                             << "Sinc";
}

QStringList irtkQtImageObject::GetInterpolationModeList() {
    return _interpolationStringList;
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

    // If image is corrupted, then throw irtkException
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

    // Nearest neighbor interpolation by default
    _interpolation = INTERPOLATION_NN;
}

void irtkQtImageObject::DeleteImage() {
    delete _image;
    delete _lookupTable;

    _image = NULL;
    _lookupTable = NULL;
}


void irtkQtImageObject::SetLabelColor(const QColor &color) {
    _labelColor = color;
}

QColor irtkQtImageObject::GetLabelColor() const {
    return _labelColor;
}
