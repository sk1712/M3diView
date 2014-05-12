#ifndef IRTKQTTHREEDIMENSIONALVIEWER_H
#define IRTKQTTHREEDIMENSIONALVIEWER_H

#include <irtkQtBaseViewer.h>

#include <QtConcurrentMap>


class irtkQtThreeDimensionalViewer : public irtkQtBaseViewer
{
    Q_OBJECT

    /// store previous slice info
    int previousSlice[3];

    /// image output map
    map<int, irtkGreyImage **> _imageOutput;

    /// image transform map
    map<int, irtkTransformation **> _transform;

    /// image interpolator map
    map<int, irtkImageFunction **> _interpolator;

    /// image transformaton filter map
    map<int, irtkImageTransformation **> _transformFilter;

public:

    /// class constructor
    irtkQtThreeDimensionalViewer();

    /// class destructor
    ~irtkQtThreeDimensionalViewer();

    /// get the array of RGB values to be drawn on the screen
    vector<QRgb**> GetDrawable();

    /// initialize the transformation from the input to the output image
    void InitializeTransformation();

    /// initialize single transformation from the input to the output image
    void InitializeCurrentTransformation();

    /// calculate the output image from the transformation
    void CalculateOutputImages();

    /// calculate single output image from the transformation
    void CalculateCurrentOutput();

    /// delete all map elements and clear maps
    void ClearDisplayedImages();

    /// delete single image
    void DeleteSingleImage(int index);

    /// move image with key previousKey to newKey
    void MoveImage(int previousKey, int newKey);

    /// update key values after invalid image is deleted from the list
    void UpdateKeysAfterIndexDeleted(int index);

public slots:

    /// callback function when image is resized to (width, height)
    void ResizeImage(int width, int height);

    /// callback function when slice is changed
    void ChangeSlice(int* slice);

    /// callback function when origin is changed
    void ChangeOrigin(int x, int y);

protected:

    /// update the current slice (in image coordinates)
    void UpdateCurrentSlice();

    /// add new image and corresponding tools to maps
    void AddToMaps(irtkImage* newImage, int index);

    /// set the orientation according to the view value
    void SetOrientation(int view);

    /// change the slice corresponding to the view value
    void ChangeViewSlice(int view);

private:

    /// delete array elements of a map
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
