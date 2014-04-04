#ifndef IRTKQTVIEWER_H
#define IRTKQTVIEWER_H

#include <irtkQtTwoDimensionalViewer.h>


class irtkQtViewer
{
    /// vector with loaded images
    vector<irtkImage*> _images;

    /// image origin in world coordinates
    double _originX, _originY, _originZ;

    /// image orientation
    double _axisX[3], _axisY[3], _axisZ[3];

    /// image resolution
    double _resolution;

    /// private constructor
    irtkQtViewer();

    /// copy operator
    irtkQtViewer(irtkQtViewer const&);

    /// assignment operator
    irtkQtViewer& operator=(irtkQtViewer const&);

    /// instance of class (singleton)
    static irtkQtViewer* viewInstance;

public:
    /// get instance of class
    static irtkQtViewer* Instance();

    /// delete instance of class
    static void Destroy();

    /// create irtkImage* from the given file name
    void CreateImage(string imageName);

    /// delete all images in _images vector
    void DestroyImages();

    /// get image with index argument
    irtkImage* GetImage(int index);

    /// create an 2D viewer of certain view (axial, sagittal, coronal)
    ///defined by viewMode
    irtkQtTwoDimensionalViewer* CreateTwoDimensionalViewer(irtkViewMode viewMode);

protected:
    void InitializeImage(int i);
};


inline irtkImage* irtkQtViewer::GetImage(int i) {
    return _images.at(i);
}

#endif // IRTKQTVIEWER_H
