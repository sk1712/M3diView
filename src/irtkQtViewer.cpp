#include <irtkQtViewer.h>


// singleton class, initialize instance to NULL
irtkQtViewer* irtkQtViewer::viewInstance = NULL;

irtkQtViewer::irtkQtViewer() {
    _imageObjects.clear();
}

void irtkQtViewer::DestroyImages() {
    while (!_imageObjects.isEmpty())
        delete _imageObjects.takeFirst();
}

irtkQtViewer* irtkQtViewer::Instance() {
    // if there is no other instance of this class create one
    if (!viewInstance)
        viewInstance = new irtkQtViewer;

    return viewInstance;
}

void irtkQtViewer::Destroy() {
    // if there is a view instance
    if (viewInstance) {
        // delete all images in the list
        viewInstance->DestroyImages();
        // and then delete the instance itselfs
        delete viewInstance;
    }
}

void irtkQtViewer::CreateImage(QString imageFileName) {
    irtkQtImageObject* newImage = NULL;

    // create a new irtkQtImageObject and add it to the list
    newImage = new irtkQtImageObject(imageFileName);
    _imageObjects.append(newImage);
}

irtkQtTwoDimensionalViewer* irtkQtViewer::CreateTwoDimensionalViewer(irtkQtBaseViewer::irtkViewMode viewMode) {
    return new irtkQtTwoDimensionalViewer(viewMode);
}

irtkQtThreeDimensionalViewer* irtkQtViewer::CreateThreeDimensionalViewer() {
    return new irtkQtThreeDimensionalViewer();
}
