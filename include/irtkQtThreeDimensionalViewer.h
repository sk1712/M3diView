#ifndef IRTKQTTHREEDIMENSIONALVIEWER_H
#define IRTKQTTHREEDIMENSIONALVIEWER_H

#include <irtkQtBaseViewer.h>


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

    /// If two images are visible, view only the first one
    QRgb** GetOnlyADrawable();

    /// If two images are visible, view only the second one
    QRgb** GetOnlyBDrawable();

    /// If two images are visible, view both of them with a horizontal shutter
    QRgb** GetHShutterDrawable();

    /// If two images are visible, view both of them with a vertical shutter
    QRgb** GetVShutterDrawable();

    /// If two images are visible, view the result of their subtraction
    QRgb** GetSubtractionDrawable();

    /// Blend the images using their opacity values
    vector<QRgb**> GetBlendDrawable();

    /// Initialize the transformations from the input to the output images
    void InitializeImageTransformations();

    /// Initialize a single transformation from the input to the output image
    void InitializeCurrentImageTransformation();

    /// Set interpolation method
    void SetInterpolationMethod(int index, irtkQtImageObject::irtkQtInterpolationMode mode);

    /// Calculate the output images from the transformations
    void CalculateImageOutput();

    /// Calculate a single output image from the transformation
    void CalculateCurrentImageOutput();

    /// Calculate the output images from the transformations
    void CalculateSegmentationOutput();

    /// Calculate a single output image from a transformation
    void CalculateCurrentSegmentationOutput();

    /// Initialize the transformations from the input to the output images
    void InitializeSegmentationTransformations();

    /// Initialize single transformation from the input to the output image
    void InitializeCurrentSegmentationTransformation();

    /// Delete all map elements and clear maps
    void ClearDisplayedImages();

    /// Delete single image
    void DeleteSingleImage(int index);

    /// Move image with key previousKey to newKey
    void MoveImage(int previousKey, int newKey);

    /// Update key values after image is deleted from the list
    void UpdateKeysAfterImageDeleted(int index);

    /// Update key values after segmentation is deleted from the list
    void UpdateKeysAfterSegmentationDeleted(int parentIndex, int index);

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

    /// Add new segmentation and corresponding tools to maps
    void AddToSegmentationMaps(irtkImage *newSegmentation, SegKey key,
                               QColor label);

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
