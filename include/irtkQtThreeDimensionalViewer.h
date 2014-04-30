#ifndef IRTKQTTHREEDIMENSIONALVIEWER_H
#define IRTKQTTHREEDIMENSIONALVIEWER_H

#include <irtkQtBaseViewer.h>

#include <QtConcurrentMap>


class irtkQtThreeDimensionalViewer : public irtkQtBaseViewer
{
    Q_OBJECT

    /// store previous slice info
    int previousSlice[3];

    /// image output vector
    vector<irtkGreyImage **> _imageOutput;

    /// image transform vector
    vector<irtkTransformation **> _transform;

    /// image interpolator vector
    vector<irtkImageFunction **> _interpolator;

    /// image transformaton filter vector
    vector<irtkImageTransformation **> _transformFilter;

public:

    /// class constructor
    irtkQtThreeDimensionalViewer();

    /// class destructor
    ~irtkQtThreeDimensionalViewer();

    /// get the array of RGB values to be drawn on the screen
    vector<QRgb**> GetDrawable();

    /// initialize the transformation from the input to the output image
    void InitializeTransformation();

    /// calculate the output image from the transformation
    void CalculateOutputImages();

    /// delete all vector elements and clear vectors
    void ClearDisplayedImages();

public slots:

    /// callback function when image is resized to (width, height)
    void ResizeImage(int width, int height);

    /// callback function when slice is changed
    void ChangeSlice(int slice);

    /// callback function when origin is changed
    void ChangeOrigin(int x, int y);

protected:

    ///
    void UpdateCurrentSlice();

    /// add new image and corresponding tools to vectors
    void AddToVectors(irtkImage* newImage);

    void SetOrientation(int view);

    void ChangeViewSlice(int view);

private:

    template<class T> void DeleteArrayVector(vector<T> & vec);
};


inline void irtkQtThreeDimensionalViewer::ClearDisplayedImages() {
    DeleteVector(_image);
    DeleteArrayVector(_imageOutput);
    DeleteVector(_lookupTable);
    DeleteArrayVector(_transform);
    DeleteArrayVector(_interpolator);
    DeleteArrayVector(_transformFilter);
}

template<class T>
inline void irtkQtThreeDimensionalViewer::DeleteArrayVector(vector<T> & vec) {
    typename vector<T>::iterator it;

    for (it = vec.begin(); it != vec.end(); it++) {
        delete [] (*it);
    }

    vec.clear();
}

#endif // IRTKQTTHREEDIMENSIONALVIEWER_H
