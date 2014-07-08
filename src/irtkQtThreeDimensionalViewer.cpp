#include <irtkQtThreeDimensionalViewer.h>


irtkQtThreeDimensionalViewer::irtkQtThreeDimensionalViewer() {
    ClearDisplayedImages();

    _viewMode = VIEW_3D;

    currentSlice = new int[3];
    sliceNum = new int[3];

    inverted = new bool[3];
}

irtkQtThreeDimensionalViewer::~irtkQtThreeDimensionalViewer() {
    ClearDisplayedImages();
    delete [] currentSlice;
}

vector<QRgb**> irtkQtThreeDimensionalViewer::GetDrawable() {
    vector<QRgb**> allDrawables;
    QRgb _backgroundColor = qRgba(0, 0, 0, 0);

    int dimensions[3][2] = {
        {sliceNum[0], sliceNum[1]},
        {sliceNum[1], sliceNum[2]},
        {sliceNum[0], sliceNum[2]}
    };

    map<int, irtkGreyImage **>::iterator it;
    for (it = _imageOutput.begin(); it != _imageOutput.end(); ++it) {
        QRgb** drawable = new QRgb*[3];

        for (int dim = 0; dim < 3; dim++) {
            drawable[dim] = new QRgb[it->second[dim]->GetNumberOfVoxels()];
            irtkGreyPixel *original = it->second[dim]->GetPointerToVoxels();
            QRgb *drawn = drawable[dim];

            irtkQtLookupTable *luTable = _lookupTable[it->first];
            int i, j;

            for (j = 0; j < dimensions[dim][0]; j++) {
                for (i = 0; i < dimensions[dim][1]; i++) {
                    if (*original >= 0) {
                        *drawn = luTable->lookupTable[*original];
                    } else {
                        *drawn = _backgroundColor;
                    }
                    original++;
                    drawn++;
                }
            }
        }

        allDrawables.push_back(drawable);
    }

    return allDrawables;
}

void irtkQtThreeDimensionalViewer::InitializeTransformation() {
    map<int, irtkImage*>::iterator it;
    for (it = _image.begin(); it != _image.end(); it++) {
        currentIndex = it->first;
        InitializeCurrentTransformation();
    }
}

void irtkQtThreeDimensionalViewer::InitializeCurrentTransformation() {
    double _targetMin, _targetMax;

    _lookupTable[currentIndex]->GetMinMaxImageValues(_targetMin, _targetMax);

    for (int dim = 0; dim < 3; dim++) {
        _transformFilter[currentIndex][dim]->SetInput(_image[currentIndex]);
        _transformFilter[currentIndex][dim]->PutScaleFactorAndOffset(255.0 / (_targetMax
            - _targetMin), -_targetMin * 255.0 / (_targetMax - _targetMin));
    }
}

///// free function used to parallelize the transformations of images
void CalculateSingleTransform(irtkImageTransformation** transform) {
    for (int i = 0; i < 3; i++) {
        transform[i]->PutSourcePaddingValue(-1);
        transform[i]->Run();
    }
}

void irtkQtThreeDimensionalViewer::SetInterpolationMethod(int index,
                                                          irtkQtImageObject::irtkQtInterpolationMode mode) {
    for (int dim = 0; dim < 3; dim++)
        delete _interpolator[index][dim];

    switch (mode) {
    case irtkQtImageObject::INTERPOLATION_NN:
        for (int dim = 0; dim < 3; dim++)
            _interpolator[index][dim] =
                    irtkInterpolateImageFunction::New(Interpolation_NN, _image[index]);
        break;
    case irtkQtImageObject::INTERPOLATION_LINEAR:
        for (int dim = 0; dim < 3; dim++)
            _interpolator[index][dim] =
                    irtkInterpolateImageFunction::New(Interpolation_Linear, _image[index]);
        break;
    case irtkQtImageObject::INTERPOLATION_B_SPLINE:
        for (int dim = 0; dim < 3; dim++)
            _interpolator[index][dim] =
                    irtkInterpolateImageFunction::New(Interpolation_BSpline, _image[index]);
        break;
    case irtkQtImageObject::INTERPOLATION_C_SPLINE:
        for (int dim = 0; dim < 3; dim++)
            _interpolator[index][dim] =
                    irtkInterpolateImageFunction::New(Interpolation_CSpline, _image[index]);
        break;
    case irtkQtImageObject::INTERPOLATION_SINC:
        for (int dim = 0; dim < 3; dim++)
            _interpolator[index][dim] =
                    irtkInterpolateImageFunction::New(Interpolation_Sinc, _image[index]);
        break;
    default:
        qCritical("Unknown interpolation option");
    }

    for (int dim = 0; dim < 3; dim++)
        _transformFilter[index][dim]->PutInterpolator(_interpolator[index][dim]);

    // Calculate the new output image
    currentIndex = index;
    CalculateCurrentOutput();
}

