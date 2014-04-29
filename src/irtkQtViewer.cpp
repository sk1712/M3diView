#include <irtkQtViewer.h>

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

    newImage = new irtkQtImageObject(imageFileName);
    _imageObjects.append(newImage);
}

void irtkQtViewer::DestroyImages() {
    while (!_imageObjects.isEmpty())
        delete _imageObjects.takeFirst();
}

irtkQtTwoDimensionalViewer* irtkQtViewer::CreateTwoDimensionalViewer(irtkViewMode viewMode) {
    irtkQtTwoDimensionalViewer* twoDViewer = new irtkQtTwoDimensionalViewer(viewMode);
    return twoDViewer;
}

irtkQtThreeDimensionalViewer* irtkQtViewer::CreateThreeDimensionalViewer() {
    irtkQtThreeDimensionalViewer* threeDViewer = new irtkQtThreeDimensionalViewer();
    return threeDViewer;
}
