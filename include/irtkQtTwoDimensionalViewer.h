#ifndef IRTKQTTWODIMENSIONALVIEWER_H
#define IRTKQTTWODIMENSIONALVIEWER_H

#include <irtkQtBaseViewer.h>

#include <QtConcurrentMap>


class irtkQtTwoDimensionalViewer : public irtkQtBaseViewer
{
    Q_OBJECT

    /// image output vector
    vector<irtkGreyImage *> _imageOutput;

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

    /// get the array of RGB values to be drawn on the screen
    vector<QRgb**> GetDrawable();

    /// initialize the transformation from the input to the output image
    void InitializeTransformation();

    /// calculate the output image from the transformation
    void CalculateOutputImages();

    /// get the labels displayed on the screen
    void GetLabels(char &top, char &bottom, char &left, char &right);

    /// get the object name for the corresponding view (useful for the styling)
    string GetObjectName();

    /// delete all vector elements and clear vectors
    void ClearDisplayedImages();

    /// add image object to the vector of images to be displayed
    void AddToDisplayedImages(irtkQtImageObject *imageObject);

public slots:

    /// callback function when image is resized to (width, height)
    void ResizeImage(int width, int height);

    /// callback function when slice is changed
    void ChangeSlice(int* slice);

    /// callback function when origin is changed
    void ChangeOrigin(int x, int y);

protected:

    /// update the slice currently visible (in image coordinates)
    void UpdateCurrentSlice();

    /// add new image and corresponding tools to vectors
    void AddToVectors(irtkImage* newImage);
};


inline void irtkQtTwoDimensionalViewer::ClearDisplayedImages() {
    DeleteVector(_image);
    DeleteVector(_imageOutput);
    DeleteVector(_lookupTable);
    DeleteVector(_transform);
    DeleteVector(_interpolator);
    DeleteVector(_transformFilter);
}

#endif // IRTKQTTWODIMENSIONALVIEWER_H
