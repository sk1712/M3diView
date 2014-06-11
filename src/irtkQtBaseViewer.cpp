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
    // if first image to be displayed make it target
    if (_image.size() == 0) {
        delete _targetImage;
        ConvertImageToTarget(newImage);
        InitializeOrigin();
        InitializeDimensions();
        InitializeOrientation();
    }
    else if (index < _image.begin()->first) {
        delete _targetImage;
        ConvertImageToTarget(newImage);
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

    // if everything is fine add to maps
    AddToMaps(newImage, index);
    _lookupTable.insert(pair<int, irtkQtLookupTable*> ( index, imageObject->GetLookupTable() ));
}

void irtkQtBaseViewer::DeleteSingleImage(int index) {
    // when an image is deleted update the target image
    if (index < _image.begin()->first) {
        delete _targetImage;
        ConvertImageToTarget(_image.begin()->second);
        UpdateCurrentSlice();

        InitializeDimensions();
        InitializeOrientation();
        CalculateOutputImages();
    }
}

void irtkQtBaseViewer::MoveImage(int, int) {
    // always make first image in the map the target image
    delete _targetImage;
    ConvertImageToTarget(_image.begin()->second);
    UpdateCurrentSlice();

    InitializeDimensions();
    InitializeOrientation();    
    CalculateOutputImages();
}

