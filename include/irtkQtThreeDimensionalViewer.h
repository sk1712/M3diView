#ifndef IRTKQTTHREEDIMENSIONALVIEWER_H
#define IRTKQTTHREEDIMENSIONALVIEWER_H

#include <irtkQtBaseViewer.h>

#include <QtConcurrentMap>


class irtkQtThreeDimensionalViewer : public irtkQtBaseViewer
{
    Q_OBJECT

    /// Image output map
    map<int, irtkGreyImage **> _imageOutput;

    /// Image transform map
    map<int, irtkTransformation **> _transform;

    /// Image interpolator map
    map<int, irtkImageFunction **> _interpolator;

    /// Image transformaton filter map
    map<int, irtkImageTransformation **> _transformFilter;

public:

    /// Class constructor
    irtkQtThreeDimensionalViewer();

    /// Class destructor
    ~irtkQtThreeDimensionalViewer();

    /// Get the array of RGB values to be drawn on the screen
    QRgb** GetOnlyADrawable();

    QRgb** GetOnlyBDrawable();

    QRgb** GetHShutterDrawable();

    QRgb** GetVShutterDrawable();

    QRgb** GetSubtractionDrawable();

    vector<QRgb**> GetBlendDrawable();

    /// Initialize the transformation from the input to the output image
    void InitializeTransformation();

    /// Initialize single transformation from the input to the output image
    void InitializeCurrentTransformation();

    /// Set interpolation method
    void SetInterpolationMethod(int index, irtkQtImageObject::irtkQtInterpolationMode mode);

    /// Calculate the output image from the transformation
    void CalculateOutputImages();

    /// Calculate single output image from the transformation
    void CalculateCurrentOutput();

    /// Delete all map elements and clear maps
    void ClearDisplayedImages();

    /// Delete single image
    void DeleteSingleImage(int index);

    /// Move image with key previousKey to newKey
    void MoveImage(int previousKey, int newKey);

    /// Update key values after invalid image is deleted from the list
    void UpdateKeysAfterIndexDeleted(int index);

public slots:

    /// Callback function when image is resized to (width, height)
    void ResizeImage(int width, int height);

    /// Callback function when slice is changed
    void ChangeSlice(int* slice);

    /// Callback function when origin is changed
    void ChangeOrigin(int x, int y);

protected:

    /// Update the current slice (in image coordinates)
    void UpdateCurrentSlice();

    /// Add new image and corresponding tools to maps
    void AddToMaps(irtkImage* newImage, int index);

    /// Set the orientation according to the view value
    void SetOrientation(int view);

    /// Change the slice corresponding to the view value
    void ChangeViewSlice(int view);

private:

    /// Delete array elements of a map
    template<class T> void DeleteArrayMap(map<int, T> & mymap);
};


inline void irtkQtThreeDimensionalViewer::ClearDisplayedImages() {
    _image.clear();
    _lookupTable.clear();
    DeleteArrayMap(_imageOutput);
    DeleteArrayMap(_transform);
    DeleteArrayMap(_interpolator);
    DeleteArrayMap(_transformFilter);
}

template<class T>
inline void irtkQtThreeDimensionalViewer::DeleteArrayMap(map<int, T> & mymap) {
    typename map<int, T>::iterator it;

    for( it = mymap.begin(); it != mymap.end(); it++){
        delete [] ((*it).second);
    }

    mymap.clear();
}

#endif // IRTKQTTHREEDIMENSIONALVIEWER_H
