#include <irtkQtBaseViewer.h>


QStringList irtkQtBaseViewer::_interpolationStringList;

irtkQtLookupTable *irtkQtBaseViewer::subtractionLookupTable = NULL;

irtkQtBaseViewer::irtkQtBaseViewer() {
    _blendMode = VIEW_BLEND;
    _viewMix = 0.5;
    _targetImage = NULL;
}

irtkQtBaseViewer::~irtkQtBaseViewer() {
    delete [] sliceNum;
    delete [] inverted;
    delete subtractionLookupTable;
}

void irtkQtBaseViewer::SetInterpolationModeList() {
    _interpolationStringList << "Nearest-neighbor"
                             << "Linear"
                             << "C-spline"
                             << "B-spline"
                             << "Sinc";
}

QStringList irtkQtBaseViewer::GetInterpolationModeList() {
    return _interpolationStringList;
}

vector<QRgb**> irtkQtBaseViewer::GetDrawable() {
    vector<QRgb**> allDrawables;

    switch (_blendMode) {
    case VIEW_A:
        delete subtractionLookupTable;
        subtractionLookupTable = NULL;
        allDrawables.push_back(GetOnlyADrawable());
        break;
    case VIEW_B:
        delete subtractionLookupTable;
        subtractionLookupTable = NULL;
        allDrawables.push_back(GetOnlyBDrawable());
        break;
    case VIEW_HSHUTTER:
        delete subtractionLookupTable;
        subtractionLookupTable = NULL;
        allDrawables.push_back(GetHShutterDrawable());
        break;
    case VIEW_VSHUTTER:
        delete subtractionLookupTable;
        subtractionLookupTable = NULL;
        allDrawables.push_back(GetVShutterDrawable());
        break;
    case VIEW_SUBTRACT:
        allDrawables.push_back(GetSubtractionDrawable());
        break;
    case VIEW_BLEND:
        delete subtractionLookupTable;
        subtractionLookupTable = NULL;
        allDrawables = GetBlendDrawable();
        break;
    default:
        qCritical("Unknown blending option");
    }

    return allDrawables;
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
    int iaxis, jaxis, kaxis;

    _targetImage->Orientation(iaxis, jaxis, kaxis);

    switch (_viewMode) {
    case VIEW_AXIAL :
        switch (iaxis) {
        case IRTK_S2I:
            *inverted = true;
            *sliceNum = _targetImage->GetX();
            break;
        case IRTK_I2S:
            *inverted = false;
            *sliceNum = _targetImage->GetX();
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_S2I:
            *inverted = true;
            *sliceNum = _targetImage->GetY();
            break;
        case IRTK_I2S:
            *inverted = false;
            *sliceNum = _targetImage->GetY();
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_S2I:
            *inverted = true;
            *sliceNum = _targetImage->GetZ();
            break;
        case IRTK_I2S:
            *inverted = false;
            *sliceNum = _targetImage->GetZ();
        default:
            break;
        }
        break;
    case VIEW_SAGITTAL :
        switch (iaxis) {
        case IRTK_L2R:
            *inverted = true;
            *sliceNum = _targetImage->GetX();
            break;
        case IRTK_R2L:
            *inverted = false;
            *sliceNum = _targetImage->GetX();
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_L2R:
            *inverted = true;
            *sliceNum = _targetImage->GetY();
            break;
        case IRTK_R2L:
            *inverted = true;
            *sliceNum = _targetImage->GetY();
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_L2R:
            *inverted = true;
            *sliceNum = _targetImage->GetZ();
            break;
        case IRTK_R2L:
            *inverted = false;
            *sliceNum = _targetImage->GetZ();
        default:
            break;
        }
        break;
    case VIEW_CORONAL :
        switch (iaxis) {
        case IRTK_A2P:
            *inverted = true;
            *sliceNum = _targetImage->GetX();
            break;
        case IRTK_P2A:
            *inverted = false;
            *sliceNum = _targetImage->GetX();
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_A2P:
            *inverted = true;
            *sliceNum = _targetImage->GetY();
            break;
        case IRTK_P2A:
            *inverted = false;
            *sliceNum = _targetImage->GetY();
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_A2P:
            *inverted = true;
            *sliceNum = _targetImage->GetZ();
            break;
        case IRTK_P2A:
            *inverted = false;
            *sliceNum = _targetImage->GetZ();
        default:
            break;
        }
        break;
    case VIEW_3D:
        switch (iaxis) {
        case IRTK_L2R:
            inverted[0] = true;
            sliceNum[0] = _targetImage->GetX();
            break;
        case IRTK_R2L:
            inverted[0] = false;
            sliceNum[0] = _targetImage->GetX();
            break;
        case IRTK_A2P:
            inverted[1] = true;
            sliceNum[1] = _targetImage->GetX();
            break;
        case IRTK_P2A:
            inverted[1] = false;
            sliceNum[1] = _targetImage->GetX();
            break;
        case IRTK_S2I:
            inverted[2] = true;
            sliceNum[2] = _targetImage->GetX();
            break;
        case IRTK_I2S:
            inverted[2] = false;
            sliceNum[2] = _targetImage->GetX();
            break;
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_L2R:
            inverted[0] = true;
            sliceNum[0] = _targetImage->GetY();
            break;
        case IRTK_R2L:
            inverted[0] = false;
            sliceNum[0] = _targetImage->GetY();
            break;
        case IRTK_A2P:
            inverted[1] = true;
            sliceNum[1] = _targetImage->GetY();
            break;
        case IRTK_P2A:
            inverted[1] = false;
            sliceNum[1] = _targetImage->GetY();
            break;
        case IRTK_S2I:
            inverted[2] = true;
            sliceNum[2] = _targetImage->GetY();
            break;
        case IRTK_I2S:
            inverted[2] = false;
            sliceNum[2] = _targetImage->GetY();
            break;
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_L2R:
            inverted[0] = true;
            sliceNum[0] = _targetImage->GetZ();
            break;
        case IRTK_R2L:
            inverted[0] = false;
            sliceNum[0] = _targetImage->GetZ();
            break;
        case IRTK_A2P:
            inverted[1] = true;
            sliceNum[1] = _targetImage->GetZ();
            break;
        case IRTK_P2A:
            inverted[1] = false;
            sliceNum[1] = _targetImage->GetZ();
            break;
        case IRTK_S2I:
            inverted[2] = true;
            sliceNum[2] = _targetImage->GetZ();
            break;
        case IRTK_I2S:
            inverted[2] = false;
            sliceNum[2] = _targetImage->GetZ();
            break;
        default:
            break;
        }
        break;
    default:
        qCritical("Not a valid type of viewer");
        exit(1);
        break;
    }
}

