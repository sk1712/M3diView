#ifndef IRTKQTBASEVIEWER_H
#define IRTKQTBASEVIEWER_H

#include <irtkImage.h>
#include <irtkTransformation.h>

#include <irtkQtLookupTable.h>
#include <irtkQtImageObject.h>

#include <QObject>


/// view modes
enum irtkViewMode {VIEW_AXIAL, VIEW_SAGITTAL, VIEW_CORONAL};

class irtkQtBaseViewer : public QObject {

protected:

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

    /// image against which all other images are transformed
    irtkImage * _targetImage;

    /// original image vector
    vector<irtkImage *> _image;

public:

    /// class constructor
    void irtkQtBaseViewer();

    /// class destructor
    virtual void ~irtkQtBaseViewer();

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

    /// get the array of RGB values to be drawn on the screen
    vector<QRgb*> GetDrawable();

    /// initialize the parameters of the output image
    void InitializeOutputImage();

    /// initialize the transformation from the input to the output image
    void InitializeTransformation();

    /// delete all vector elements and clear vectors
    virtual void ClearDisplayedImages() = 0;

    /// add image object to the vector of images to be displayed
    void AddToDisplayedImages(irtkQtImageObject *imageObject);

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


inline void irtkQtBaseViewer::SetTarget(irtkImage *image) {
    _targetImage = image;
}

inline void irtkQtBaseViewer::SetOrigin(double x, double y, double z) {
    _originX = x;
    _originY = y;
    _originZ = z;
}

inline void irtkQtBaseViewer::SetResolution(double dx, double dy, double dz) {
    _dx = dx;
    _dy = dy;
    _dz = dz;
}

inline void irtkQtBaseViewer::IncreaseResolution() {
    if (_dx > 0.1)
        _dx -= 0.1;
    if (_dy > 0.1)
        _dy -= 0.1;
}

inline void irtkQtBaseViewer::DecreaseResolution() {
    if (_dx < 10.0)
        _dx += 0.1;
    if (_dy < 10.0)
        _dy += 0.1;
}

inline void irtkQtBaseViewer::SetDimensions(int width, int height) {
    _width = width;
    _height = height;
}

template<class T>
inline void irtkQtBaseViewer::DeleteVector(vector<T> & vec) {
    qDeleteAll(vec);
    vec.clear();
}

#endif // IRTKQTBASEVIEWER_H
