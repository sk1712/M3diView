#ifndef IRTKQTVIEWER_H
#define IRTKQTVIEWER_H

#include <irtkQtTwoDimensionalViewer.h>

class irtkQtViewer
{
    irtkImage* _targetImage;
    irtkImage* _sourceImage;

    /// image origin in world coordinates
    double _originX, _originY, _originZ;

    /// image orientation
    double _axisX[3], _axisY[3], _axisZ[3];

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

    void CreateSourceImage(string imageName);
    void DestroySourceImage();

    irtkQtTwoDimensionalViewer* CreateTwoDimensionalViewer(irtkViewMode viewMode,
                                                           double originX = 0,
                                                           double originY = 0,
                                                           double originZ = 0);

protected:
    void InitializeTargetImage();
    void InitializeSourceImage();
};

#endif // IRTKQTVIEWER_H
