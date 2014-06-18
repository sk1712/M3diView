#ifndef IRTKQTBASEVIEWER_H
#define IRTKQTBASEVIEWER_H

#include <irtkQtImageObject.h>

#include <qtconcurrentrun.h>

#include <QObject>
#include <QVector>


class irtkQtBaseViewer : public QObject
{
    Q_OBJECT

public:

    /// View modes
    enum irtkViewMode {VIEW_AXIAL = 0, VIEW_SAGITTAL, VIEW_CORONAL, VIEW_3D};

protected:

    /// Image origin
    double _originX, _originY, _originZ;

    /// Orientation
    double _axisX[3], _axisY[3], _axisZ[3];

    /// Resolution
    double _dx, _dy, _dz;

    /// Dimensions
    int _width, _height;

    /// View mode (axial, sagittal, coronal, 3D)
    irtkViewMode _viewMode;

    /// Number of slices in current view
    int* sliceNum;

    /// Slices currently visible
    int* currentSlice;

    /// Current index added to the images displayed
    int currentIndex;

    /// Image against which all other images are transformed
    irtkImage* _targetImage;

    /// Original image map
    map<int, irtkImage*> _image;

    /// Image lookup table map
    map<int, irtkQtLookupTable *> _lookupTable;

public:

    /// Class constructor
    irtkQtBaseViewer();

    /// Class destructor
    virtual ~irtkQtBaseViewer();

    /// Set target image
    void SetTarget(irtkImage* image);

    /// Set image origin
    void SetOrigin(double x, double y, double z);

    /// Set image resolution
    void SetResolution(double dx, double dy, double dz);

    /// Increase image resolution
    void IncreaseResolution();

    /// Decrease image resolution
    void DecreaseResolution();

    /// Set image dimensions
    void SetDimensions(int width, int height);

    /// Get view mode (axial, sagittal, coronal)
    irtkViewMode GetViewMode();

    /// Get total number of slices
    int* GetSliceNumber();

    /// Get current slice in image coordinates
    int* GetCurrentSlice();

    /// Get the array of RGB values to be drawn on the screen
    virtual vector<QRgb**> GetDrawable() = 0;

    /// Calculate the output image from the transformation
    virtual void CalculateOutputImages() = 0;

    /// Calculate single output image from the transformation
    virtual void CalculateCurrentOutput() = 0;

    /// Initialize the transformation from the input to the output image
    virtual void InitializeTransformation() = 0;

    /// Initialize single transformation from the input to the output image
    virtual void InitializeCurrentTransformation() = 0;

    /// Delete all map elements and clear maps
    virtual void ClearDisplayedImages() = 0;

    /// Add image object to the maps of images to be displayed
    virtual void AddToDisplayedImages(irtkQtImageObject *imageObject, int index);

    /// Delete single image
    virtual void DeleteSingleImage(int index);

    /// Move image with key previousKey to newKey
    virtual void MoveImage(int previousKey, int newKey);

    /// Update keys of maps after invalid image is deleted
    virtual void UpdateKeysAfterIndexDeleted(int index) = 0;

public slots:

    /// Callback function when image is resized to (width, height)
    virtual void ResizeImage(int width, int height) = 0;

    /// Callback function when slice is changed
    virtual void ChangeSlice(int* slice) = 0;

    /// Callback function when origin is changed
    virtual void ChangeOrigin(int x, int y) = 0;

protected:

    /// Initialize the parameters of the output image
    irtkImageAttributes InitializeAttributes();

    /// Initialize image origin
    void InitializeOrigin();

    /// Initialize image dimensions
    void InitializeDimensions();

    /// Initialize image orientation
    void InitializeOrientation();

    /// Update the current slice (in image coordinates) corresponding to the world coordinates
    virtual void UpdateCurrentSlice() = 0;

    /// Add new image and corresponding tools to maps
    virtual void AddToMaps(irtkImage* newImage, int index) = 0;

    /// Set image orientation
    void SetOrientation(const double * xaxis, const double * yaxis, const double * zaxis);

