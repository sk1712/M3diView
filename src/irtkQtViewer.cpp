#include <irtkQtViewer.h>


// Singleton class, initialize instance to NULL
irtkQtViewer* irtkQtViewer::viewInstance = NULL;

irtkQtViewer::irtkQtViewer() {
    _imageObjects.clear();
}

void irtkQtViewer::DestroyImages() {
    while (!_imageObjects.isEmpty())
        delete _imageObjects.takeFirst();
}

irtkQtViewer* irtkQtViewer::Instance() {
    // If there is no other instance of this class create one
    if (!viewInstance)
        viewInstance = new irtkQtViewer;

    return viewInstance;
}

void irtkQtViewer::Destroy() {
    // If there is a view instance
    if (viewInstance) {
        // Delete all images in the list
        viewInstance->DestroyImages();
        // And then delete the instance itself
        delete viewInstance;
    }
}

void irtkQtViewer::CreateImage(QString imageFileName) {
    irtkQtImageObject* newImage = NULL;

    // Create a new irtkQtImageObject and add it to the list
    newImage = new irtkQtImageObject(imageFileName);
    _imageObjects.append(newImage);
}

irtkQtTwoDimensionalViewer* irtkQtViewer::CreateTwoDimensionalViewer(irtkQtBaseViewer::irtkViewMode viewMode) {
    return new irtkQtTwoDimensionalViewer(viewMode);
}

irtkQtThreeDimensionalViewer* irtkQtViewer::CreateThreeDimensionalViewer() {
    return new irtkQtThreeDimensionalViewer();
}
