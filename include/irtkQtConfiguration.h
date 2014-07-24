#ifndef IRTKQTCONFIGURATION_H
#define IRTKQTCONFIGURATION_H

#include <irtkQtTwoDimensionalViewer.h>
#include <irtkQtThreeDimensionalViewer.h>

class QXmlStreamWriter;
class QXmlStreamReader;

struct irtkQtConfigurationImage {
    QString fileName;
    double minDisplay;
    double maxDisplay;
    int opacity;
    bool visible;
    QString interpolation;
    QString colormap;
};

struct irtkQtConfigurationViewer {
    QString type;
    bool cursorVisible;
    bool labelVisible;
    bool fullScreen;
    bool linked;
    double origin[3];
    double resolution[3];
};

/*
 * Singleton class responsible for configuration
 */

class irtkQtConfiguration
{
    /// Image list
    QList<irtkQtConfigurationImage> _imageList;

    /// Viewer list
    QList<irtkQtConfigurationViewer> _viewerList;

    /// List of loaded images
    QList<irtkQtImageObject*> _imageObjectList;

    /// Instance of class
    static irtkQtConfiguration* viewInstance;

    /// Private constructor
    irtkQtConfiguration();

    /// Copy operator
    irtkQtConfiguration(irtkQtConfiguration const&);

    /// Assignment operator
    irtkQtConfiguration& operator=(irtkQtConfiguration const&);

    /// Delete all image objects
    void DestroyImages();

    /// Write image list and attributes in configuration file
    void WriteImages(QXmlStreamWriter& xmlWriter);

    /// Write viewer list and attributes in configuration file
    void WriteViewers(QXmlStreamWriter& xmlWriter);

public:

    /// Get instance of class
    static irtkQtConfiguration* Instance();

    /// Delete instance of class
    static void Destroy();

    /// Create image object from the given file name
    void CreateImage(QString imageFileName);

    /// Get reference to image list
    QList<irtkQtImageObject*> & GetImageObjectList();

    /// Read configuration file
    void Read(const QString fileName);

    /// Write configuration file
    void Write(const QString fileName);

    /// Set list with viewer info
    void SetViewerList(QList<irtkQtConfigurationViewer> const &viewerList);
};


inline QList<irtkQtImageObject*> & irtkQtConfiguration::GetImageObjectList() {
    return _imageObjectList;
}

inline void irtkQtConfiguration::SetViewerList(QList<irtkQtConfigurationViewer> const &viewerList) {
    _viewerList = viewerList;
}

#endif // IRTKQTCONFIGURATION_H
