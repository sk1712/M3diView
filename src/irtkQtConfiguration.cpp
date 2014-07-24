#include <irtkQtConfiguration.h>

#include <QFile>
#include <QMessageBox>
#include <QXmlStreamWriter>

// Initialize instance to null
irtkQtConfiguration* irtkQtConfiguration::viewInstance = NULL;

irtkQtConfiguration::irtkQtConfiguration() {
    _imageObjectList.clear();
}

void irtkQtConfiguration::DestroyImages() {
    while (!_imageObjectList.isEmpty())
        delete _imageObjectList.takeFirst();
}

void irtkQtConfiguration::WriteImages(QXmlStreamWriter &xmlWriter) {
    xmlWriter.writeStartElement("images");

    QList<irtkQtImageObject*>::iterator it;
    for (it = _imageObjectList.begin(); it != _imageObjectList.end(); it++) {
        xmlWriter.writeStartElement("image");

        if ((*it)->IsVisible()) {
            xmlWriter.writeAttribute("visible", "true");
            xmlWriter.writeTextElement("minimumDisplay", QString::number((*it)->GetMinDisplayValue()));
            xmlWriter.writeTextElement("maximumDisplay", QString::number((*it)->GetMaxDisplayValue()));
            xmlWriter.writeTextElement("opacity", QString::number((*it)->GetOpacity()));
            xmlWriter.writeTextElement("colormap",
                                    irtkQtLookupTable::GetColorModeList().at((*it)->GetColormap()));
            xmlWriter.writeTextElement("interpolation",
                                    irtkQtImageObject::GetInterpolationModeList().at((*it)->GetInterpolation()));
        }
        else {
            xmlWriter.writeAttribute("visible", "false");
        }

        xmlWriter.writeTextElement("file", (*it)->GetPath());
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement(); // images
}

void irtkQtConfiguration::WriteViewers(QXmlStreamWriter &xmlWriter) {
    xmlWriter.writeStartElement("viewers");

    QList<irtkQtConfigurationViewer>::iterator v_it;
    for (v_it = _viewerList.begin(); v_it != _viewerList.end(); v_it++) {
        xmlWriter.writeStartElement("viewer");

        xmlWriter.writeAttribute("fullScreen",
                                 (v_it->fullScreen) ? "true" : "false");
        xmlWriter.writeAttribute("labelsVisible",
                                 (v_it->labelVisible) ? "true" : "false");
        xmlWriter.writeAttribute("cursorVisible",
                                 (v_it->cursorVisible) ? "true" : "false");
        xmlWriter.writeAttribute("linked",
                                 (v_it->linked) ? "true" : "false");

        xmlWriter.writeTextElement("type", v_it->type);

        xmlWriter.writeStartElement("origin");
        xmlWriter.writeTextElement("x", QString::number(v_it->origin[0]));
        xmlWriter.writeTextElement("y", QString::number(v_it->origin[1]));
        xmlWriter.writeTextElement("z", QString::number(v_it->origin[2]));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("resolution");
        xmlWriter.writeTextElement("x", QString::number(v_it->resolution[0]));
        xmlWriter.writeTextElement("y", QString::number(v_it->resolution[1]));
        xmlWriter.writeTextElement("z", QString::number(v_it->resolution[2]));
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
}

irtkQtConfiguration* irtkQtConfiguration::Instance() {
    // If there is no other instance of this class create one
    if (!viewInstance)
        viewInstance = new irtkQtConfiguration;

    return viewInstance;
}

void irtkQtConfiguration::Destroy() {
    // If there is a view instance
    if (viewInstance) {
        // Delete all images in the list
        viewInstance->DestroyImages();
        // And then delete the instance itself
        delete viewInstance;
    }
}

void irtkQtConfiguration::CreateImage(QString imageFileName) {
    irtkQtImageObject* newImage = NULL;

    // Create a new irtkQtImageObject and add it to the list
    newImage = new irtkQtImageObject(imageFileName);
    _imageObjectList.append(newImage);
}

void irtkQtConfiguration::Read(const QString fileName) {

}

void irtkQtConfiguration::Write(const QString fileName) {
    QFile xmlFile(fileName);
    if (xmlFile.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter stream(&xmlFile);
        stream.setAutoFormatting(true);

        // Start xml document
        stream.writeStartDocument();

        stream.writeStartElement("m3diview");
        WriteImages(stream);
        WriteViewers(stream);
        stream.writeEndElement(); // m3diview

        // End xml document
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
