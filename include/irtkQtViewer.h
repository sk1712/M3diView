#ifndef IRTKQTVIEWER_H
#define IRTKQTVIEWER_H

#include <irtkImage.h>

class irtkQtViewer
{
    static irtkImage* _targetImage;
    static irtkImage* _sourceImage;

    irtkQtViewer();
    irtkQtViewer(irtkQtViewer const&);
    irtkQtViewer& operator=(irtkQtViewer const&);
    static irtkQtViewer* viewInstance;

public:
    static irtkQtViewer* Instance();
    static void CreateTargetImage(string imageName);
    static void DestroyTargetImage();

    static void CreateSourceImage(string imageName);
    static void DestroySourceImage();

};

#endif // IRTKQTVIEWER_H
