#include <irtkQtTwoDimensionalViewer.h>

//#include <time.h>
//#include <QDebug>


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

vector<QRgb**> irtkQtTwoDimensionalViewer::GetDrawable() {
    vector<QRgb**> allDrawables;
    // set background color to transparent black
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

void irtkQtTwoDimensionalViewer::CalculateCurrentOutput() {
    irtkImageAttributes attr = InitializeAttributes();
    _imageOutput[currentIndex]->Initialize(attr);

    _transformFilter[currentIndex]->PutSourcePaddingValue(-1);
    _transformFilter[currentIndex]->Run();
}

void irtkQtTwoDimensionalViewer::InitializeTransformation() {
    map<int, irtkImage*>::iterator it;
    for (it = _image.begin(); it != _image.end(); it++) {
        currentIndex = it->first;
        InitializeCurrentTransformation();
    }
}

void irtkQtTwoDimensionalViewer::InitializeCurrentTransformation() {
    double _targetMin, _targetMax;

    _image[currentIndex]->GetMinMaxAsDouble(&_targetMin, &_targetMax);

    _transformFilter[currentIndex]->SetInput(_image[currentIndex]);
    _transformFilter[currentIndex]->PutScaleFactorAndOffset(255.0 / (_targetMax
        - _targetMin), -_targetMin * 255.0 / (_targetMax - _targetMin));
}

void irtkQtTwoDimensionalViewer::MoveImage(int previousKey, int newKey) {
    if (newKey < previousKey) {
        MoveImageUp(_image, previousKey, newKey);
        MoveImageUp(_imageOutput, previousKey, newKey);
        MoveImageUp(_lookupTable, previousKey, newKey);
        MoveImageUp(_transform, previousKey, newKey);
        MoveImageUp(_interpolator, previousKey, newKey);
        MoveImageUp(_transformFilter, previousKey, newKey);
    }
    else if (newKey > previousKey){
        MoveImageDown(_image, previousKey, newKey);
        MoveImageDown(_imageOutput, previousKey, newKey);
        MoveImageDown(_lookupTable, previousKey, newKey);
        MoveImageDown(_transform, previousKey, newKey);
        MoveImageDown(_interpolator, previousKey, newKey);
        MoveImageDown(_transformFilter, previousKey, newKey);
    }
}

void irtkQtTwoDimensionalViewer::UpdateKeysAfterIndexDeleted(int index) {
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_image, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_imageOutput, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_lookupTable, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_transform, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_interpolator, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_transformFilter, index);
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

void irtkQtTwoDimensionalViewer::AddToDisplayedImages(irtkQtImageObject *imageObject, int index) {
    irtkQtBaseViewer::AddToDisplayedImages(imageObject, index);
    // set pixel size to 1x1mm
    if (_image.size() == 1) {
        SetResolution(1, 1, _targetImage->GetZSize());
    }
}

void irtkQtTwoDimensionalViewer::DeleteSingleImage(int index) {
    _image.erase(index);
    _lookupTable.erase(index);

    delete _imageOutput[index];
    _imageOutput.erase(index);

    delete _transform[index];
    _transform.erase(index);

    delete _interpolator[index];
    _interpolator.erase(index);

    delete _transformFilter[index];
    _transformFilter.erase(index);

    irtkQtBaseViewer::DeleteSingleImage(index);
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

///// free function used to parallelize the transformations of images
//void CalculateSingleTransform(irtkImageTransformation* &transform) {
//    transform->PutSourcePaddingValue(-1);
//    transform->Run();
//}




