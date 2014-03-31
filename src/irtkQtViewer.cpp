#include <irtkQtViewer.h>

#include <cstdio>

irtkQtViewer* irtkQtViewer::viewInstance = NULL;

irtkQtViewer::irtkQtViewer() {
    _targetImage = NULL;
}

irtkQtViewer* irtkQtViewer::Instance() {
    if (!viewInstance)
        viewInstance = new irtkQtViewer;

    return viewInstance;
}

void irtkQtViewer::Destroy() {
    delete viewInstance;
}

void irtkQtViewer::CreateTargetImage(string imageFileName) {
    _targetImage = irtkImage::New(imageFileName.c_str());
    printf("Creating target image \n");

    _targetImage->GetOrientation(_axisX, _axisY, _axisZ);
    _targetImage->GetOrigin(_originX, _originY, _originZ);

    _resolution = 1;
}

void irtkQtViewer::DestroyTargetImage() {
    delete _targetImage;
}

irtkQtTwoDimensionalViewer* irtkQtViewer::CreateTwoDimensionalViewer(irtkViewMode viewMode) {
    irtkQtTwoDimensionalViewer* twoDViewer = new irtkQtTwoDimensionalViewer(viewMode);
    return twoDViewer;
}