irtkImageAttributes irtkQtBaseViewer::InitializeAttributes() {
    irtkImageAttributes attr;

    // image size
    attr._x = _width;
    attr._y = _height;
    attr._z = 1;

    // image origin in world coordinates
    attr._xorigin = _originX;
    attr._yorigin = _originY;
    attr._zorigin = _originZ;

    // voxel size
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

void irtkQtBaseViewer::ConvertImageToTarget(irtkImage *originalImage) {
    int iaxis, jaxis, kaxis;
    double xaxis[3], yaxis[3], zaxis[3];

    irtkGreyImage *outputImage = new irtkGreyImage;
    irtkTransformation *transformation = new irtkAffineTransformation;
    irtkImageFunction *interpolator = new irtkNearestNeighborInterpolateImageFunction;

    irtkImageAttributes attr = originalImage->GetImageAttributes();

    originalImage->GetOrientation(xaxis, yaxis, zaxis);
    originalImage->Orientation(iaxis, jaxis, kaxis);

    switch (iaxis) {
    case IRTK_L2R:
         attr._xaxis[0] = -xaxis[0];
         attr._xaxis[1] = -xaxis[1];
         attr._xaxis[2] = -xaxis[2];
         attr._x = originalImage->GetX();
         attr._dx = originalImage->GetXSize();
        break;
    case IRTK_R2L:
         attr._xaxis[0] = xaxis[0];
         attr._xaxis[1] = xaxis[1];
         attr._xaxis[2] = xaxis[2];
         attr._x = originalImage->GetX();
         attr._dx = originalImage->GetXSize();
        break;
    case IRTK_P2A:
         attr._yaxis[0] = xaxis[0];
         attr._yaxis[1] = xaxis[1];
         attr._yaxis[2] = xaxis[2];
         attr._y = originalImage->GetX();
         attr._dy = originalImage->GetXSize();
        break;
    case IRTK_A2P:
         attr._yaxis[0] = -xaxis[0];
         attr._yaxis[1] = -xaxis[1];
         attr._yaxis[2] = -xaxis[2];
         attr._y = originalImage->GetX();
         attr._dy = originalImage->GetXSize();
        break;
    case IRTK_I2S:
         attr._zaxis[0] = xaxis[0];
         attr._zaxis[1] = xaxis[1];
         attr._zaxis[2] = xaxis[2];
         attr._z = originalImage->GetX();
         attr._dz = originalImage->GetXSize();
        break;
    case IRTK_S2I:
         attr._zaxis[0] = -xaxis[0];
         attr._zaxis[1] = -xaxis[1];
         attr._zaxis[2] = -xaxis[2];
         attr._z = originalImage->GetX();
         attr._dz = originalImage->GetXSize();
        break;
    default:
        cerr << "irtkQtBaseViewer::ConvertImageToTarget: Can't work out x-orientation" << endl;
        break;
    }

    switch (jaxis) {
    case IRTK_L2R:
         attr._xaxis[0] = -yaxis[0];
         attr._xaxis[1] = -yaxis[1];
         attr._xaxis[2] = -yaxis[2];
         attr._x = originalImage->GetY();
         attr._dx = originalImage->GetYSize();
        break;
    case IRTK_R2L:
         attr._xaxis[0] = yaxis[0];
         attr._xaxis[1] = yaxis[1];
         attr._xaxis[2] = yaxis[2];
         attr._x = originalImage->GetY();
         attr._dx = originalImage->GetYSize();
        break;
    case IRTK_P2A:
         attr._yaxis[0] = yaxis[0];
         attr._yaxis[1] = yaxis[1];
         attr._yaxis[2] = yaxis[2];
         attr._y = originalImage->GetY();
         attr._dy = originalImage->GetYSize();
        break;
    case IRTK_A2P:
         attr._yaxis[0] = -yaxis[0];
         attr._yaxis[1] = -yaxis[1];
         attr._yaxis[2] = -yaxis[2];
         attr._y = originalImage->GetY();
         attr._dy = originalImage->GetYSize();
        break;
    case IRTK_I2S:
         attr._zaxis[0] = yaxis[0];
         attr._zaxis[1] = yaxis[1];
         attr._zaxis[2] = yaxis[2];
         attr._z = originalImage->GetY();
         attr._dz = originalImage->GetYSize();
        break;
    case IRTK_S2I:
         attr._zaxis[0] = -yaxis[0];
         attr._zaxis[1] = -yaxis[1];
         attr._zaxis[2] = -yaxis[2];
         attr._z = originalImage->GetY();
         attr._dz = originalImage->GetYSize();
        break;
    default:
        cerr << "irtkQtBaseViewer::ConvertImageToTarget: Can't work out y-orientation" << endl;
        break;
    }

    switch (kaxis) {
    case IRTK_L2R:
         attr._xaxis[0] = -zaxis[0];
         attr._xaxis[1] = -zaxis[1];
         attr._xaxis[2] = -zaxis[2];
         attr._x = originalImage->GetZ();
         attr._dx = originalImage->GetZSize();
        break;
    case IRTK_R2L:
         attr._xaxis[0] = zaxis[0];
         attr._xaxis[1] = zaxis[1];
         attr._xaxis[2] = zaxis[2];
         attr._x = originalImage->GetZ();
         attr._dx = originalImage->GetZSize();
        break;
    case IRTK_P2A:
         attr._yaxis[0] = zaxis[0];
         attr._yaxis[1] = zaxis[1];
         attr._yaxis[2] = zaxis[2];
         attr._y = originalImage->GetZ();
         attr._dy = originalImage->GetZSize();
        break;
    case IRTK_A2P:
         attr._yaxis[0] = -zaxis[0];
         attr._yaxis[1] = -zaxis[1];
         attr._yaxis[2] = -zaxis[2];
         attr._y = originalImage->GetZ();
         attr._dy = originalImage->GetZSize();
        break;
    case IRTK_I2S:
         attr._zaxis[0] = zaxis[0];
         attr._zaxis[1] = zaxis[1];
         attr._zaxis[2] = zaxis[2];
         attr._z = originalImage->GetZ();
         attr._dz = originalImage->GetZSize();
        break;
    case IRTK_S2I:
         attr._zaxis[0] = -zaxis[0];
         attr._zaxis[1] = -zaxis[1];
         attr._zaxis[2] = -zaxis[2];
         attr._z = originalImage->GetZ();
         attr._dz = originalImage->GetZSize();
        break;
    default:
        cerr << "irtkQtBaseViewer::ConvertImageToTarget: Can't work out z-orientation" << endl;
        break;
    }

    outputImage->Initialize(attr);

    irtkImageTransformation *imageTransformation = irtkImageTransformation::New(transformation);
    imageTransformation->SetInput(originalImage);
    imageTransformation->SetOutput(outputImage);
    imageTransformation->PutInterpolator(interpolator);
    imageTransformation->PutSourcePaddingValue(0);
    imageTransformation->Run();

    SetTarget(outputImage);

    delete transformation;
    delete interpolator;
    delete imageTransformation;
}

void irtkQtBaseViewer::InitializeOrigin() {
    // get original image origin
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

    // get original image orientation
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
