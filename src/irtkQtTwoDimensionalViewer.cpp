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



void irtkQtTwoDimensionalViewer::InitializeTransformation() {
    double _targetMin, _targetMax;

    for (unsigned int i = 0; i < _image.size(); i++) {
        _image[i]->GetMinMaxAsDouble(&_targetMin, &_targetMax);

        _transformFilter[i]->SetInput(_image[i]);
        _transformFilter[i]->PutScaleFactorAndOffset(255.0 / (_targetMax
            - _targetMin), -_targetMin * 255.0 / (_targetMax - _targetMin));
    }
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