void irtkQtThreeDimensionalViewer::CalculateOutputImages() {
    irtkImageAttributes attr[3];
    // Width of output image
    int width[3] = {sliceNum[0], sliceNum[1], sliceNum[0]};
    // Height of output image
    int height[3] = {sliceNum[1], sliceNum[2], sliceNum[2]};

    // Store a backup of current origin
    double originX_backup = _originX, originY_backup = _originY, originZ_backup = _originZ;

    for (int i = 0; i < 3; i++) {
        _width = width[i]; _height = height[i];
        SetOrientation(i);
        ChangeViewSlice(i);
        attr[i] = InitializeAttributes();

        map<int, irtkGreyImage **>::iterator image_it;
        for (image_it = _imageOutput.begin(); image_it != _imageOutput.end(); image_it++) {
            image_it->second[i]->Initialize(attr[i]);
        }
    }

    // Run the transformation for the different images in parallel
    QFuture<void> *threads = new QFuture<void>[_transformFilter.size()];
    int t_index = 0;

    irtkImageTransformation** transformFilter;
    map<int, irtkImageTransformation **>::iterator trans_it;
    for (trans_it = _transformFilter.begin(); trans_it != _transformFilter.end(); trans_it++) {
        transformFilter = trans_it->second;
        threads[t_index] = QtConcurrent::run(CalculateSingleTransform, transformFilter);
        t_index++;
    }

    for (int i = 0; i < t_index; i++) {
        threads[i].waitForFinished();
    }

    delete [] threads;

    // Restore current origin
    _originX = originX_backup;
    _originY = originY_backup;
    _originZ = originZ_backup;
}

void irtkQtThreeDimensionalViewer::CalculateCurrentOutput() {
    irtkImageAttributes attr[3];
    // Width of output image
    int width[3] = {sliceNum[0], sliceNum[1], sliceNum[0]};
    // Height of output image
    int height[3] = {sliceNum[1], sliceNum[2], sliceNum[2]};

    // Store a backup of current origin
    double originX_backup = _originX, originY_backup = _originY, originZ_backup = _originZ;

    for (int i = 0; i < 3; i++) {
        _width = width[i]; _height = height[i];
        SetOrientation(i);
        ChangeViewSlice(i);
        attr[i] = InitializeAttributes();

        _imageOutput[currentIndex][i]->Initialize(attr[i]);
        _transformFilter[currentIndex][i]->PutSourcePaddingValue(-1);
        _transformFilter[currentIndex][i]->Run();
    }

    _originX = originX_backup;
    _originY = originY_backup;
    _originZ = originZ_backup;
}

void irtkQtThreeDimensionalViewer::DeleteSingleImage(int index) {
    _image.erase(index);
    _lookupTable.erase(index);

    delete [] _imageOutput[index];
    _imageOutput.erase(index);

    delete [] _transform[index];
    _transform.erase(index);

    delete [] _interpolator[index];
    _interpolator.erase(index);

    delete [] _transformFilter[index];
    _transformFilter.erase(index);

    irtkQtBaseViewer::DeleteSingleImage(index);
}

void irtkQtThreeDimensionalViewer::MoveImage(int previousKey, int newKey) {
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

void irtkQtThreeDimensionalViewer::UpdateKeysAfterIndexDeleted(int index) {
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_image, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_imageOutput, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_lookupTable, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_transform, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_interpolator, index);
    irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(_transformFilter, index);
}

void irtkQtThreeDimensionalViewer::ResizeImage(int width, int height) {

}

