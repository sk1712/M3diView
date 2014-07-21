#ifndef IRTKQTTWODIMENSIONALVIEWER_H
#define IRTKQTTWODIMENSIONALVIEWER_H

#include <irtkQtBaseViewer.h>

#include <QtConcurrentMap>


class irtkQtTwoDimensionalViewer : public irtkQtBaseViewer
{
    Q_OBJECT

    /// Image output map
    map<int, irtkGreyImage *> _imageOutput;

    /// Image transform map
    map<int, irtkTransformation *> _transform;

    /// Image interpolator map
    map<int, irtkImageFunction *> _interpolator;

    /// Image transformaton filter map
    map<int, irtkImageTransformation *> _transformFilter;

public:

    /// Class constructor
    irtkQtTwoDimensionalViewer(irtkViewMode viewMode);

    /// Class destructor
    ~irtkQtTwoDimensionalViewer();

    /// Get the array of RGB values to be drawn on the screen
    QRgb** GetOnlyADrawable();

    QRgb** GetOnlyBDrawable();

    QRgb** GetHShutterDrawable();

    QRgb** GetVShutterDrawable();

    QRgb** GetSubtractionDrawable();

    vector<QRgb**> GetBlendDrawable();

    /// Set interpolation method
    void SetInterpolationMethod(int index, irtkQtImageObject::irtkQtInterpolationMode mode);

    /// Calculate the output image from the transformation
    void CalculateOutputImages();

    /// Calculate single output image from the transformation
    void CalculateCurrentOutput();

    /// Initialize the transformation from the input to the output image
    void InitializeTransformation();

    /// Initialize single transformation from the input to the output image
    void InitializeCurrentTransformation();

    /// Move image with key previousKey to newKey
    void MoveImage(int previousKey, int newKey);

    /// Update key values after invalid image is deleted from the list
    void UpdateKeysAfterIndexDeleted(int index);

    /// Get the labels displayed on the screen
    void GetLabels(char &top, char &bottom, char &left, char &right);

    /// Get the object name for the corresponding view (useful for the styling)
    string GetObjectName();

    /// Delete all map elements and clear maps
    void ClearDisplayedImages();

    /// Add image object to the map of images to be displayed
    void AddToDisplayedImages(irtkQtImageObject *imageObject, int index);

    /// Delete single image
    void DeleteSingleImage(int index);

public slots:

    /// Callback function when image is resized to (width, height)
    void ResizeImage(int width, int height);

    /// Callback function when slice is changed
    void ChangeSlice(int* slice);

    /// Callback function when origin is changed
    void ChangeOrigin(int x, int y);

protected:

    /// Update the slice currently visible (in image coordinates)
    void UpdateCurrentSlice();

    /// Add new image and corresponding tools to maps
    void AddToMaps(irtkImage* newImage, int index);

    /// Delete all elements of a map first and then clear
    template<class T>
    void DeleteMap(map<int, T> & mymap);
};


inline void irtkQtTwoDimensionalViewer::ClearDisplayedImages() {
    _image.clear();
    _lookupTable.clear();
    DeleteMap(_imageOutput);
    DeleteMap(_transform);
    DeleteMap(_interpolator);
    DeleteMap(_transformFilter);
}

template<class T>
void irtkQtTwoDimensionalViewer::DeleteMap(map<int, T> & mymap) {
    typename map<int, T>::iterator it;

    for( it = mymap.begin(); it != mymap.end(); it++){
            delete ((*it).second);
    }
    mymap.clear();
}

#endif // IRTKQTTWODIMENSIONALVIEWER_H
