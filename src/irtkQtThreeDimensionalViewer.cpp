#include <irtkQtThreeDimensionalViewer.h>


irtkQtThreeDimensionalViewer::irtkQtThreeDimensionalViewer() {
    ClearDisplayedImages();
    _viewMode = VIEW_NONE;
    currentSlice = new int[3];
    sliceNum = new int[3];
}

irtkQtThreeDimensionalViewer::~irtkQtThreeDimensionalViewer() {
    ClearDisplayedImages();
    delete [] currentSlice;
}

int* irtkQtThreeDimensionalViewer::GetCurrentSlice() {
    double x, y, z;

    x = _originX;
    y = _originY;
    z = _originZ;

    _targetImage->WorldToImage(x, y, z);

    currentSlice[0] = (int) round(z);
    currentSlice[1] = (int) round(x);
    currentSlice[2] = (int) round(y);

    return currentSlice;
}

vector<QRgb**> irtkQtThreeDimensionalViewer::GetDrawable() {
    vector<QRgb**> allDrawables;
    QRgb _backgroundColor = qRgba(0, 0, 0, 0);

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
}

void CalculateSingleTransform(irtkImageTransformation** &transform) {
    for (int i = 0; i < 3; i++) {
        transform[i]->PutSourcePaddingValue(-1);
        transform[i]->Run();
    }
}

void irtkQtThreeDimensionalViewer::CalculateOutputImages() {
    double x[3], y[3], z[3];
    irtkImageAttributes attr[3];

    _targetImage->GetOrientation(x, y, z);

    // axial view
    _width = sliceNum[0];
    _height = sliceNum[1];
    SetOrientation(x, y, z);
    attr[0] = InitializeAttributes();

    // sagittal view
    _width = sliceNum[1];
    _height = sliceNum[2];
    SetOrientation(y, z, x);
    attr[1] = InitializeAttributes();

    // coronal view
    _width = sliceNum[0];
    _height = sliceNum[2];
    SetOrientation(x, z, y);
    attr[2] = InitializeAttributes();

    vector<irtkGreyImage **>::iterator it;
    for (it = _imageOutput.begin(); it != _imageOutput.end(); it++) {
        for (int i = 0; i < 3; i++) {
            (*it)[i]->Initialize(attr[i]);
        }
    }

    for (unsigned int j = 0; j < _transformFilter.size(); j++) {
        for (int i = 0; i < 3; i++) {
            _transformFilter[j][i]->PutSourcePaddingValue(-1);
            _transformFilter[j][i]->Run();
        }
    }
    //QtConcurrent::blockingMap(_transformFilter, &CalculateSingleTransform);
}
