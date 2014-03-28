#include <irtkQtTwoDimensionalViewer.h>

irtkQtTwoDimensionalViewer::irtkQtTwoDimensionalViewer(irtkViewMode viewMode) {
    _viewMode = viewMode;

    _targetImageOutput = new irtkGreyImage;
    _targetTransform = new irtkAffineTransformation;
    _targetInterpolator = new irtkNearestNeighborInterpolateImageFunction;
    _targetLookupTable = new irtkLookupTable;
    _targetTransformFilter = NULL;
}

irtkQtTwoDimensionalViewer::~irtkQtTwoDimensionalViewer() {
    delete _targetImageOutput;
    delete _targetTransform;
    delete _targetInterpolator;
    delete _targetLookupTable;
    delete _targetTransformFilter;
}

int irtkQtTwoDimensionalViewer::GetCurrentSlice() {
    double x, y, z;

    x = _originX;
    y = _originY;
    z = _originZ;

    _targetImage->WorldToImage(x, y, z);

    int result;

    switch (_viewMode) {
    case VIEW_AXIAL :
        result = (int) round(z);
        break;
    case VIEW_SAGITTAL :
        result = (int) round(x);
        break;
    case VIEW_CORONAL :
        result = (int) round(y);
        break;
    default:
        result = 0;
        break;
    }

    return result;
}

void irtkQtTwoDimensionalViewer::InitializeOutputImage() {
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

    _targetImageOutput->Initialize(attr);

    // calculate the actual output image
    CalculateOutputImage();
}

void irtkQtTwoDimensionalViewer::InitializeTransformation() {
    double _targetMin, _targetMax;
    _targetImage->GetMinMaxAsDouble(&_targetMin, &_targetMax);

   _targetTransformFilter = irtkImageTransformation::New(_targetTransform);
   _targetTransformFilter->SetInput(_targetImage);
   _targetTransformFilter->SetOutput(_targetImageOutput);
   _targetTransformFilter->SetTransformation(_targetTransform);
   _targetTransformFilter->PutInterpolator(_targetInterpolator);
   _targetTransformFilter->PutScaleFactorAndOffset(10000.0 / (_targetMax
   - _targetMin), -_targetMin * 10000.0 / (_targetMax - _targetMin));

   printf("initialized transformation \n");
}

void irtkQtTwoDimensionalViewer::ResizeImage(int width, int height) {
    printf("in resize event %d, %d\n", width, height);

    SetDimensions(width, height);
    InitializeOutputImage();
    emit ImageResized(GetDrawable());
}

void irtkQtTwoDimensionalViewer::ChangeSlice(int slice) {
    double x, y, z;

    _targetImageOutput->GetOrigin(x, y, z);
    printf("current origin %f, %f, %f \n", x, y, z);
    _targetImageOutput->WorldToImage(x, y, z);
    z += slice - GetCurrentSlice();
    printf("new slice is %f \n", z);
    _targetImageOutput->ImageToWorld(x, y, z);

    _targetImage->WorldToImage(x, y, z);
    x = round(x);
    y = round(y);
    z = round(z);
    _targetImage->ImageToWorld(x, y, z);

    printf("sending new origin %f, %f, %f \n", x, y, z);
    emit OriginChanged(x, y, z);
}

void irtkQtTwoDimensionalViewer::InitializeOriginOrientation() {
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

void irtkQtTwoDimensionalViewer::SetOrientation(const double * xaxis, const double * yaxis, const double * zaxis) {
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

void irtkQtTwoDimensionalViewer::CalculateOutputImage() {
   _targetTransformFilter->PutSourcePaddingValue(-1);
   _targetTransformFilter->Run();

   printf("ran transformation \n");
}

irtkColor* irtkQtTwoDimensionalViewer::GetDrawable() {
    irtkColor *drawable = new irtkColor[_targetImageOutput->GetNumberOfVoxels()];
    irtkColor _backgroundColor = irtkColor();

    irtkGreyPixel *original = _targetImageOutput->GetPointerToVoxels();
    irtkColor *drawn = drawable;

    int i, j;

    // Only display the target image
    for (j = 0; j < _height; j++) {
        for (i = 0; i < _width; i++) {
            if (*original >= 0) {
                *drawn = _targetLookupTable->lookupTable[*original];
            } else {
                *drawn = _backgroundColor;
            }
            original++;
            drawn++;
        }
    }

    return drawable;
}

