#ifndef IRTKQTCONFIGURATION_H
#define IRTKQTCONFIGURATION_H

#include <irtkQtTwoDimensionalViewer.h>
#include <irtkQtThreeDimensionalViewer.h>

struct irtkQtConfigurationImage {
    QString fileName;
    double minDisplay;
    double maxDisplay;
    int opacity;
    bool visible;
    QString interpolation;
};

struct irtkQtConfigurationViewer {
    QString type;
    bool cursorVisible;
    bool labelVisible;
    bool fullScreen;
    double origin[3];
    double resolution[3];
};

/*
 * Signleton class responsible for configuration
 */

class irtkQtViewer
{
    /// Image list
    QList<irtkQtConfigurationImage> _imageList;

    /// Viewer list
    QList<irtkQtConfigurationViewer> _viewerList;

    /// List of loaded images
    QList<irtkQtImageObject*> _imageObjectList;

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

    /// Get reference to image list
    QList<irtkQtImageObject*> & GetImageObjectList();

    /// Read configuration file
    void ReadConfiguration(const QString fileName);

    /// Write configuration file
    void WriteConfiguration(const QString fileName);

    /// Set list with viewer info
    void SetViewerList(QList<irtkQtConfigurationViewer> const&);
};


inline QList<irtkQtImageObject*> & irtkQtViewer::GetImageObjectList() {
    return _imageObjectList;
}

#endif // IRTKQTCONFIGURATION_H
