#ifndef IRTKQTVIEWER_H
#define IRTKQTVIEWER_H

#include <irtkQtTwoDimensionalViewer.h>
#include <irtkQtThreeDimensionalViewer.h>

/*
 * Signleton class where list of loaded images is stored
 */

class irtkQtViewer
{
    /// List of loaded images
    QList<irtkQtImageObject*> _imageObjects;

    /// Instance of class
    static irtkQtViewer* viewInstance;

    /// Private constructor
    irtkQtViewer();

    /// Copy operator
    irtkQtViewer(irtkQtViewer const&);

    /// Assignment operator
    irtkQtViewer& operator=(irtkQtViewer const&);

    /// Delete all image objects
    void DestroyImages();

public:

    /// Get instance of class
    static irtkQtViewer* Instance();

    /// Delete instance of class
    static void Destroy();

    /// Create image object from the given file name
    void CreateImage(QString imageFileName);

    /// Create a 2D viewer of certain view (axial, sagittal, coronal)
    /// defined by viewMode
    irtkQtTwoDimensionalViewer* CreateTwoDimensionalViewer(irtkQtBaseViewer::irtkViewMode viewMode);

    /// Create a 3D viewer
    irtkQtThreeDimensionalViewer* CreateThreeDimensionalViewer();

    /// Get reference to image list
    QList<irtkQtImageObject*> & GetImageList();
};


inline QList<irtkQtImageObject*> & irtkQtViewer::GetImageList() {
    return _imageObjects;
}

#endif // IRTKQTVIEWER_H
