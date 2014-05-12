#include <irtkQtThreeDimensionalViewer.h>

irtkQtThreeDimensionalViewer::irtkQtThreeDimensionalViewer() {
    ClearDisplayedImages();

    _viewMode = VIEW_3D;
    currentSlice = new int[3];
    sliceNum = new int[3];

    // initialize all previous slices to 0
    for (int i = 0; i < 3; i++) previousSlice[i] = 0;
}

irtkQtThreeDimensionalViewer::~irtkQtThreeDimensionalViewer() {
    ClearDisplayedImages();
    delete [] currentSlice;
}

vector<QRgb**> irtkQtThreeDimensionalViewer::GetDrawable() {
    vector<QRgb**> allDrawables;
    QRgb _backgroundColor = qRgba(0, 0, 0, 1);

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

            int i, j;

            for (j = 0; j < dimensions[dim][0]; j++) {
                for (i = 0; i < dimensions[dim][1]; i++) {
                    if (*original >= 0) {
                        *drawn = _lookupTable[it->first]->lookupTable[*original];
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

    _image[currentIndex]->GetMinMaxAsDouble(&_targetMin, &_targetMax);
    _lookupTable[currentIndex]->SetMinMaxImageValues(_targetMin, _targetMax);

    for (int dim = 0; dim < 3; dim++) {
        _transformFilter[currentIndex][dim]->SetInput(_image[currentIndex]);
        _transformFilter[currentIndex][dim]->PutScaleFactorAndOffset(255.0 / (_targetMax
            - _targetMin), -_targetMin * 255.0 / (_targetMax - _targetMin));
    }
}

void irtkQtThreeDimensionalViewer::CalculateOutputImages() {
    irtkImageAttributes attr[3];
    // width of output image
    int width[3] = {sliceNum[0], sliceNum[1], sliceNum[0]};
    // height of output image
    int height[3] = {sliceNum[1], sliceNum[2], sliceNum[2]};
    // index of corresponding slice for each view
    int index[3] = {2, 0, 1};

    // store a backup of current origin
    double originX_backup = _originX, originY_backup = _originY, originZ_backup = _originZ;

    for (int i = 0; i < 3; i++) {
        if (previousSlice[index[i]] != currentSlice[index[i]]) {
            _width = width[i]; _height = height[i];
            SetOrientation(i);
            ChangeViewSlice(i);
            attr[i] = InitializeAttributes();

            map<int, irtkGreyImage **>::iterator image_it;
            for (image_it = _imageOutput.begin(); image_it != _imageOutput.end(); image_it++) {
                image_it->second[i]->Initialize(attr[i]);
            }

            map<int, irtkImageTransformation **>::iterator trans_it;
            for (trans_it = _transformFilter.begin(); trans_it != _transformFilter.end(); trans_it++) {
                trans_it->second[i]->PutSourcePaddingValue(-1);
                trans_it->second[i]->Run();
            }
        }
    }

    _originX = originX_backup;
    _originY = originY_backup;
    _originZ = originZ_backup;
    //QtConcurrent::blockingMap(_transformFilter, &CalculateSingleTransform);
}

void irtkQtThreeDimensionalViewer::CalculateCurrentOutput() {
    irtkImageAttributes attr[3];
    // width of output image
    int width[3] = {sliceNum[0], sliceNum[1], sliceNum[0]};
    // height of output image
    int height[3] = {sliceNum[1], sliceNum[2], sliceNum[2]};

    // store a backup of current origin
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
    delete _image[index];
    _image.erase(index);

    delete [] _imageOutput[index];
    _imageOutput.erase(index);

    delete _lookupTable[index];
    _lookupTable.erase(index);

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

    for (int i = 0; i < 3; i++) previousSlice[i] = currentSlice[i];

    currentSlice[0] = (int) round(x);
    currentSlice[1] = (int) round(y);
    currentSlice[2] = (int) round(z);
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

    for (int i = 0; i < 3; i++) previousSlice[i] = 0;
}

//void CalculateSingleTransform(irtkImageTransformation** &transform) {
//    for (int i = 0; i < 3; i++) {
//        transform[i]->PutSourcePaddingValue(-1);
//        transform[i]->Run();
//    }
//}

void irtkQtThreeDimensionalViewer::SetOrientation(int view) {
    double x[3], y[3], z[3];

    _targetImage->GetOrientation(x, y, z);

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
        cerr << "Not a valid type of two dimensional viewer" << endl;
        exit(1);
        break;
    }
}

void irtkQtThreeDimensionalViewer::ChangeViewSlice(int view) {
    double originX, originY, originZ;

    _targetImage->GetOrigin(originX, originY, originZ);
    _targetImage->WorldToImage(originX, originY, originZ);

    switch (view) {
    case VIEW_AXIAL :
        originZ = currentSlice[2];
        _dx = _targetImage->GetXSize();
        _dy = _targetImage->GetYSize();
        break;
    case VIEW_SAGITTAL :
        originX = currentSlice[0];
        _dx = _targetImage->GetYSize();
        _dy = _targetImage->GetZSize();
        break;
    case VIEW_CORONAL :
        originY = currentSlice[1];
        _dx = _targetImage->GetXSize();
        _dy = _targetImage->GetZSize();
        break;
    default:
        cerr << "Not a valid type of two dimensional viewer" << endl;
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
