#include <irtkQtViewer.h>

//#include <cstdio>

irtkQtViewer* irtkQtViewer::viewInstance = NULL;

irtkQtViewer::irtkQtViewer() {
    _imageObjects.clear();
}

irtkQtViewer* irtkQtViewer::Instance() {
    if (!viewInstance)
        viewInstance = new irtkQtViewer;

    return viewInstance;
}

void irtkQtViewer::Destroy() {
    if (viewInstance) {
        viewInstance->DestroyImages();
        delete viewInstance;
    }
}

void irtkQtViewer::CreateImage(QString imageFileName) {
    irtkQtImageObject* newImage = NULL;

    try {
        newImage = new irtkQtImageObject(imageFileName);
        _imageObjects.append(newImage);
    } catch (irtkException e) {
        delete newImage;
        throw e;
    }
}

void irtkQtViewer::DestroyImages() {
    while (!_imageObjects.isEmpty())
        delete _imageObjects.takeFirst();
}

irtkQtTwoDimensionalViewer* irtkQtViewer::CreateTwoDimensionalViewer(irtkViewMode viewMode) {
    irtkQtTwoDimensionalViewer* twoDViewer = new irtkQtTwoDimensionalViewer(viewMode);
    return twoDViewer;
}
