#ifndef IRTKQTTWODIMENSIONALVIEWER_H
#define IRTKQTTWODIMENSIONALVIEWER_H

#include <irtkQtBaseViewer.h>


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

    /// Segmentation output map
    map<SegKey, irtkGreyImage *> _segmentationOutput;

    /// Segmentation trasform map
    map<SegKey, irtkTransformation *> _segTransform;

    /// Segmentation interpolator map
    map<SegKey, irtkImageFunction *> _segInterpolator;

    /// Segmentation transformation filter map
    map<SegKey, irtkImageTransformation *> _segTransformFilter;

public:

    /// Class constructor
    irtkQtTwoDimensionalViewer(irtkViewMode viewMode);

    /// Class destructor
    ~irtkQtTwoDimensionalViewer();

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

    /// Get the array of RGB values for the labels drawn on the screen
    vector<QRgb*> GetSegmentationDrawable();

    /// Set interpolation method
    void SetInterpolationMethod(int index, irtkQtImageObject::irtkQtInterpolationMode mode);

    /// Calculate the output images from the transformations
    void CalculateImageOutput();

    /// Calculate a single output image from a transformation
    void CalculateCurrentImageOutput();

    /// Initialize the transformations from the input to the output images
    void InitializeImageTransformations();

    /// Initialize a single transformation from the input to the output image
    void InitializeCurrentImageTransformation();

    /// Calculate the output images from the transformations
    void CalculateSegmentationOutput();

    /// Calculate a single output image from a transformation
    void CalculateCurrentSegmentationOutput();

    /// Initialize the transformations from the input to the output images
    void InitializeSegmentationTransformations();

    /// Initialize single transformation from the input to the output image
    void InitializeCurrentSegmentationTransformation();

    /// Move image with key previousKey to newKey
    void MoveImage(int previousKey, int newKey);

    /// Update key values after invalid image is deleted from the list
    void UpdateKeysAfterIndexDeleted(int index);

    /// Get the labels displayed on the screen
    void GetLabels(char &top, char &bottom, char &left, char &right);

    /// Get the object name for the corresponding view (used for styling)
    string GetObjectName();

    /// Delete all map elements and clear maps
    void ClearDisplayedImages();

    /// Add image object to the map of images to be displayed
    void AddToDisplayedImages(irtkQtImageObject *imageObject, int index);

    /// Delete single image
    void DeleteSingleImage(int index);

    /// Delete single segmentation
    void DeleteSingleSegmentation(int parentIndex, int index);

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

    /// Add new segmentation and corresponding tools to maps
    void AddToSegmentationMaps(irtkImage *newSegmentation, SegKey key,
                               QColor label);

    /// Delete all elements of a map first and then clear
    template<class K, class T>
    void DeleteMap(map<K, T> & mymap);
};


inline void irtkQtTwoDimensionalViewer::ClearDisplayedImages() {
    _image.clear();
    _lookupTable.clear();
    DeleteMap(_imageOutput);
    DeleteMap(_transform);
    DeleteMap(_interpolator);
    DeleteMap(_transformFilter);

    _segmentation.clear();
    _labelColor.clear();
    DeleteMap(_segmentationOutput);
    DeleteMap(_segTransform);
    DeleteMap(_segInterpolator);
    DeleteMap(_segTransformFilter);
}

template<class K, class T>
void irtkQtTwoDimensionalViewer::DeleteMap(map<K, T> &mymap) {
    typename map<K, T>::iterator it;

    for( it = mymap.begin(); it != mymap.end(); it++){
        delete ((*it).second);
    }
    mymap.clear();
}

#endif // IRTKQTTWODIMENSIONALVIEWER_H
