#include <irtkQtTwoDimensionalViewer.h>

#include <time.h>
#include <QDebug>

irtkQtTwoDimensionalViewer::irtkQtTwoDimensionalViewer(irtkViewMode viewMode) {
    ClearDisplayedImages();
    _viewMode = viewMode;
    currentSlice = new int[1];
    sliceNum = new int[1];
}

irtkQtTwoDimensionalViewer::~irtkQtTwoDimensionalViewer() {    
    ClearDisplayedImages();
    delete currentSlice;
}

void irtkQtTwoDimensionalViewer::UpdateCurrentSlice() {
    double x, y, z;

    x = _originX;
    y = _originY;
    z = _originZ;

    _targetImage->WorldToImage(x, y, z);

    switch (_viewMode) {
    case VIEW_AXIAL :
        *currentSlice = (int) round(z);
        break;
    case VIEW_SAGITTAL :
        *currentSlice = (int) round(x);
        break;
    case VIEW_CORONAL :
        *currentSlice = (int) round(y);
        break;
    default:
        currentSlice = 0;
        break;
    }
}

vector<QRgb**> irtkQtTwoDimensionalViewer::GetDrawable() {
    vector<QRgb**> allDrawables;
    QRgb _backgroundColor = qRgba(0, 0, 0, 0);

    map<int, irtkGreyImage *>::iterator it;
    for (it = _imageOutput.begin(); it != _imageOutput.end(); ++it) {
        QRgb** drawable = new QRgb*[1];
        drawable[0] = new QRgb[it->second->GetNumberOfVoxels()];
        irtkGreyPixel *original = it->second->GetPointerToVoxels();
        QRgb *drawn = drawable[0];

        int i, j;

        // Create a drawable for all images
        for (j = 0; j < _height; j++) {
            for (i = 0; i < _width; i++) {
                if (*original >= 0) {
                    *drawn = _lookupTable[it->first]->lookupTable[*original];
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

string irtkQtTwoDimensionalViewer::GetObjectName() {
    string objectName = "";

    switch (_viewMode) {
    case VIEW_AXIAL :
        objectName = "axial";
        break;
    case VIEW_SAGITTAL :
        objectName = "sagittal";
        break;
    case VIEW_CORONAL :
        objectName = "coronal";
        break;
    default :
        break;
    }

    return objectName;
}

void irtkQtTwoDimensionalViewer::InitializeTransformation() {
    double _targetMin, _targetMax;

    map<int, irtkImage*>::iterator it;
    for (it = _image.begin(); it != _image.end(); it++) {
        it->second->GetMinMaxAsDouble(&_targetMin, &_targetMax);

        _transformFilter[it->first]->SetInput(it->second);
        _transformFilter[it->first]->PutScaleFactorAndOffset(255.0 / (_targetMax
            - _targetMin), -_targetMin * 255.0 / (_targetMax - _targetMin));
    }
}

void irtkQtTwoDimensionalViewer::AddToDisplayedImages(irtkQtImageObject *imageObject, int index) {
    irtkQtBaseViewer::AddToDisplayedImages(imageObject, index);
    // set pixel size to 1x1mm
    if (_image.size() == 1) {
        SetResolution(1, 1, _targetImage->GetZSize());
    }
}

void irtkQtTwoDimensionalViewer::ResizeImage(int width, int height) {
    SetDimensions(width, height);
    CalculateOutputImages();

    emit ImageResized(QVector<QRgb**>::fromStdVector(GetDrawable()));
}

void irtkQtTwoDimensionalViewer::ChangeSlice(int* slice) {
    double originX, originY, originZ;

    irtkGreyImage *_targetImageOutput = _imageOutput.begin()->second;

    _targetImageOutput->GetOrigin(originX, originY, originZ);
    _targetImageOutput->WorldToImage(originX, originY, originZ);
    originZ += slice[0] - *GetCurrentSlice();
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

    irtkGreyImage *_targetImageOutput = _imageOutput.begin()->second;

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

void irtkQtTwoDimensionalViewer::AddToMaps(irtkImage* newImage, int index) {
    _image.insert(pair<int, irtkImage *> (index, newImage));
    _imageOutput.insert(pair<int, irtkGreyImage *> (index, new irtkGreyImage));
    _transform.insert(pair<int, irtkTransformation *> (index, new irtkAffineTransformation));
    _interpolator.insert(pair<int, irtkImageFunction *> (index,
                                                         new irtkNearestNeighborInterpolateImageFunction));

    irtkImageTransformation *transformation = new irtkImageTransformation;
    transformation->SetOutput(_imageOutput[index]);
    transformation->SetTransformation(_transform[index]);
    transformation->PutInterpolator(_interpolator[index]);
    transformation->PutSourcePaddingValue(0);
    _transformFilter.insert(pair<int, irtkImageTransformation *> (index, transformation));
}

/// free function used to parallelize the transformations of images
void CalculateSingleTransform(irtkImageTransformation* &transform) {
    transform->PutSourcePaddingValue(-1);
    transform->Run();
}

void irtkQtTwoDimensionalViewer::CalculateOutputImages() {
//    clock_t t, t2;

//    t = clock();
    irtkImageAttributes attr = InitializeAttributes();

    map<int, irtkGreyImage *>::iterator it;
    for (it = _imageOutput.begin(); it != _imageOutput.end(); it++) {
        it->second->Initialize(attr);
    }

    map<int, irtkImageTransformation *>::iterator trit;
    for (trit = _transformFilter.begin(); trit != _transformFilter.end(); trit++) {
        trit->second->PutSourcePaddingValue(-1);
        trit->second->Run();
    }

//    t2 = clock() - t;
//    qDebug() << "Now running thread: " << QThread::currentThreadId()
//             << " and it took me " << ((float)t2)/CLOCKS_PER_SEC
//             << " seconds to calculate output images.\n";
}


