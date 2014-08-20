#include <irtkQtTwoDimensionalViewer.h>


irtkQtTwoDimensionalViewer::irtkQtTwoDimensionalViewer(irtkViewMode viewMode) {
    _viewMode = viewMode;

    currentSlice = new int[1];
    sliceNum = new int[1];

    inverted = new bool[1];

    ClearDisplayedImages();
}

irtkQtTwoDimensionalViewer::~irtkQtTwoDimensionalViewer() {    
    ClearDisplayedImages();
    delete currentSlice;
}

QRgb** irtkQtTwoDimensionalViewer::GetOnlyADrawable() {
    QRgb** drawable = new QRgb*[1];
    // Set background color to transparent black
    QRgb _backgroundColor = qRgba(0, 0, 0, 0);

    map<int, irtkGreyImage *>::iterator it = _imageOutput.begin();
    drawable[0] = new QRgb[it->second->GetNumberOfVoxels()];
    irtkGreyPixel *original = it->second->GetPointerToVoxels();
    QRgb *drawn = drawable[0];

    irtkQtLookupTable *luTable = _lookupTable[it->first];
    int i, j;

    // Create a drawable only for the first image
    for (j = 0; j < _height; j++) {
        for (i = 0; i < _width; i++) {
            if (*original >= 0) {
                *drawn = luTable->lookupTable[*original];
            } else {
                *drawn = _backgroundColor;
            }
            original++;
            drawn++;
        }
    }

    return drawable;
}

QRgb** irtkQtTwoDimensionalViewer::GetOnlyBDrawable() {
    QRgb** drawable = new QRgb*[1];
    // Set background color to transparent black
    QRgb _backgroundColor = qRgba(0, 0, 0, 0);

    map<int, irtkGreyImage *>::iterator it = --_imageOutput.end();
    drawable[0] = new QRgb[it->second->GetNumberOfVoxels()];
    irtkGreyPixel *original = it->second->GetPointerToVoxels();
    QRgb *drawn = drawable[0];

    irtkQtLookupTable *luTable = _lookupTable[it->first];
    int i, j;

    // Create a drawable only for the second image
    for (j = 0; j < _height; j++) {
        for (i = 0; i < _width; i++) {
            if (*original >= 0) {
                *drawn = luTable->lookupTable[*original];
            } else {
                *drawn = _backgroundColor;
            }
            original++;
            drawn++;
        }
    }

    return drawable;
}

QRgb** irtkQtTwoDimensionalViewer::GetHShutterDrawable() {
    QRgb** drawable = new QRgb*[1];
    // Set background color to transparent black
    QRgb _backgroundColor = qRgba(0, 0, 0, 0);

    map<int, irtkGreyImage *>::iterator it = _imageOutput.begin();
    drawable[0] = new QRgb[it->second->GetNumberOfVoxels()];

    irtkGreyPixel *target = it->second->GetPointerToVoxels();
    irtkQtLookupTable *targetTable = _lookupTable[it->first];

    irtkGreyPixel *source = (++it)->second->GetPointerToVoxels();
    irtkQtLookupTable *sourceTable = _lookupTable[it->first];

    QRgb *drawn = drawable[0];
    int i, j;

    // Display target and source images with a horizontal shutter
    for (j = 0; j < _height; j++) {
        if (j < _viewMix * _height) {
            for (i = 0; i < _width; i++) {
                if (*target >= 0) {
                    *drawn = targetTable->lookupTable[*target];
                } else {
                    *drawn = _backgroundColor;
                }
                target++;
                source++;
                drawn++;
            }
        } else {
            for (i = 0; i < _width; i++) {
                if (*source >= 0) {
                    *drawn = sourceTable->lookupTable[*source];
                } else {
                    *drawn = _backgroundColor;
                }
                target++;
                source++;
                drawn++;
            }
        }
    }

    return drawable;
}

