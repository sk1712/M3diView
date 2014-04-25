#include <irtkQtTwoDimensionalViewer.h>

#include <time.h>
#include <QDebug>

irtkQtTwoDimensionalViewer::irtkQtTwoDimensionalViewer(irtkViewMode viewMode) {
    _viewMode = viewMode;
    ClearDisplayedImages();
}

irtkQtTwoDimensionalViewer::~irtkQtTwoDimensionalViewer() {    
    ClearDisplayedImages();
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

vector<QRgb*> irtkQtTwoDimensionalViewer::GetDrawable() {
    vector<QRgb*> allDrawables;

    for (unsigned int index = 0; index < _image.size(); index++) {
        QRgb *drawable = new QRgb[_imageOutput[index]->GetNumberOfVoxels()];
        QRgb _backgroundColor = qRgba(0, 0, 0, 0);

        irtkGreyPixel *original = _imageOutput[index]->GetPointerToVoxels();
        QRgb *drawn = drawable;

        int i, j;

        // Only display the target image
        for (j = 0; j < _height; j++) {
            for (i = 0; i < _width; i++) {
                if (*original >= 0) {
                    *drawn = _lookupTable[index]->lookupTable[*original];
                } else {
                    *drawn = _backgroundColor;
                }
                original++;
                drawn++;
            }
        }

        allDrawables.push_back(drawable);
    }

    return allDrawables;
}

void irtkQtTwoDimensionalViewer::GetLabels(char &top, char &bottom, char &left, char &right) {
    switch (_viewMode) {
    case VIEW_AXIAL :
        top = 'A';
        bottom = 'P';
        left = 'R';
        right = 'L';
        break;
    case VIEW_SAGITTAL :
        top = 'S';
        bottom = 'I';
        left = 'P';
        right = 'A';
        break;
    case VIEW_CORONAL :
        top = 'S';
        bottom = 'I';
        left = 'R';
        right = 'L';
        break;
    default :
        break;
    }
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

    // calculate the actual output images
    CalculateOutputImages(attr);
}

void irtkQtTwoDimensionalViewer::InitializeTransformation() {
    double _targetMin, _targetMax;

    for (unsigned int i = 0; i < _image.size(); i++) {
        _image[i]->GetMinMaxAsDouble(&_targetMin, &_targetMax);

        _transformFilter[i]->SetInput(_image[i]);
        _transformFilter[i]->PutScaleFactorAndOffset(255.0 / (_targetMax
            - _targetMin), -_targetMin * 255.0 / (_targetMax - _targetMin));
    }
}

void irtkQtTwoDimensionalViewer::AddToDisplayedImages(irtkQtImageObject *imageObject) {
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
//        if (!(_targetImage->GetImageAttributes() == newImage->GetImageAttributes())) {
//            delete newImage;
//            return;
//        }
    }

    // if everything is fine add to vectors
    AddToVectors(newImage);
    _lookupTable.push_back(new irtkQtLookupTable());
    _lookupTable.back()->SetAlpha(imageObject->GetOpacity());
}

void irtkQtTwoDimensionalViewer::ResizeImage(int width, int height) {
    SetDimensions(width, height);
    InitializeOutputImage();

    emit ImageResized(QVector<QRgb*>::fromStdVector(GetDrawable()));
}

void irtkQtTwoDimensionalViewer::ChangeSlice(int slice) {
    double originX, originY, originZ;

    irtkGreyImage *_targetImageOutput = _imageOutput[0];

    _targetImageOutput->GetOrigin(originX, originY, originZ);
    _targetImageOutput->WorldToImage(originX, originY, originZ);
    originZ += slice - GetCurrentSlice();
    _targetImageOutput->ImageToWorld(originX, originY, originZ);

    _targetImage->WorldToImage(originX, originY, originZ);
    originX = round(originX);
    originY = round(originY);
    originZ = round(originZ);
    _targetImage->ImageToWorld(originX, originY, originZ);

    emit OriginChanged(originX, originY, originZ);
}

void irtkQtTwoDimensionalViewer::ChangeOrigin(int x, int y) {
    double originX, originY, originZ;

    irtkGreyImage *_targetImageOutput = _imageOutput[0];

    originX = x;
    originY = _height - y;
    originZ = 0;

    _targetImageOutput->ImageToWorld(originX, originY, originZ);

    _targetImage->WorldToImage(originX, originY, originZ);
    originX = round(originX);
    originY = round(originY);
    originZ = round(originZ);
    _targetImage->ImageToWorld(originX, originY, originZ);

    emit OriginChanged(originX, originY, originZ);
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

void irtkQtTwoDimensionalViewer::AddToVectors(irtkImage* newImage) {
    _image.push_back(newImage);
    _imageOutput.push_back(new irtkGreyImage);
    _transform.push_back(new irtkAffineTransformation);
    _interpolator.push_back(new irtkNearestNeighborInterpolateImageFunction);

    irtkImageTransformation *transformation = new irtkImageTransformation;
    transformation->SetOutput(_imageOutput.back());
    transformation->SetTransformation(_transform.back());
    transformation->PutInterpolator(_interpolator.back());
    transformation->PutSourcePaddingValue(0);
    _transformFilter.push_back(transformation);
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

/// free function used to parallelize the transformations of images
void CalculateSingleTransform(irtkImageTransformation* &transform) {
    transform->PutSourcePaddingValue(-1);
    transform->Run();
}

void irtkQtTwoDimensionalViewer::CalculateOutputImages(irtkImageAttributes attr) {
    clock_t t, t2;

    t = clock();

    vector<irtkGreyImage *>::iterator it;
    for (it = _imageOutput.begin(); it != _imageOutput.end(); it++) {
        (*it)->Initialize(attr);
    }

    // apply transformation to every item in the container, modifying the items in-place
    QtConcurrent::blockingMap(_transformFilter, &CalculateSingleTransform);

    t2 = clock() - t;
    qDebug() << "Now running thread: " << QThread::currentThreadId()
             << " and it took me " << ((float)t2)/CLOCKS_PER_SEC
             << " seconds to calculate output images.\n";
}


