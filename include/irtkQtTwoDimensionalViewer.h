#ifndef IRTKQTTWODIMENSIONALVIEWER_H
#define IRTKQTTWODIMENSIONALVIEWER_H

#include <irtkImage.h>
#include <irtkTransformation.h>

#include <irtkQtLookupTable.h>
#include <irtkQtImageObject.h>

#include <QObject>
#include <QVector>
#include <QtConcurrentMap>


/// view modes
enum irtkViewMode {VIEW_AXIAL, VIEW_SAGITTAL, VIEW_CORONAL};

class irtkQtTwoDimensionalViewer : public QObject
{
    Q_OBJECT

    /// image origin
    double _originX, _originY, _originZ;

    /// orientation
    double _axisX[3], _axisY[3], _axisZ[3];

    /// resolution
    double _dx, _dy, _dz;

    /// dimensions
    int _width, _height;

    /// number of slices
    int sliceNum;

    /// view mode (axial, sagittal, coronal)
    irtkViewMode _viewMode;

    /// image against which all other images are transformed
    irtkImage * _targetImage;

    /// original image vector
    vector<irtkImage *> _image;

    /// image output vector
    vector<irtkGreyImage *> _imageOutput;

    /// image lookup table vector
    vector<irtkQtLookupTable *> _lookupTable;

    /// image transform vector
    vector<irtkTransformation *> _transform;

    /// image interpolator vector
    vector<irtkImageFunction *> _interpolator;

    /// image transformaton filter vector
    vector<irtkImageTransformation *> _transformFilter;

public:

    /// class constructor
    irtkQtTwoDimensionalViewer(irtkViewMode viewMode);

    /// class destructor
    ~irtkQtTwoDimensionalViewer();

    /// set target image
    void SetTarget(irtkImage* image);

    /// set image origin
    void SetOrigin(double x, double y, double z);

    /// set image resolution
    void SetResolution(double dx, double dy, double dz);

    /// increase image resolution
    void IncreaseResolution();

    /// decrease image resolution
    void DecreaseResolution();

    /// set image dimensions
    void SetDimensions(int width, int height);

    /// get current slice in image coordinates
    int GetCurrentSlice();

    /// get total number of slices
    int GetSliceNumber();

    /// get view mode (axial, sagittal, coronal)
    irtkViewMode GetViewMode();

    /// get the array of RGB values to be drawn on the screen
    vector<QRgb*> GetDrawable();

    /// get the labels displayed on the screen
    void GetLabels(char &top, char &bottom, char &left, char &right);

    /// initialize the parameters of the output image
    void InitializeOutputImage();

    /// initialize the transformation from the input to the output image
    void InitializeTransformation();

    /// delete all vector elements and clear vectors
    void ClearDisplayedImages();

    /// add image object to the vector of images to be displayed
    void AddToDisplayedImages(irtkQtImageObject *imageObject);

public slots:

    /// callback function when image is resized to (width, height)
    void ResizeImage(int width, int height);

    /// callback function when slice is changed
    void ChangeSlice(int slice);

    /// callback function when origin is changed
    void ChangeOrigin(int x, int y);

protected:

    /// initialize image origin and orientation
    void InitializeOriginOrientation();

    /// add new image and corresponding tools to vectors
    void AddToVectors(irtkImage* newImage);

    /// sets image orientation
    void SetOrientation(const double * xaxis, const double * yaxis, const double * zaxis);

    /// calculate the output image from the transformation
    void CalculateOutputImages(irtkImageAttributes attr);

    /// delete all elements of a vector first and then clear
    template<class T> void DeleteVector(vector<T> & vec);

signals:

    /// signal emitted when image is resized
    void ImageResized(QVector<QRgb*>);

    /// signal emitted when image origin changes
    void OriginChanged(double originX, double originY, double originZ);
};


inline void irtkQtTwoDimensionalViewer::SetTarget(irtkImage *image) {
    _targetImage = image;
}

inline void irtkQtTwoDimensionalViewer::SetOrigin(double x, double y, double z) {
    _originX = x;
    _originY = y;
    _originZ = z;
}

inline void irtkQtTwoDimensionalViewer::SetResolution(double dx, double dy, double dz) {
    _dx = dx;
    _dy = dy;
    _dz = dz;
}

inline void irtkQtTwoDimensionalViewer::IncreaseResolution() {
    if (_dx > 0.1)
        _dx -= 0.1;
    if (_dy > 0.1)
        _dy -= 0.1;
}

inline void irtkQtTwoDimensionalViewer::DecreaseResolution() {
    if (_dx < 10.0)
        _dx += 0.1;
    if (_dy < 10.0)
        _dy += 0.1;
}

inline void irtkQtTwoDimensionalViewer::SetDimensions(int width, int height) {
    _width = width;
    _height = height;
}

inline int irtkQtTwoDimensionalViewer::GetSliceNumber() {
    return sliceNum;
}

inline irtkViewMode irtkQtTwoDimensionalViewer::GetViewMode() {
    return _viewMode;
}

inline void irtkQtTwoDimensionalViewer::ClearDisplayedImages() {
    DeleteVector(_image);
    DeleteVector(_imageOutput);
    DeleteVector(_lookupTable);
    DeleteVector(_transform);
    DeleteVector(_interpolator);
    DeleteVector(_transformFilter);
}

template<class T>
inline void irtkQtTwoDimensionalViewer::DeleteVector(vector<T> & vec) {
    qDeleteAll(vec);
    vec.clear();
}

#endif // IRTKQTTWODIMENSIONALVIEWER_H
