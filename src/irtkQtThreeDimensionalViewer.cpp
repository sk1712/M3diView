#include <irtkQtThreeDimensionalViewer.h>

irtkQtThreeDimensionalViewer::irtkQtThreeDimensionalViewer() {
    ClearDisplayedImages();
    _viewMode = VIEW_NONE;
    currentSlice = new int[3];
    sliceNum = new int[3];

    for (int i = 0; i < 3; i++) previousSlice[i] = 0;
}

irtkQtThreeDimensionalViewer::~irtkQtThreeDimensionalViewer() {
    ClearDisplayedImages();
    delete [] currentSlice;
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

vector<QRgb**> irtkQtThreeDimensionalViewer::GetDrawable() {
    vector<QRgb**> allDrawables;
    QRgb _backgroundColor = qRgba(0, 0, 0, 1);


    int dimensions[3][2] = {
        {sliceNum[0], sliceNum[1]},
        {sliceNum[1], sliceNum[2]},
        {sliceNum[0], sliceNum[2]}
    };

    for (unsigned int index = 0; index < _image.size(); index++) {
        QRgb** drawable = new QRgb*[3];

        for (int dim = 0; dim < 3; dim++) {
            drawable[dim] = new QRgb[_imageOutput[index][dim]->GetNumberOfVoxels()];
            irtkGreyPixel *original = _imageOutput[index][dim]->GetPointerToVoxels();
            QRgb *drawn = drawable[dim];

            int i, j;

            for (j = 0; j < dimensions[dim][0]; j++) {
                for (i = 0; i < dimensions[dim][1]; i++) {
                    if (*original >= 0) {
                        *drawn = _lookupTable[index]->lookupTable[*original];
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
    double _targetMin, _targetMax;

    for (unsigned int i = 0; i < _image.size(); i++) {
        _image[i]->GetMinMaxAsDouble(&_targetMin, &_targetMax);

        for (int dim = 0; dim < 3; dim++) {
            _transformFilter[i][dim]->SetInput(_image[i]);
            _transformFilter[i][dim]->PutScaleFactorAndOffset(255.0 / (_targetMax
                - _targetMin), -_targetMin * 255.0 / (_targetMax - _targetMin));
        }
    }
}

void irtkQtThreeDimensionalViewer::ResizeImage(int width, int height) {

}

void irtkQtThreeDimensionalViewer::ChangeSlice(int slice) {

}

void irtkQtThreeDimensionalViewer::ChangeOrigin(int x, int y) {

}

void irtkQtThreeDimensionalViewer::AddToVectors(irtkImage* newImage) {
    _targetImage->Print();
    cout << "Voxel size x = " << _targetImage->GetXSize() << " and voxel size z = " << _targetImage->GetZSize() << endl;

    _image.push_back(newImage);

    _imageOutput.push_back(new irtkGreyImage*[3]);
    _transform.push_back(new irtkTransformation*[3]);
    _interpolator.push_back(new irtkImageFunction*[3]);
    _transformFilter.push_back(new irtkImageTransformation*[3]);

    for (int dim = 0; dim < 3; dim++) {
        _imageOutput.back()[dim] = new irtkGreyImage;
        _transform.back()[dim] = new irtkAffineTransformation;
        _interpolator.back()[dim] = new irtkNearestNeighborInterpolateImageFunction;

        irtkImageTransformation *transformation = new irtkImageTransformation;
        transformation->SetOutput(_imageOutput.back()[dim]);
        transformation->SetTransformation(_transform.back()[dim]);
        transformation->PutInterpolator(_interpolator.back()[dim]);
        transformation->PutSourcePaddingValue(0);
        _transformFilter.back()[dim] = transformation;
    }

    for (int i = 0; i < 3; i++) previousSlice[i] = 0;
}

void CalculateSingleTransform(irtkImageTransformation** &transform) {
    for (int i = 0; i < 3; i++) {
        transform[i]->PutSourcePaddingValue(-1);
        transform[i]->Run();
    }
}

void irtkQtThreeDimensionalViewer::CalculateOutputImages() {
    irtkImageAttributes attr[3];
    int width[3] = {sliceNum[0], sliceNum[1], sliceNum[0]};
    int height[3] = {sliceNum[1], sliceNum[2], sliceNum[2]};
    int index[3] = {2, 0, 1};

    double originX_backup = _originX, originY_backup = _originY, originZ_backup = _originZ;

    for (int i = 0; i < 3; i++) {
        if (previousSlice[index[i]] != currentSlice[index[i]]) {
            _width = width[i];
            _height = height[i];
            SetOrientation(i);
            ChangeViewSlice(i);
            attr[i] = InitializeAttributes();

            vector<irtkGreyImage **>::iterator image_it;
            for (image_it = _imageOutput.begin(); image_it != _imageOutput.end(); image_it++) {
                (*image_it)[i]->Initialize(attr[i]);
            }

            vector<irtkImageTransformation **>::iterator trans_it;
            for (trans_it = _transformFilter.begin(); trans_it != _transformFilter.end(); trans_it++) {
                (*trans_it)[i]->PutSourcePaddingValue(-1);
                (*trans_it)[i]->Run();
            }
        }
    }

    _originX = originX_backup;
    _originY = originY_backup;
    _originZ = originZ_backup;
    //QtConcurrent::blockingMap(_transformFilter, &CalculateSingleTransform);
}

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
