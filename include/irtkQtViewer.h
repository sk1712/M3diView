#ifndef IRTKQTVIEWER_H
#define IRTKQTVIEWER_H

#include <irtkQtTwoDimensionalViewer.h>

class irtkQtViewer
{
    vector<irtkImage*> _targetImage;

    /// image origin in world coordinates
    double _originX, _originY, _originZ;

    /// image orientation
    double _axisX[3], _axisY[3], _axisZ[3];

    /// image resolution
    double _resolution;

    /// private constructor
    irtkQtViewer();
    irtkQtViewer(irtkQtViewer const&);
    irtkQtViewer& operator=(irtkQtViewer const&);
    static irtkQtViewer* viewInstance;

public:
    static irtkQtViewer* Instance();
    static void Destroy();

    void CreateImage(string imageName);
    void DestroyImages();
    irtkImage* GetImage(int i);

    irtkQtTwoDimensionalViewer* CreateTwoDimensionalViewer(irtkViewMode viewMode);

protected:
    void InitializeImage(int i);
};


inline irtkImage* irtkQtViewer::GetImage(int i) {
    return _targetImage.at(i);
}

#endif // IRTKQTVIEWER_H
