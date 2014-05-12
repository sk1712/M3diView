#include <irtkQtLookupTable.h>

#include <QDebug>

irtkQtLookupTable::irtkQtLookupTable(int min, int max) {
    _alpha = 255;
    minDisplay = 0;
    maxDisplay = 0;
    _mode = MODE_LUMINANCE;
    lookupTable = new QRgb[max-min+1];
    Initialize();
}

irtkQtLookupTable::~irtkQtLookupTable() {
    delete lookupTable;
}

void irtkQtLookupTable::SetAlpha(int a) {
    _alpha = a;
    Initialize();
}

void irtkQtLookupTable::Initialize() {
    switch (_mode) {
    case MODE_RED:
        SetColorModeToRed();
        break;
    case MODE_GREEN:
        SetColorModeToGreen();
        break;
    case MODE_BLUE:
        SetColorModeToBlue();
        break;
    case MODE_LUMINANCE:
        SetColorModeToLuminance();
        break;
    }
}

void irtkQtLookupTable::SetColorMode(irtkColorMode mode) {
    _mode = mode;
    Initialize();
}

void irtkQtLookupTable::SetColorModeToRed() {
    qDebug() << "setting color mode to red";
    for (int i = 0; i <= 255; i++) {
        lookupTable[i] = qRgba(0, 0, i, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToGreen() {
    qDebug() << "setting color mode to green";
    for (int i = 0; i <= 255; i++) {
        lookupTable[i] = qRgba(0, i, 0, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToBlue() {
    qDebug() << "setting color mode to blue";
    for (int i = 0; i <= 255; i++) {
        lookupTable[i] = qRgba(i, 0, 0, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToLuminance() {
    qDebug() << "setting color mode to luminance";
    for (int i = 0; i <= 255; i++) {
        lookupTable[i] = qRgba(i, i, i, _alpha);
    }
}
