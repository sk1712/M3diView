#include <irtkQtViewer.h>

#include <QFile>
#include <QXmlStreamWriter>

// Initialize instance to null
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

void irtkQtViewer::ReadConfiguration(const QString fileName) {

}

void irtkQtViewer::WriteConfiguration(const QString fileName) {
//    imageList.clear();
//    viewerList.clear();

    QFile xmlFile(fileName);
    if (xmlFile.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter stream(&xmlFile);

        stream.setAutoFormatting(true);
        stream.writeStartDocument();

        stream.writeStartElement("images");
        QList<irtkQtImageObject*>::iterator it;
        for (it = _imageObjects.begin(); it != _imageObjects.end(); it++) {
            stream.writeStartElement("image");

            if ((*it)->IsVisible()) {
                stream.writeAttribute("visible", "true");
            }
            else {
                stream.writeAttribute("visible", "false");
            }
            stream.writeTextElement("file", (*it)->GetPath());
            stream.writeTextElement("minimumDisplay", QString::number((*it)->GetMinDisplayValue()));
            stream.writeTextElement("maximumDisplay", QString::number((*it)->GetMaxDisplayValue()));
            stream.writeTextElement("opacity", QString::number((*it)->GetOpacity()));
            stream.writeTextElement("interpolation", QString::number((*it)->GetInterpolation()));

            stream.writeEndElement();
        }
        stream.writeEndElement();

        stream.writeEndDocument();
    }
}
