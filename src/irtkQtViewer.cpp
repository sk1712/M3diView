#include <irtkQtViewer.h>

#include <cstdio>

irtkQtViewer* irtkQtViewer::viewInstance = NULL;

irtkQtViewer::irtkQtViewer() {
    _images.clear();
}

irtkQtViewer* irtkQtViewer::Instance() {
    if (!viewInstance)
        viewInstance = new irtkQtViewer;

    return viewInstance;
}

void irtkQtViewer::Destroy() {
    viewInstance->DestroyImages();
    delete viewInstance;
}

void irtkQtViewer::CreateImage(string imageFileName) {
    irtkImage *newImage = irtkImage::New(imageFileName.c_str());
    _images.push_back(newImage);
    printf("Creating new image \n");

    if (_images.size() == 1) {
        newImage->GetOrientation(_axisX, _axisY, _axisZ);
        newImage->GetOrigin(_originX, _originY, _originZ);
        _resolution = 1;
    }
}

void irtkQtViewer::DestroyImages() {
    for(vector<irtkImage*>::const_iterator it = _images.begin();
        it != _images.end(); it++) {
        delete *it;
    }
    _images.clear();
}

irtkQtTwoDimensionalViewer* irtkQtViewer::CreateTwoDimensionalViewer(irtkViewMode viewMode) {
    irtkQtTwoDimensionalViewer* twoDViewer = new irtkQtTwoDimensionalViewer(viewMode);
    return twoDViewer;
}
