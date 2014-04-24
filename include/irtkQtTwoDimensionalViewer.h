#ifndef IRTKQTTWODIMENSIONALVIEWER_H
#define IRTKQTTWODIMENSIONALVIEWER_H

#include <irtkQtBaseViewer.h>

#include <QVector>
#include <QtConcurrentMap>


class irtkQtTwoDimensionalViewer : public irtkQtBaseViewer
{
    Q_OBJECT

    /// view mode (axial, sagittal, coronal)
    irtkViewMode _viewMode;

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

    /// get current slice in image coordinates
    int GetCurrentSlice();

    /// get total number of slices
    int GetSliceNumber();

    /// get view mode (axial, sagittal, coronal)
    irtkViewMode GetViewMode();

    /// get the labels displayed on the screen
    void GetLabels(char &top, char &bottom, char &left, char &right);

    /// delete all vector elements and clear vectors
    void ClearDisplayedImages();

public slots:

    /// callback function when image is resized to (width, height)
    void ResizeImage(int width, int height);

    /// callback function when slice is changed
    void ChangeSlice(int slice);

    /// callback function when origin is changed
    void ChangeOrigin(int x, int y);

};


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

#endif // IRTKQTTWODIMENSIONALVIEWER_H
