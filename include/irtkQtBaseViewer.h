#ifndef IRTKQTBASEVIEWER_H
#define IRTKQTBASEVIEWER_H

#include <irtkImage.h>
#include <irtkTransformation.h>

#include <irtkQtLookupTable.h>
#include <irtkQtImageObject.h>

#include <map>

#include <QObject>
#include <QVector>


class irtkQtBaseViewer : public QObject
{
    Q_OBJECT

public:

    /// view modes
    enum irtkViewMode {VIEW_AXIAL = 0, VIEW_SAGITTAL, VIEW_CORONAL, VIEW_NONE};

protected:

    /// image origin
    double _originX, _originY, _originZ;

    /// orientation
    double _axisX[3], _axisY[3], _axisZ[3];

    /// resolution
    double _dx, _dy, _dz;

    /// dimensions
    int _width, _height;

    /// view mode (axial, sagittal, coronal)
    irtkViewMode _viewMode;

    /// number of slices
    int* sliceNum;

    /// slices currently visible
    int* currentSlice;

    /// current index added to the images displayed
    int currentIndex;

    /// image against which all other images are transformed
    irtkImage* _targetImage;

    /// original image map
    map<int, irtkImage*> _image;

    /// image lookup table map
    map<int, irtkQtLookupTable *> _lookupTable;

public:

    /// class constructor
    irtkQtBaseViewer();

    /// class destructor
    virtual ~irtkQtBaseViewer();

    /// set target image
    void SetTarget(irtkImage* image);

    /// set image origin
    void SetOrigin(double x, double y, double z);

    /// set image resolution
    void SetResolution(double dx, double dy, double dz);

    /// increase image resolution
    void IncreaseResolution();

    /// decrease image resolution
    void DecreaseResolution();

    /// set image dimensions
    void SetDimensions(int width, int height);

    /// get view mode (axial, sagittal, coronal)
    irtkViewMode GetViewMode();

    /// get total number of slices
    int* GetSliceNumber();

    /// get current slice in image coordinates
    int* GetCurrentSlice();

    /// get the array of RGB values to be drawn on the screen
    virtual vector<QRgb**> GetDrawable() = 0;

    /// calculate the output image from the transformation
    virtual void CalculateOutputImages() = 0;

    /// calculate single output image from the transformation
    virtual void CalculateCurrentOutput() = 0;

    /// initialize the transformation from the input to the output image
    virtual void InitializeTransformation() = 0;

    /// initialize single transformation from the input to the output image
    virtual void InitializeCurrentTransformation() = 0;

    /// delete all map elements and clear maps
    virtual void ClearDisplayedImages() = 0;

    /// add image object to the maps of images to be displayed
    virtual void AddToDisplayedImages(irtkQtImageObject *imageObject, int index);

    /// delete single image
    virtual void DeleteSingleImage(int index);

    /// move image with key previousKey to newKey
    virtual void MoveImage(int previousKey, int newKey) = 0;

    /// set opacity for the corresponding image
    void SetOpacity(int value, int index);

public slots:

    /// callback function when image is resized to (width, height)
    virtual void ResizeImage(int width, int height) = 0;

    /// callback function when slice is changed
    virtual void ChangeSlice(int* slice) = 0;

    /// callback function when origin is changed
    virtual void ChangeOrigin(int x, int y) = 0;

protected:

    /// initialize the parameters of the output image
    irtkImageAttributes InitializeAttributes();

    /// initialize image origin and orientation
    void InitializeOriginOrientation();

    /// update the current slice (in image coordinates) corresponding to the world coordinates
    virtual void UpdateCurrentSlice() = 0;

    /// add new image and corresponding tools to maps
    virtual void AddToMaps(irtkImage* newImage, int index) = 0;

    /// set image orientation
    void SetOrientation(const double * xaxis, const double * yaxis, const double * zaxis);

    /// delete all elements of a map first and then clear
    template<class T>
    void DeleteMap(map<int, T> & mymap);

    template<class T>
    void MoveImageUp(map<int, T> & mymap, int previousKey, int newKey);

    template<class T>
    void MoveImageDown(map<int, T> & mymap, int previousKey, int newKey);

signals:

    /// signal emitted when image is resized
    void ImageResized(QVector<QRgb**>);

    /// signal emitted when image origin changes
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
inline void irtkQtBaseViewer::DeleteMap(map<int, T> & mymap) {
    typename map<int, T>::iterator it;

    for( it = mymap.begin(); it != mymap.end(); it++){
            delete ((*it).second);
    }
    mymap.clear();
}

template<class T>
void irtkQtBaseViewer::MoveImageUp(map<int, T> & mymap, int previousKey, int newKey) {
    map<int, T> newmap;
    typename map<int, T>::iterator it;

    for (it = mymap.begin(); it != mymap.end(); it++) {
        if ((it->first < newKey || it->first > previousKey)) {
            newmap.insert(pair<int, T> (it->first, it->second));
            continue;
        }
        else if (it->first == previousKey) {
            newmap.insert(pair<int, T> (newKey, it->second));
            continue;
        }
        else {
            newmap.insert(pair<int, T> (it->first + 1, it->second));
        }
    }

    mymap.clear();

    mymap = newmap;
}

template<class T>
void irtkQtBaseViewer::MoveImageDown(map<int, T> & mymap, int previousKey, int newKey) {
    map<int, T> newmap;
    typename map<int, T>::iterator it;

    for (it = mymap.begin(); it != mymap.end(); it++) {
        if ((it->first > newKey) || (it->first < previousKey)) {
            newmap.insert(pair<int, T> (it->first, it->second));
            continue;
        }
        else if (it->first == previousKey) {
            newmap.insert(pair<int, T> (newKey, it->second));
            continue;
        }
        else {
            newmap.insert(pair<int, T> (it->first - 1, it->second));
        }
    }

    mymap.clear();

    mymap = newmap;
}

#endif // IRTKQTBASEVIEWER_H
