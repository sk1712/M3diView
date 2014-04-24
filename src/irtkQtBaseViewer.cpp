#include <irtkQtBaseViewer.h>


void irtkQtBaseViewer::InitializeOutputImage() {
    irtkImageAttributes attr;

    attr._x = _width;
    attr._y = _height;
    attr._z = 1;

    attr._xorigin = _originX;
    attr._yorigin = _originY;
    attr._zorigin = _originZ;

    attr._dx = _dx;
    attr._dy = _dy;
    attr._dz = _dz;

    attr._xaxis[0] = _axisX[0];
    attr._xaxis[1] = _axisX[1];
    attr._xaxis[2] = _axisX[2];
    attr._yaxis[0] = _axisY[0];
    attr._yaxis[1] = _axisY[1];
    attr._yaxis[2] = _axisY[2];
    attr._zaxis[0] = _axisZ[0];
    attr._zaxis[1] = _axisZ[1];
    attr._zaxis[2] = _axisZ[2];

    // calculate the actual output images
    CalculateOutputImages(attr);
}

void irtkQtBaseViewer::AddToDisplayedImages(irtkQtImageObject *imageObject) {
    irtkImage *newImage = NULL;

    // check if image object can be constructed from the file
    try {
        newImage = irtkImage::New(imageObject->GetPath().toStdString().c_str());
    }
    catch (irtkException e) {
        throw e;
        return;
    }

    // if first image to be displayed make it target, otherwise check if attributes agree
    if (_image.size() == 0) {
        _targetImage = newImage;
        SetResolution(1, 1, _targetImage->GetZSize());
        InitializeOriginOrientation();
    }
    else {
        if (!(_targetImage->GetImageAttributes() == newImage->GetImageAttributes())) {
            delete newImage;
            return;
        }
    }

    // if everything is fine add to vectors
    AddToVectors(newImage);
    _lookupTable.push_back(new irtkQtLookupTable());
    _lookupTable.back()->SetAlpha(imageObject->GetOpacity());
}

void irtkQtBaseViewer::InitializeOriginOrientation() {
    double x[3], y[3], z[3];

    _targetImage->GetOrientation(x, y, z);
    _targetImage->GetOrigin(_originX, _originY, _originZ);

    switch (_viewMode) {
    case VIEW_AXIAL :
        sliceNum = _targetImage->GetZ();
        SetOrientation(x, y, z);
        break;
    case VIEW_SAGITTAL :
        sliceNum = _targetImage->GetX();
        SetOrientation(y, z, x);
        break;
    case VIEW_CORONAL :
        sliceNum = _targetImage->GetY();
        SetOrientation(x, z, y);
        break;
    default:
        cerr << "Not a valid type of two dimensional viewer" << endl;
        exit(1);
        break;
    }
}

void irtkQtBaseViewer::SetOrientation(const double * xaxis, const double * yaxis, const double * zaxis) {
    _axisX[0] = xaxis[0];
    _axisX[1] = xaxis[1];
    _axisX[2] = xaxis[2];

    _axisY[0] = yaxis[0];
    _axisY[1] = yaxis[1];
    _axisY[2] = yaxis[2];

    _axisZ[0] = zaxis[0];
    _axisZ[1] = zaxis[1];
    _axisZ[2] = zaxis[2];
}