void irtkQtBaseViewer::InitializeOrientation() {
    double x[3], y[3], z[3];

    // Get original image orientation
    GetNeurologicalOrientation(x, y, z);

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
        qCritical("Not a valid type of viewer");
        exit(1);
        break;
    }
}

void irtkQtBaseViewer::GetNeurologicalOrientation(double *x, double *y, double *z) {
    int iaxis, jaxis, kaxis;
    double xaxis[3], yaxis[3], zaxis[3];

    // get original image orientation
    _targetImage->GetOrientation(xaxis, yaxis, zaxis);
    _targetImage->Orientation(iaxis, jaxis, kaxis);

    switch (iaxis) {
    case IRTK_L2R:
        x[0] = -xaxis[0];
        x[1] = -xaxis[1];
        x[2] = -xaxis[2];
        break;
    case IRTK_R2L:
        x[0] = xaxis[0];
        x[1] = xaxis[1];
        x[2] = xaxis[2];
        break;
    case IRTK_P2A:
        y[0] = xaxis[0];
        y[1] = xaxis[1];
        y[2] = xaxis[2];
        break;
    case IRTK_A2P:
        y[0] = -xaxis[0];
        y[1] = -xaxis[1];
        y[2] = -xaxis[2];
        break;
    case IRTK_I2S:
        z[0] = xaxis[0];
        z[1] = xaxis[1];
        z[2] = xaxis[2];
        break;
    case IRTK_S2I:
        z[0] = -xaxis[0];
        z[1] = -xaxis[1];
        z[2] = -xaxis[2];
        break;
    default:
        qCritical("Cannot work out x-orientation");
        break;
    }
    switch (jaxis) {
    case IRTK_L2R:
        x[0] = -yaxis[0];
        x[1] = -yaxis[1];
        x[2] = -yaxis[2];
        break;
    case IRTK_R2L:
        x[0] = yaxis[0];
        x[1] = yaxis[1];
        x[2] = yaxis[2];
        break;
    case IRTK_P2A:
        y[0] = yaxis[0];
        y[1] = yaxis[1];
        y[2] = yaxis[2];
        break;
    case IRTK_A2P:
        y[0] = -yaxis[0];
        y[1] = -yaxis[1];
        y[2] = -yaxis[2];
        break;
    case IRTK_I2S:
        z[0] = yaxis[0];
        z[1] = yaxis[1];
        z[2] = yaxis[2];
        break;
    case IRTK_S2I:
        z[0] = -yaxis[0];
        z[1] = -yaxis[1];
        z[2] = -yaxis[2];
        break;
    default:
        qCritical("Cannot work out y-orientation");
        break;
    }
    switch (kaxis) {
    case IRTK_L2R:
        x[0] = -zaxis[0];
        x[1] = -zaxis[1];
        x[2] = -zaxis[2];
        break;
    case IRTK_R2L:
        x[0] = zaxis[0];
        x[1] = zaxis[1];
        x[2] = zaxis[2];
        break;
    case IRTK_P2A:
        y[0] = zaxis[0];
        y[1] = zaxis[1];
        y[2] = zaxis[2];
        break;
    case IRTK_A2P:
        y[0] = -zaxis[0];
        y[1] = -zaxis[1];
        y[2] = -zaxis[2];
        break;
    case IRTK_I2S:
        z[0] = zaxis[0];
        z[1] = zaxis[1];
        z[2] = zaxis[2];
        break;
    case IRTK_S2I:
        z[0] = -zaxis[0];
        z[1] = -zaxis[1];
        z[2] = -zaxis[2];
        break;
    default:
        qCritical("Cannot work out z-orientation");
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
