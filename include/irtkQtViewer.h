#ifndef IRTKQTVIEWER_H
#define IRTKQTVIEWER_H

#include <irtkQtTwoDimensionalViewer.h>
#include <irtkQtThreeDimensionalViewer.h>


class irtkQtViewer
{
    /// list with loaded images
    QList<irtkQtImageObject*> _imageObjects;

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

    /// create image object from the given file name
    void CreateImage(QString imageFileName);

    /// delete all image objects
    void DestroyImages();

    /// create an 2D viewer of certain view (axial, sagittal, coronal)
    /// defined by viewMode
    irtkQtTwoDimensionalViewer* CreateTwoDimensionalViewer(irtkViewMode viewMode);

    /// create a 3D viewer
    irtkQtThreeDimensionalViewer* CreateThreeDimensionalViewer();

    /// get reference to image list
    QList<irtkQtImageObject*> & GetImageList();
};


inline QList<irtkQtImageObject*> & irtkQtViewer::GetImageList() {
    return _imageObjects;
}

#endif // IRTKQTVIEWER_H