QRgb** irtkQtTwoDimensionalViewer::GetVShutterDrawable() {
    QRgb** drawable = new QRgb*[1];
    // Set background color to transparent black
    QRgb _backgroundColor = qRgba(0, 0, 0, 0);

    map<int, irtkGreyImage *>::iterator it = _imageOutput.begin();
    drawable[0] = new QRgb[it->second->GetNumberOfVoxels()];

    irtkGreyPixel *target = it->second->GetPointerToVoxels();
    irtkQtLookupTable *targetTable = _lookupTable[it->first];

    irtkGreyPixel *source = (++it)->second->GetPointerToVoxels();
    irtkQtLookupTable *sourceTable = _lookupTable[it->first];

    QRgb *drawn = drawable[0];
    int i, j;

    // Display target and source images with a vertical shutter
    for (j = 0; j < _height; j++) {
        for (i = 0; i < _width; i++) {
            if (i < _viewMix * _width) {
                if (*target >= 0) {
                    *drawn = targetTable->lookupTable[*target];
                } else {
                    *drawn = _backgroundColor;
                }
            } else {
                if (*source >= 0) {
                    *drawn = sourceTable->lookupTable[*source];
                } else {
                    *drawn = _backgroundColor;
                }
            }
            target++;
            source++;
            drawn++;
        }
    }

    return drawable;
}

QRgb** irtkQtTwoDimensionalViewer::GetSubtractionDrawable() {
    QRgb** drawable = new QRgb*[1];
    // Set background color to transparent black
    QRgb _backgroundColor = qRgba(0, 0, 0, 0);

    double targetMin, targetMax;
    targetMin = _lookupTable.begin()->second->GetImageMinValue();
    targetMax = _lookupTable.begin()->second->GetImageMaxValue();

    double sourceMin, sourceMax;
    sourceMin = (--_lookupTable.end())->second->GetImageMinValue();
    sourceMax = (--_lookupTable.end())->second->GetImageMaxValue();

    double subtractionMin, subtractionMax;
    subtractionMin = targetMin - sourceMax;
    subtractionMax = targetMax - sourceMin;

    if (!subtractionLookupTable) {
        subtractionLookupTable = new irtkQtLookupTable;
        subtractionLookupTable->SetMinMaxImageValues(subtractionMin, subtractionMax);
        subtractionLookupTable->SetMinMaxDisplayValues(subtractionMin, subtractionMax);
        subtractionLookupTable->Initialize();
    }

    drawable[0] = new QRgb[_imageOutput.begin()->second->GetNumberOfVoxels()];
    QRgb *drawn = drawable[0];
    irtkGreyPixel *target = _imageOutput.begin()->second->GetPointerToVoxels();
    irtkGreyPixel *source = (--_imageOutput.end())->second->GetPointerToVoxels();

    int i, j;

    for (j = 0; j < _height; j++) {
        for (i = 0; i < _width; i++) {
            if ((*target >= 0) && (*source >= 0)) {
                *drawn = subtractionLookupTable->lookupTable[(*target - *source + 255) / 2];
            } else {
                *drawn = _backgroundColor;
            }
            target++;
            source++;
            drawn++;
        }
    }

    return drawable;
}

