#ifndef IRTKQTTWODIMENSIONALVIEWER_H
#define IRTKQTTWODIMENSIONALVIEWER_H

#include <QObject>

#include <vector>

#include <irtkImage.h>
#include <irtkTransformation.h>

#include <irtkLookupTable.h>

enum irtkViewMode {VIEW_AXIAL, VIEW_SAGITTAL, VIEW_CORONAL};

class irtkQtTwoDimensionalViewer : public QObject
{
    Q_OBJECT

    int _id;

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

    /// original target image
    irtkImage *_targetImage;

    /// image output
    irtkGreyImage *_targetImageOutput;

    /// image lookup table
    irtkLookupTable *_targetLookupTable;

    /// image transform
    irtkTransformation *_targetTransform;

    /// image interpolator
    irtkImageFunction *_targetInterpolator;

    /// image transformaton filter
    irtkImageTransformation *_targetTransformFilter;

public:

    irtkQtTwoDimensionalViewer(irtkViewMode viewMode);

    ~irtkQtTwoDimensionalViewer();

    void SetId(int id);

    void SetTarget(irtkImage* image);

    void SetOrigin(double x, double y, double z);

    void SetResolution(double dx, double dy, double dz);

    void IncreaseResolution();

    void DecreaseResolution();

    void SetDimensions(int width, int height);

    int GetCurrentSlice();

    int GetSliceNumber();

    irtkViewMode GetViewMode();

    irtkColor* GetDrawable();

    void GetLabels(char &top, char &bottom, char &left, char &right);

    void InitializeOutputImage();

    void InitializeTransformation();

public slots:

    void ResizeImage(int width, int height);

    void ChangeSlice(int slice);

    void ChangeOrigin(int x, int y);

protected:

    void InitializeOriginOrientation();

    void SetOrientation(const double * xaxis, const double * yaxis, const double * zaxis);

    void CalculateOutputImage();

signals:

    void ImageResized(irtkColor*);

    void OriginChanged(double originX, double originY, double originZ);
};


inline void irtkQtTwoDimensionalViewer::SetId(int id) {
    _id = id;
}

inline void irtkQtTwoDimensionalViewer::SetTarget(irtkImage *image) {
    _targetImage = image;
    SetResolution(1, 1, _targetImage->GetZSize());
    InitializeOriginOrientation();
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
    _dx -= 0.1;
    _dy -= 0.1;
}

inline void irtkQtTwoDimensionalViewer::DecreaseResolution() {
    _dx += 0.1;
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

#endif // IRTKQTTWODIMENSIONALVIEWER_H
