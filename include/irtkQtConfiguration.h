#ifndef IRTKQTCONFIGURATION_H
#define IRTKQTCONFIGURATION_H

#include <irtkQtTwoDimensionalViewer.h>
#include <irtkQtThreeDimensionalViewer.h>

class QXmlStreamWriter;
class QXmlStreamReader;

struct irtkQtConfigurationImage {
    int parentId;
    QString fileName;
    double minDisplay;
    double maxDisplay;
    int opacity;
    bool visible;
    QString interpolation;
    QString colormap;
    QString color;
};

struct irtkQtConfigurationViewer {
    QString type;
    bool cursorVisible;
    bool labelsVisible;
    bool imageOriginVisible;
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

    /// Instance of class
    static irtkQtConfiguration* viewInstance;

    /// Private constructor
    irtkQtConfiguration();

    /// Copy operator
    irtkQtConfiguration(irtkQtConfiguration const&);

    /// Assignment operator
    irtkQtConfiguration& operator=(irtkQtConfiguration const&);

    /// Read image list from configuration file
    void ReadImages(QXmlStreamReader& xmlReader);

    /// Read viewer list from configuration file
    void ReadViewers(QXmlStreamReader& xmlReader);

    /// Write image list and attributes in configuration file
    void WriteImages(QXmlStreamWriter& xmlWriter);

    /// Write viewer list and attributes in configuration file
    void WriteViewers(QXmlStreamWriter& xmlWriter);

public:

    /// Get instance of class
    static irtkQtConfiguration* Instance();

    /// Delete instance of class
    static void Destroy();

    /// Read configuration file
    bool Read(const QString fileName);

    /// Write configuration file
    void Write(const QString fileName);

    /// Set list with viewer info
    void SetImageList(QList<irtkQtConfigurationImage> const &imageList);

    /// Set list with viewer info
    void SetViewerList(QList<irtkQtConfigurationViewer> const &viewerList);

    /// Get image list
    QList<irtkQtConfigurationImage> GetImageList() const;

    /// Get viewer list
    QList<irtkQtConfigurationViewer> GetViewerList() const;
};


inline void irtkQtConfiguration::SetImageList(const QList<irtkQtConfigurationImage> &imageList) {
    _imageList = imageList;
}

inline void irtkQtConfiguration::SetViewerList(QList<irtkQtConfigurationViewer> const &viewerList) {
    _viewerList = viewerList;
}

inline QList<irtkQtConfigurationImage> irtkQtConfiguration::GetImageList() const {
    return _imageList;
}

inline QList<irtkQtConfigurationViewer> irtkQtConfiguration::GetViewerList() const {
    return _viewerList;
}

#endif // IRTKQTCONFIGURATION_H
