#include <irtkQtBaseViewer.h>


irtkQtBaseViewer::irtkQtBaseViewer() {
    _targetImage = NULL;
}

irtkQtBaseViewer::~irtkQtBaseViewer() {
    delete [] sliceNum;
}

void irtkQtBaseViewer::AddToDisplayedImages(irtkQtImageObject *imageObject, int index) {
    irtkImage* newImage = imageObject->GetImage();

    currentIndex = index;
    // If first image to be displayed make it target
    if (_image.size() == 0) {
        SetTarget(newImage);
        InitializeOrigin();
        InitializeDimensions();
        InitializeOrientation();
    }
    else if (index < _image.begin()->first) {
        SetTarget(newImage);
        UpdateCurrentSlice();

        InitializeDimensions();
        InitializeOrientation();
        CalculateOutputImages();
        /// check whether image dimensions agree
//        if (!(_targetImage->GetImageAttributes() == newImage->GetImageAttributes())) {
//            delete newImage;
//            return;
//        }
    }

    // If everything is fine add to maps
    AddToMaps(newImage, index);
    _lookupTable.insert(pair<int, irtkQtLookupTable*> ( index, imageObject->GetLookupTable() ));
}

void irtkQtBaseViewer::DeleteSingleImage(int index) {
    // When an image is deleted update the target image
    if (index < _image.begin()->first) {
        SetTarget(_image.begin()->second);
        UpdateCurrentSlice();

        InitializeDimensions();
        InitializeOrientation();
        CalculateOutputImages();
    }
}

void irtkQtBaseViewer::MoveImage(int, int) {
    // Always make first image in the map the target image
    if (_targetImage != _image.begin()->second) {

        SetTarget(_image.begin()->second);
        UpdateCurrentSlice();

        InitializeDimensions();
        InitializeOrientation();
        CalculateOutputImages();
    }
}

irtkImageAttributes irtkQtBaseViewer::InitializeAttributes() {
    irtkImageAttributes attr;

    // Image size
    attr._x = _width;
    attr._y = _height;
    attr._z = 1;

    // Image origin in world coordinates
    attr._xorigin = _originX;
    attr._yorigin = _originY;
    attr._zorigin = _originZ;

    // Voxel size
    attr._dx = _dx;
    attr._dy = _dy;
    attr._dz = _dz;

    // x, y, z axes
    attr._xaxis[0] = _axisX[0];
    attr._xaxis[1] = _axisX[1];
    attr._xaxis[2] = _axisX[2];
    attr._yaxis[0] = _axisY[0];
    attr._yaxis[1] = _axisY[1];
    attr._yaxis[2] = _axisY[2];
    attr._zaxis[0] = _axisZ[0];
    attr._zaxis[1] = _axisZ[1];
    attr._zaxis[2] = _axisZ[2];

    return attr;
}

void irtkQtBaseViewer::InitializeOrigin() {
    // Get original image origin
    _targetImage->GetOrigin(_originX, _originY, _originZ);

    UpdateCurrentSlice();
}

void irtkQtBaseViewer::InitializeDimensions() {
    switch (_viewMode) {
    case VIEW_AXIAL :
        *sliceNum = _targetImage->GetZ();
        break;
    case VIEW_SAGITTAL :
        *sliceNum = _targetImage->GetX();
        break;
    case VIEW_CORONAL :
        *sliceNum = _targetImage->GetY();
        break;
    case VIEW_3D:
        sliceNum[0] = _targetImage->GetX();
        sliceNum[1] = _targetImage->GetY();
        sliceNum[2] = _targetImage->GetZ();
        break;
    default:
        cerr << "Not a valid type of viewer" << endl;
        exit(1);
        break;
    }
}

void irtkQtBaseViewer::InitializeOrientation() {
    double x[3], y[3], z[3];

    // Get original image orientation
    _targetImage->GetOrientation(x, y, z);

    switch (_viewMode) {
    case VIEW_AXIAL :
        SetOrientation(x, y, z);
        break;
    case VIEW_SAGITTAL :
        SetOrientation(y, z, x);
        break;
    case VIEW_CORONAL :
        SetOrientation(x, z, y);
        break;
    case VIEW_3D :
        break;
    default:
        cerr << "Not a valid type of viewer" << endl;
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
