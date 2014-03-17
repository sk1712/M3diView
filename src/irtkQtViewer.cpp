#include <irtkQtViewer.h>

#include <cstdio>

irtkQtViewer* irtkQtViewer::viewInstance = NULL;
irtkImage* irtkQtViewer::_targetImage = NULL;
irtkImage* irtkQtViewer::_sourceImage = NULL;

irtkQtViewer::irtkQtViewer() {
//    _targetImage = NULL;
//    _sourceImage = NULL;
}

irtkQtViewer* irtkQtViewer::Instance() {
    if (!viewInstance)
        viewInstance = new irtkQtViewer;

    return viewInstance;
}

void irtkQtViewer::CreateTargetImage(string imageFileName) {
    _targetImage = irtkImage::New(imageFileName.c_str());
    printf("Creating target image \n");
}

void irtkQtViewer::DestroyTargetImage() {
    delete _targetImage;
}

void irtkQtViewer::CreateSourceImage(string imageFileName) {
    _sourceImage = irtkImage::New(imageFileName.c_str());
    printf("Creating source image \n");
}

void irtkQtViewer::DestroySourceImage() {
    delete _sourceImage;
}
