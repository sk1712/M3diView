#ifndef IRTKQTVIEWER_H
#define IRTKQTVIEWER_H

#include <irtkQtTwoDimensionalViewer.h>

class irtkQtViewer
{
    irtkImage* _targetImage;

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

    void CreateTargetImage(string imageName);
    void DestroyTargetImage();
    irtkImage* GetTargetImage();

    irtkQtTwoDimensionalViewer* CreateTwoDimensionalViewer(irtkViewMode viewMode);

protected:
    void InitializeTargetImage();
};


inline irtkImage* irtkQtViewer::GetTargetImage() {
    return _targetImage;
}

#endif // IRTKQTVIEWER_H
