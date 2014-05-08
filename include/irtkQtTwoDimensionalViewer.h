#ifndef IRTKQTTWODIMENSIONALVIEWER_H
#define IRTKQTTWODIMENSIONALVIEWER_H

#include <irtkQtBaseViewer.h>

#include <QtConcurrentMap>


class irtkQtTwoDimensionalViewer : public irtkQtBaseViewer
{
    Q_OBJECT

    /// image output map
    map<int, irtkGreyImage *> _imageOutput;

    /// image transform map
    map<int, irtkTransformation *> _transform;

    /// image interpolator map
    map<int, irtkImageFunction *> _interpolator;

    /// image transformaton filter map
    map<int, irtkImageTransformation *> _transformFilter;

public:

    /// class constructor
    irtkQtTwoDimensionalViewer(irtkViewMode viewMode);

    /// class destructor
    ~irtkQtTwoDimensionalViewer();

    /// get the array of RGB values to be drawn on the screen
    vector<QRgb**> GetDrawable();

    /// calculate the output image from the transformation
    void CalculateOutputImages();

    /// calculate single output image from the transformation
    void CalculateCurrentOutput();

    /// initialize the transformation from the input to the output image
    void InitializeTransformation();

    /// initialize single transformation from the input to the output image
    void InitializeCurrentTransformation();

    /// move image with key previousKey to newKey
    void MoveImage(int previousKey, int newKey);

    /// get the labels displayed on the screen
    void GetLabels(char &top, char &bottom, char &left, char &right);

    /// get the object name for the corresponding view (useful for the styling)
    string GetObjectName();

    /// delete all vector elements and clear vectors
    void ClearDisplayedImages();

    /// add image object to the vector of images to be displayed
    void AddToDisplayedImages(irtkQtImageObject *imageObject, int index);

    /// delete single image
    void DeleteSingleImage(int index);

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
    void AddToMaps(irtkImage* newImage, int index);
};


inline void irtkQtTwoDimensionalViewer::ClearDisplayedImages() {
    DeleteMap(_image);
    DeleteMap(_imageOutput);
    DeleteMap(_lookupTable);
    DeleteMap(_transform);
    DeleteMap(_interpolator);
    DeleteMap(_transformFilter);
}

#endif // IRTKQTTWODIMENSIONALVIEWER_H