void irtkQtThreeDimensionalViewer::ChangeSlice(int* slice) {
    double originX, originY, originZ;

    originX = slice[0];
    originY = slice[1];
    originZ = slice[2];

    int iaxis, jaxis, kaxis;
    _targetImage->Orientation(iaxis, jaxis, kaxis);

    switch (iaxis) {
    case IRTK_L2R:
    case IRTK_R2L:
        originX = slice[0];
        break;
    case IRTK_A2P:
    case IRTK_P2A:
        originX = slice[1];
        break;
    case IRTK_S2I:
    case IRTK_I2S:
        originX = slice[2];
        break;
    default:
        break;
    }
    switch (jaxis) {
    case IRTK_L2R:
    case IRTK_R2L:
        originY = slice[0];
        break;
    case IRTK_A2P:
    case IRTK_P2A:
        originY = slice[1];
        break;
    case IRTK_S2I:
    case IRTK_I2S:
        originY = slice[2];
        break;
    default:
        break;
    }
    switch (kaxis) {
    case IRTK_L2R:
    case IRTK_R2L:
        originZ = slice[0];
        break;
    case IRTK_A2P:
    case IRTK_P2A:
        originZ = slice[1];
        break;
    case IRTK_S2I:
    case IRTK_I2S:
        originZ = slice[2];
        break;
    default:
        break;
    }

    _targetImage->ImageToWorld(originX, originY, originZ);

    emit OriginChanged(originX, originY, originZ);
}

void irtkQtThreeDimensionalViewer::ChangeOrigin(int x, int y) {

}

void irtkQtThreeDimensionalViewer::UpdateCurrentSlice() {
    double x, y, z;

    x = _originX;
    y = _originY;
    z = _originZ;
    _targetImage->WorldToImage(x, y, z);

    int iaxis, jaxis, kaxis;
    _targetImage->Orientation(iaxis, jaxis, kaxis);

    switch (iaxis) {
    case IRTK_L2R:
    case IRTK_R2L:
        currentSlice[0] = round2(x);
        break;
    case IRTK_P2A:
    case IRTK_A2P:
        currentSlice[1] = round2(x);
        break;
    case IRTK_I2S:
    case IRTK_S2I:
        currentSlice[2] = round2(x);
        break;
    default:
        break;
    }
    switch (jaxis) {
    case IRTK_L2R:
    case IRTK_R2L:
        currentSlice[0] = round2(y);
        break;
    case IRTK_P2A:
    case IRTK_A2P:
        currentSlice[1] = round2(y);
        break;
    case IRTK_I2S:
    case IRTK_S2I:
        currentSlice[2] = round2(y);
        break;
    default:
        break;
    }
    switch (kaxis) {
    case IRTK_L2R:
    case IRTK_R2L:
        currentSlice[0] = round2(z);
        break;
    case IRTK_P2A:
    case IRTK_A2P:
        currentSlice[1] = round2(z);
        break;
    case IRTK_I2S:
    case IRTK_S2I:
        currentSlice[2] = round2(z);
        break;
    default:
        break;
    }
}

void irtkQtThreeDimensionalViewer::AddToMaps(irtkImage* newImage, int index) {
    _image.insert(pair<int, irtkImage *> (index, newImage));

    _imageOutput.insert(pair<int, irtkGreyImage **> (index, new irtkGreyImage*[3]));
    _transform.insert(pair<int, irtkTransformation **> (index, new irtkTransformation*[3]));
    _interpolator.insert(pair<int, irtkImageFunction **> (index, new irtkImageFunction*[3]));
    _transformFilter.insert(pair<int, irtkImageTransformation **> (index, new irtkImageTransformation*[3]));

    for (int dim = 0; dim < 3; dim++) {
        _imageOutput[index][dim] = new irtkGreyImage;
        _transform[index][dim] = new irtkAffineTransformation;
        _interpolator[index][dim] = new irtkNearestNeighborInterpolateImageFunction;

        irtkImageTransformation *transformation = new irtkImageTransformation;
        transformation->SetOutput(_imageOutput[index][dim]);
        transformation->SetTransformation(_transform[index][dim]);
        transformation->PutInterpolator(_interpolator[index][dim]);
        transformation->PutSourcePaddingValue(0);
        _transformFilter[index][dim] = transformation;
    }
}

