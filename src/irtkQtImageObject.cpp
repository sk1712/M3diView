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
        ConvertImageToTarget();
    }
    // if not, throw an exception and return
    catch (irtkException e) {
        throw e;
        return;
    }

    // set up the look up table
    _lookupTable = new irtkQtLookupTable;

    double imageMin, imageMax;
    _image->GetMinMaxAsDouble(&imageMin, &imageMax);
    _lookupTable->SetMinMaxImageValues(imageMin, imageMax);
    _lookupTable->SetMinMaxDisplayValues(imageMin, imageMax);
    _lookupTable->Initialize();
}

void irtkQtImageObject::ConvertImageToTarget() {
    int iaxis, jaxis, kaxis;
    double xaxis[3], yaxis[3], zaxis[3];

    irtkGreyImage *outputImage = new irtkGreyImage;
    irtkTransformation *transformation = new irtkAffineTransformation;
    irtkImageFunction *interpolator = new irtkNearestNeighborInterpolateImageFunction;

    irtkImageAttributes attr = _image->GetImageAttributes();

    _image->GetOrientation(xaxis, yaxis, zaxis);
    _image->Orientation(iaxis, jaxis, kaxis);

    switch (iaxis) {
    case IRTK_L2R:
         attr._xaxis[0] = -xaxis[0];
         attr._xaxis[1] = -xaxis[1];
         attr._xaxis[2] = -xaxis[2];
         attr._x = _image->GetX();
         attr._dx = _image->GetXSize();
        break;
    case IRTK_R2L:
         attr._xaxis[0] = xaxis[0];
         attr._xaxis[1] = xaxis[1];
         attr._xaxis[2] = xaxis[2];
         attr._x = _image->GetX();
         attr._dx = _image->GetXSize();
        break;
    case IRTK_P2A:
         attr._yaxis[0] = xaxis[0];
         attr._yaxis[1] = xaxis[1];
         attr._yaxis[2] = xaxis[2];
         attr._y = _image->GetX();
         attr._dy = _image->GetXSize();
        break;
    case IRTK_A2P:
         attr._yaxis[0] = -xaxis[0];
         attr._yaxis[1] = -xaxis[1];
         attr._yaxis[2] = -xaxis[2];
         attr._y = _image->GetX();
         attr._dy = _image->GetXSize();
        break;
    case IRTK_I2S:
         attr._zaxis[0] = xaxis[0];
         attr._zaxis[1] = xaxis[1];
         attr._zaxis[2] = xaxis[2];
         attr._z = _image->GetX();
         attr._dz = _image->GetXSize();
        break;
    case IRTK_S2I:
         attr._zaxis[0] = -xaxis[0];
         attr._zaxis[1] = -xaxis[1];
         attr._zaxis[2] = -xaxis[2];
         attr._z = _image->GetX();
         attr._dz = _image->GetXSize();
        break;
    default:
        cerr << "irtkQtImageObject::ConvertImageToTarget: Can't work out x-orientation" << endl;
        break;
    }

    switch (jaxis) {
    case IRTK_L2R:
         attr._xaxis[0] = -yaxis[0];
         attr._xaxis[1] = -yaxis[1];
         attr._xaxis[2] = -yaxis[2];
         attr._x = _image->GetY();
         attr._dx = _image->GetYSize();
        break;
    case IRTK_R2L:
         attr._xaxis[0] = yaxis[0];
         attr._xaxis[1] = yaxis[1];
         attr._xaxis[2] = yaxis[2];
         attr._x = _image->GetY();
         attr._dx = _image->GetYSize();
        break;
    case IRTK_P2A:
         attr._yaxis[0] = yaxis[0];
         attr._yaxis[1] = yaxis[1];
         attr._yaxis[2] = yaxis[2];
         attr._y = _image->GetY();
         attr._dy = _image->GetYSize();
        break;
    case IRTK_A2P:
         attr._yaxis[0] = -yaxis[0];
         attr._yaxis[1] = -yaxis[1];
         attr._yaxis[2] = -yaxis[2];
         attr._y = _image->GetY();
         attr._dy = _image->GetYSize();
        break;
    case IRTK_I2S:
         attr._zaxis[0] = yaxis[0];
         attr._zaxis[1] = yaxis[1];
         attr._zaxis[2] = yaxis[2];
         attr._z = _image->GetY();
         attr._dz = _image->GetYSize();
        break;
    case IRTK_S2I:
         attr._zaxis[0] = -yaxis[0];
         attr._zaxis[1] = -yaxis[1];
         attr._zaxis[2] = -yaxis[2];
         attr._z = _image->GetY();
         attr._dz = _image->GetYSize();
        break;
    default:
        cerr << "irtkQtImageObject::ConvertImageToTarget: Can't work out y-orientation" << endl;
        break;
    }

    switch (kaxis) {
    case IRTK_L2R:
         attr._xaxis[0] = -zaxis[0];
         attr._xaxis[1] = -zaxis[1];
         attr._xaxis[2] = -zaxis[2];
         attr._x = _image->GetZ();
         attr._dx = _image->GetZSize();
        break;
    case IRTK_R2L:
         attr._xaxis[0] = zaxis[0];
         attr._xaxis[1] = zaxis[1];
         attr._xaxis[2] = zaxis[2];
         attr._x = _image->GetZ();
         attr._dx = _image->GetZSize();
        break;
    case IRTK_P2A:
         attr._yaxis[0] = zaxis[0];
         attr._yaxis[1] = zaxis[1];
         attr._yaxis[2] = zaxis[2];
         attr._y = _image->GetZ();
         attr._dy = _image->GetZSize();
        break;
    case IRTK_A2P:
         attr._yaxis[0] = -zaxis[0];
         attr._yaxis[1] = -zaxis[1];
         attr._yaxis[2] = -zaxis[2];
         attr._y = _image->GetZ();
         attr._dy = _image->GetZSize();
        break;
    case IRTK_I2S:
         attr._zaxis[0] = zaxis[0];
         attr._zaxis[1] = zaxis[1];
         attr._zaxis[2] = zaxis[2];
         attr._z = _image->GetZ();
         attr._dz = _image->GetZSize();
        break;
    case IRTK_S2I:
         attr._zaxis[0] = -zaxis[0];
         attr._zaxis[1] = -zaxis[1];
         attr._zaxis[2] = -zaxis[2];
         attr._z = _image->GetZ();
         attr._dz = _image->GetZSize();
        break;
    default:
        cerr << "irtkQtImageObject::ConvertImageToTarget: Can't work out z-orientation" << endl;
        break;
    }

    outputImage->Initialize(attr);

    irtkImageTransformation *imageTransformation = irtkImageTransformation::New(transformation);
    imageTransformation->SetInput(_image);
    imageTransformation->SetOutput(outputImage);
    imageTransformation->PutInterpolator(interpolator);
    imageTransformation->PutSourcePaddingValue(0);
    imageTransformation->Run();

    delete _image;
    _image = outputImage;

    delete transformation;
    delete interpolator;
    delete imageTransformation;
}

void irtkQtImageObject::DeleteImage() {
    delete _image;
    delete _lookupTable;

    _image = NULL;
    _lookupTable = NULL;
}
