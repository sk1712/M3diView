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

void irtkQtConfiguration::ReadImages(QXmlStreamReader &xmlReader) {
    _imageList.clear();

    while (! (xmlReader.isEndElement() && (xmlReader.name() == "images") ) ) {
        xmlReader.readNext();

        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "image") {
                irtkQtConfigurationImage image;
                _imageList.push_back(image);
                QXmlStreamAttributes attr = xmlReader.attributes();
                _imageList.back().visible = (attr.value("visible").toString() == "true");
            }
            else if (xmlReader.name() == "minimumDisplay") {
                _imageList.back().minDisplay = xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "maximumDisplay") {
                _imageList.back().maxDisplay = xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "opacity") {
                _imageList.back().opacity = xmlReader.readElementText().toInt();
            }
            else if (xmlReader.name() == "colormap") {
                _imageList.back().colormap = xmlReader.readElementText();
            }
            else if (xmlReader.name() == "interpolation") {
                _imageList.back().interpolation = xmlReader.readElementText();
            }
            else if (xmlReader.name() == "file") {
                _imageList.back().fileName = xmlReader.readElementText();
            }
        }
    }
}

void irtkQtConfiguration::ReadViewers(QXmlStreamReader &xmlReader) {
    _viewerList.clear();

    while (! (xmlReader.isEndElement() && (xmlReader.name() == "viewers") ) ) {
        xmlReader.readNext();

        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "viewer") {
                irtkQtConfigurationViewer viewer;
                _viewerList.push_back(viewer);

                QXmlStreamAttributes attr = xmlReader.attributes();
                _viewerList.back().fullScreen = (attr.value("fullScreen").toString() == "true");
                _viewerList.back().linked = (attr.value("linked").toString() == "true");
                _viewerList.back().labelsVisible = (attr.value("labelsVisible").toString() == "true");
                _viewerList.back().cursorVisible = (attr.value("cursorVisible").toString() == "true");
                _viewerList.back().imageOriginVisible = (attr.value("originVisible").toString() == "true");
            }
            else if (xmlReader.name() == "type") {
                _viewerList.back().type = xmlReader.readElementText();
            }
            else if (xmlReader.name() == "origin_x") {
                _viewerList.back().origin[0] = xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "origin_y") {
                _viewerList.back().origin[1] = xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "origin_z") {
                _viewerList.back().origin[2] = xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "resolution_x") {
                _viewerList.back().resolution[0] = xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "resolution_y") {
                _viewerList.back().resolution[1] = xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "resolution_z") {
                _viewerList.back().resolution[2] = xmlReader.readElementText().toDouble();
            }
        }
    }
}

void irtkQtConfiguration::WriteImages(QXmlStreamWriter &xmlWriter) {
    xmlWriter.writeStartElement("images");

    QList<irtkQtImageObject*>::iterator it;
    for (it = _imageObjectList.begin(); it != _imageObjectList.end(); ++it) {
        xmlWriter.writeStartElement("image");

        if ((*it)->IsVisible()) {
            xmlWriter.writeAttribute("visible", "true");
            xmlWriter.writeTextElement("minimumDisplay",
                                       QString::number((*it)->GetMinDisplayValue()));
            xmlWriter.writeTextElement("maximumDisplay",
                                       QString::number((*it)->GetMaxDisplayValue()));
            xmlWriter.writeTextElement("opacity",
                                       QString::number((*it)->GetOpacity()));
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
    for (v_it = _viewerList.begin(); v_it != _viewerList.end(); ++v_it) {
        xmlWriter.writeStartElement("viewer");

        xmlWriter.writeAttribute("fullScreen",
                                 (v_it->fullScreen) ? "true" : "false");
        xmlWriter.writeAttribute("labelsVisible",
                                 (v_it->labelsVisible) ? "true" : "false");
        xmlWriter.writeAttribute("cursorVisible",
                                 (v_it->cursorVisible) ? "true" : "false");
        xmlWriter.writeAttribute("originVisible",
                                 (v_it->imageOriginVisible) ? "true" : "false");
        xmlWriter.writeAttribute("linked",
                                 (v_it->linked) ? "true" : "false");

        xmlWriter.writeTextElement("type", v_it->type);

        xmlWriter.writeTextElement("origin_x", QString::number(v_it->origin[0]));
        xmlWriter.writeTextElement("origin_y", QString::number(v_it->origin[1]));
        xmlWriter.writeTextElement("origin_z", QString::number(v_it->origin[2]));

        xmlWriter.writeTextElement("resolution_x", QString::number(v_it->resolution[0]));
        xmlWriter.writeTextElement("resolution_y", QString::number(v_it->resolution[1]));
        xmlWriter.writeTextElement("resolution_z", QString::number(v_it->resolution[2]));

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
    newImage = new irtkQtImageObject();
    newImage->setImagePath(imageFileName);
    _imageObjectList.append(newImage);
}

bool irtkQtConfiguration::Read(const QString fileName) {
    QFile inFile(fileName);

    if (inFile.open(QIODevice::ReadOnly)) {
        QXmlStreamReader xml(&inFile);

        if (xml.readNextStartElement()) {
            if (xml.name() != "m3diview") {
                return false;
            }
        }

        while (xml.readNextStartElement()) {
            if (xml.name() == "images") {
                ReadImages(xml);
            }
            else if (xml.name() == "viewers") {
                ReadViewers(xml);
            }
        }

        if (xml.hasError()) {
            return false;
        }
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Could not open file");
        msgBox.setInformativeText(fileName);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();

        return false;
    }

    return true;
}

void irtkQtConfiguration::Write(const QString fileName) {
    QFile outFile(fileName);

    if (outFile.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter xml(&outFile);
        xml.setAutoFormatting(true);

        // Start xml document
        xml.writeStartDocument();

        xml.writeStartElement("m3diview");
        WriteImages(xml);
        WriteViewers(xml);
        xml.writeEndElement(); // m3diview

        // End xml document
        xml.writeEndDocument();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Could not open file");
        msgBox.setInformativeText(fileName);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}