vector<QRgb**> irtkQtTwoDimensionalViewer::GetBlendDrawable() {
    vector<QRgb**> allDrawables;
    // Set background color to transparent black
    QRgb _backgroundColor = qRgba(0, 0, 0, 0);

    map<int, irtkGreyImage *>::iterator it;
    for (it = _imageOutput.begin(); it != _imageOutput.end(); ++it) {
        QRgb** drawable = new QRgb*[1];
        drawable[0] = new QRgb[it->second->GetNumberOfVoxels()];
        irtkGreyPixel *original = it->second->GetPointerToVoxels();
        QRgb *drawn = drawable[0];

        irtkQtLookupTable *luTable = _lookupTable[it->first];
        int i, j;

        // Create a drawable for all images
        for (j = 0; j < _height; j++) {
            for (i = 0; i < _width; i++) {
                if (*original >= 0) {
                    *drawn = luTable->lookupTable[*original];
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

void irtkQtTwoDimensionalViewer::SetInterpolationMethod(int index, irtkQtImageObject::irtkQtInterpolationMode mode) {
    delete _interpolator[index];

    switch (mode) {
    case irtkQtImageObject::INTERPOLATION_NN:
        _interpolator[index] = irtkInterpolateImageFunction::New(Interpolation_NN, _image[index]);
        qDebug("Change to interpolation NN");
        break;
    case irtkQtImageObject::INTERPOLATION_LINEAR:
        _interpolator[index] = irtkInterpolateImageFunction::New(Interpolation_Linear, _image[index]);
        qDebug("Change to interpolation linear");
        break;
    case irtkQtImageObject::INTERPOLATION_B_SPLINE:
        _interpolator[index] = irtkInterpolateImageFunction::New(Interpolation_BSpline, _image[index]);
        qDebug("Change to interpolation b-spline");
        break;
    case irtkQtImageObject::INTERPOLATION_C_SPLINE:
        _interpolator[index] = irtkInterpolateImageFunction::New(Interpolation_CSpline, _image[index]);
        qDebug("Change to interpolation c-spline");
        break;
    case irtkQtImageObject::INTERPOLATION_SINC:
        qDebug("Change to interpolation sinc");
        _interpolator[index] = irtkInterpolateImageFunction::New(Interpolation_Sinc, _image[index]);
        break;
    default:
        qCritical("Unknown interpolation option");
    }

    _transformFilter[index]->PutInterpolator(_interpolator[index]);

    // Calculate the new output image
    currentIndex = index;
    CalculateCurrentOutput();
}

///// free function used to parallelize the transformations of images
void CalculateSingleTransform(irtkImageTransformation* transform) {
    transform->PutSourcePaddingValue(-1);
    transform->Run();
}

void irtkQtTwoDimensionalViewer::CalculateOutputImages() {
    irtkImageAttributes attr = InitializeAttributes();

    map<int, irtkGreyImage *>::iterator it;
    for (it = _imageOutput.begin(); it != _imageOutput.end(); ++it) {
        it->second->Initialize(attr);
    }

    QFuture<void> *threads = new QFuture<void>[_transformFilter.size()];
    int t_index = 0;

    irtkImageTransformation* transformFilter;
    map<int, irtkImageTransformation *>::iterator trit;
    for (trit = _transformFilter.begin(); trit != _transformFilter.end(); ++trit) {
        transformFilter = trit->second;
        threads[t_index] = QtConcurrent::run(CalculateSingleTransform, transformFilter);
        t_index++;
    }

    for (int i = 0; i < t_index; i++) {
        threads[i].waitForFinished();
    }
    delete [] threads;
}

void irtkQtTwoDimensionalViewer::CalculateCurrentOutput() {
    irtkImageAttributes attr = InitializeAttributes();
    _imageOutput[currentIndex]->Initialize(attr);

    irtkImageTransformation* transformFilter = _transformFilter[currentIndex];
    transformFilter->PutSourcePaddingValue(-1);
    transformFilter->Run();
}

void irtkQtTwoDimensionalViewer::InitializeTransformation() {
    map<int, irtkImage*>::iterator it;
    for (it = _image.begin(); it != _image.end(); ++it) {
        currentIndex = it->first;
        InitializeCurrentTransformation();
    }
}

void irtkQtTwoDimensionalViewer::InitializeCurrentTransformation() {
    double _targetMin, _targetMax;

    _lookupTable[currentIndex]->GetMinMaxImageValues(_targetMin, _targetMax);

    irtkImageTransformation* transformFilter = _transformFilter[currentIndex];
    transformFilter->SetInput(_image[currentIndex]);
    transformFilter->PutScaleFactorAndOffset(255.0 / (_targetMax
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

    irtkQtBaseViewer::MoveImage(previousKey, newKey);
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
    // Set pixel size to 1x1mm
    if (_image.size() == 1) {
        SetResolution(1, 1, 1);
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
    double diff = slice[0] - *GetCurrentSlice();
    if (*inverted)
        diff *= -1;
    originZ += diff;
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

    // Changes the origin on image click (x, y widget coordinates)
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

    int iaxis, jaxis, kaxis;
    _targetImage->Orientation(iaxis, jaxis, kaxis);

    switch (_viewMode) {
    case VIEW_AXIAL :
        switch (iaxis) {
        case IRTK_I2S:
        case IRTK_S2I:
            *currentSlice = round2(x);
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_I2S:
        case IRTK_S2I:
            *currentSlice = round2(y);
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_I2S:
        case IRTK_S2I:
            *currentSlice = round2(z);
        default:
            break;
        }
        break;
    case VIEW_SAGITTAL :
        switch (iaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            *currentSlice = round2(x);
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            *currentSlice = round2(y);
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            *currentSlice = round2(z);
        default:
            break;
        }
        break;
    case VIEW_CORONAL :
        switch (iaxis) {
        case IRTK_P2A:
        case IRTK_A2P:
            *currentSlice = round2(x);
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_P2A:
        case IRTK_A2P:
            *currentSlice = round2(y);
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_P2A:
        case IRTK_A2P:
            *currentSlice = round2(z);
        default:
            break;
        }
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