    /// Move one of the displayed images higher in hierarchy
    template<class T>
    void MoveImageUp(map<int, T> & mymap, int previousKey, int newKey);

    /// Move one of the displayed images lower in hierarchy
    template<class T>
    void MoveImageDown(map<int, T> & mymap, int previousKey, int newKey);

    /// Update key values of maps after deleting invalid image
    template<class T>
    void UpdateKeysAfterIndexDeleted(map<int, T> & mymap, int index);

signals:

    /// Signal emitted when image is resized
    void ImageResized(QVector<QRgb**>);

    /// Signal emitted when image origin changes
    void OriginChanged(double originX, double originY, double originZ);
};


inline void irtkQtBaseViewer::SetTarget(irtkImage *image) {
    _targetImage = image;
}

inline void irtkQtBaseViewer::SetOrigin(double x, double y, double z) {
    _originX = x;
    _originY = y;
    _originZ = z;

    UpdateCurrentSlice();
}

inline void irtkQtBaseViewer::SetResolution(double dx, double dy, double dz) {
    _dx = dx;
    _dy = dy;
    _dz = dz;
}

inline void irtkQtBaseViewer::IncreaseResolution() {
    if (_dx > 0.1)
        _dx -= 0.1;
    if (_dy > 0.1)
        _dy -= 0.1;
}

inline void irtkQtBaseViewer::DecreaseResolution() {
    if (_dx < 10.0)
        _dx += 0.1;
    if (_dy < 10.0)
        _dy += 0.1;
}

inline void irtkQtBaseViewer::SetDimensions(int width, int height) {
    _width = width;
    _height = height;
}

inline irtkQtBaseViewer::irtkViewMode irtkQtBaseViewer::GetViewMode() {
    return _viewMode;
}

inline int* irtkQtBaseViewer::GetSliceNumber() {
    return sliceNum;
}

inline int* irtkQtBaseViewer::GetCurrentSlice() {
    return currentSlice;
}

template<class T>
void irtkQtBaseViewer::MoveImageUp(map<int, T> & mymap, int previousKey, int newKey) {
    map<int, T> newmap;
    typename map<int, T>::iterator it;

    int key;
    T value;

    for (it = mymap.begin(); it != mymap.end(); it++) {
        key = it->first;
        value = it->second;

        if ((key < newKey || key > previousKey)) {
            newmap.insert(pair<int, T> (key, value));
            continue;
        }
        else if (key == previousKey) {
            newmap.insert(pair<int, T> (newKey, value));
            continue;
        }
        else {
            newmap.insert(pair<int, T> (key + 1, value));
        }
    }

    mymap.clear();

    mymap = newmap;
}

template<class T>
void irtkQtBaseViewer::MoveImageDown(map<int, T> & mymap, int previousKey, int newKey) {
    map<int, T> newmap;
    typename map<int, T>::iterator it;

    int key;
    T value;

    for (it = mymap.begin(); it != mymap.end(); it++) {
        key = it->first;
        value = it->second;

        if ((key > newKey) || (key < previousKey)) {
            newmap.insert(pair<int, T> (key, value));
            continue;
        }
        else if (it->first == previousKey) {
            newmap.insert(pair<int, T> (newKey, value));
            continue;
        }
        else {
            newmap.insert(pair<int, T> (key - 1, value));
        }
    }

    mymap.clear();

    mymap = newmap;
}

template<class T>
void irtkQtBaseViewer::UpdateKeysAfterIndexDeleted(map<int, T> & mymap, int index) {
    map<int, T> newmap;
    typename map<int, T>::iterator it;

    int key;
    T value;

    for (it = mymap.begin(); it != mymap.end(); it++) {
        key = it->first;
        value = it->second;

        if (key < index) {
            newmap.insert(pair<int, T> (key, value));
            continue;
        }
        else {
            newmap.insert(pair<int, T> (key - 1, value));
        }
    }

    mymap.clear();

    mymap = newmap;
}

#endif // IRTKQTBASEVIEWER_H