void irtkQtThreeDimensionalViewer::SetOrientation(int view) {
    double x[3], y[3], z[3];

    GetNeurologicalOrientation(x, y, z);

    switch (view) {
    case VIEW_AXIAL :
        irtkQtBaseViewer::SetOrientation(x, y, z);
        break;
    case VIEW_SAGITTAL :
        irtkQtBaseViewer::SetOrientation(y, z, x);
        break;
    case VIEW_CORONAL :
        irtkQtBaseViewer::SetOrientation(x, z, y);
        break;
    default:
        qCritical("Not a valid type of two dimensional view");
        exit(1);
        break;
    }
}

void irtkQtThreeDimensionalViewer::ChangeViewSlice(int view) {
    double originX, originY, originZ;

    _targetImage->GetOrigin(originX, originY, originZ);
    _targetImage->WorldToImage(originX, originY, originZ);

    int iaxis, jaxis, kaxis;
    _targetImage->Orientation(iaxis, jaxis, kaxis);

    switch (view) {
    case VIEW_AXIAL :
        switch (iaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            _dx = _targetImage->GetXSize();
            break;
        case IRTK_P2A:
        case IRTK_A2P:
            _dy = _targetImage->GetXSize();
            break;
        case IRTK_I2S:
        case IRTK_S2I:
            originX = currentSlice[2];
            break;
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            _dx = _targetImage->GetYSize();
            break;
        case IRTK_P2A:
        case IRTK_A2P:
            _dy = _targetImage->GetYSize();
            break;
        case IRTK_I2S:
        case IRTK_S2I:
            originY = currentSlice[2];
            break;
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            _dx = _targetImage->GetZSize();
            break;
        case IRTK_P2A:
        case IRTK_A2P:
            _dy = _targetImage->GetZSize();
            break;
        case IRTK_I2S:
        case IRTK_S2I:
            originZ = currentSlice[2];
        default:
            break;
        }
        _dz = 1;
        break;
    case VIEW_SAGITTAL :
        switch (iaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            originX = currentSlice[0];
            break;
        case IRTK_P2A:
        case IRTK_A2P:
            _dx = _targetImage->GetXSize();
            break;
        case IRTK_I2S:
        case IRTK_S2I:
            _dy = _targetImage->GetXSize();
            break;
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            originY = currentSlice[0];
            break;
        case IRTK_P2A:
        case IRTK_A2P:
            _dx = _targetImage->GetYSize();
            break;
        case IRTK_I2S:
        case IRTK_S2I:
            _dy = _targetImage->GetYSize();
            break;
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            originZ = currentSlice[0];
            break;
        case IRTK_P2A:
        case IRTK_A2P:
            _dx = _targetImage->GetZSize();
            break;
        case IRTK_I2S:
        case IRTK_S2I:
            _dy = _targetImage->GetZSize();
        default:
            break;
        }
        _dz = 1;
        break;
    case VIEW_CORONAL :
        switch (iaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            _dx = _targetImage->GetXSize();
            break;
        case IRTK_P2A:
        case IRTK_A2P:
            originX = currentSlice[1];
            break;
        case IRTK_I2S:
        case IRTK_S2I:
            _dy = _targetImage->GetXSize();
            break;
        default:
            break;
        }
        switch (jaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            _dx = _targetImage->GetYSize();
            break;
        case IRTK_P2A:
        case IRTK_A2P:
            originY = currentSlice[1];
            break;
        case IRTK_I2S:
        case IRTK_S2I:
            _dy = _targetImage->GetYSize();
            break;
        default:
            break;
        }
        switch (kaxis) {
        case IRTK_L2R:
        case IRTK_R2L:
            _dx = _targetImage->GetZSize();
            break;
        case IRTK_P2A:
        case IRTK_A2P:
            originZ = currentSlice[1];
            break;
        case IRTK_I2S:
        case IRTK_S2I:
            _dy = _targetImage->GetZSize();
        default:
            break;
        }
        _dz = 1;
        break;
    default:
        qCritical("Not a valid type of two dimensional viewer");
        exit(1);
        break;
    }

    originX = round(originX);
    originY = round(originY);
    originZ = round(originZ);
    _targetImage->ImageToWorld(originX, originY, originZ);

    _originX = originX;
    _originY = originY;
    _originZ = originZ;
}
