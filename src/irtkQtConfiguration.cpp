#include <irtkQtConfiguration.h>

#include <QFile>
#include <QMessageBox>
#include <QXmlStreamWriter>

// Initialize instance to null
irtkQtViewer* irtkQtViewer::viewInstance = NULL;

irtkQtViewer::irtkQtViewer() {
    _imageObjectList.clear();
}

void irtkQtViewer::DestroyImages() {
    while (!_imageObjectList.isEmpty())
        delete _imageObjectList.takeFirst();
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
    _imageObjectList.append(newImage);
}

void irtkQtViewer::ReadConfiguration(const QString fileName) {

}

void irtkQtViewer::WriteConfiguration(const QString fileName) {
    QFile xmlFile(fileName);
    if (xmlFile.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter stream(&xmlFile);

        stream.setAutoFormatting(true);
        stream.writeStartDocument();

        stream.writeStartElement("m3diview");

        stream.writeStartElement("images");
        QList<irtkQtImageObject*>::iterator it;
        for (it = _imageObjectList.begin(); it != _imageObjectList.end(); it++) {
            stream.writeStartElement("image");

            if ((*it)->IsVisible()) {
                stream.writeAttribute("visible", "true");
                stream.writeTextElement("minimumDisplay", QString::number((*it)->GetMinDisplayValue()));
                stream.writeTextElement("maximumDisplay", QString::number((*it)->GetMaxDisplayValue()));
                stream.writeTextElement("opacity", QString::number((*it)->GetOpacity()));
                stream.writeTextElement("colormap",
                                        irtkQtLookupTable::GetColorModeList().at((*it)->GetColormap()));
                stream.writeTextElement("interpolation",
                                        irtkQtImageObject::GetInterpolationModeList().at((*it)->GetInterpolation()));
            }
            else {
                stream.writeAttribute("visible", "false");
            }
            stream.writeTextElement("file", (*it)->GetPath());

            stream.writeEndElement();
        }
        stream.writeEndElement(); // images

        stream.writeEndElement(); // m3diview

        stream.writeEndDocument();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Could not open file");
        msgBox.setInformativeText(fileName);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}
