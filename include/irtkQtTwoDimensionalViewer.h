#ifndef IRTKQTTWODIMENSIONALVIEWER_H
#define IRTKQTTWODIMENSIONALVIEWER_H

#include <irtkImage.h>
#include <irtkTransformation.h>

#include <irtkLookupTable.h>

enum irtkViewMode {VIEW_AXIAL, VIEW_SAGITTAL, VIEW_CORONAL};

class irtkQtTwoDimensionalViewer
{
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

    irtkLookupTable *_targetLookupTable;

    irtkTransformation *_targetTransform;

    irtkImageFunction *_targetInterpolator;

public:
    irtkQtTwoDimensionalViewer(irtkViewMode viewMode);
    ~irtkQtTwoDimensionalViewer();

    void SetTarget(irtkImage* image);
    void SetOrigin(double x, double y, double z);
    void SetResolution(double dx, double dy, double dz);
    void SetDimensions(int width, int height);

    int GetSliceNumber();
    irtkViewMode GetViewMode();
    irtkColor* GetDrawable();

    void InitializeOutputImage();

protected:
    void InitializeOriginOrientation();

    void SetOrientation(const double * xaxis, const double * yaxis, const double * zaxis);
    void CalculateOutputImage();
};


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
