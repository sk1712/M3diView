#ifndef IRTKQTVIEWER_H
#define IRTKQTVIEWER_H

#include <irtkQtImageObject.h>
#include <irtkQtTwoDimensionalViewer.h>


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

    /// create irtkImage* from the given file name
    void CreateImage(QString imageFileName);

    /// delete all images in _images vector
    void DestroyImages();

    /// create an 2D viewer of certain view (axial, sagittal, coronal)
    ///defined by viewMode
    irtkQtTwoDimensionalViewer* CreateTwoDimensionalViewer(irtkViewMode viewMode);

    /// get image list
    const QList<irtkQtImageObject*> & GetImageList() const;
};


inline const QList<irtkQtImageObject*> & irtkQtViewer::GetImageList() const {
    return _imageObjects;
}

#endif // IRTKQTVIEWER_H
