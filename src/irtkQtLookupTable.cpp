#include <irtkQtLookupTable.h>

#include <math.h>

#include <QDebug>


irtkQtLookupTable::irtkQtLookupTable() {
    _alpha = 255;

    minImage = 0;
    maxImage = 0;
    minDisplay = minImage;
    maxDisplay = maxImage;

    _mode = MODE_LUMINANCE;

    lookupTable = new QRgb[256];
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
    for (int i = 0; i <= 255; i++) {
        lookupTable[i] = qRgba(i, 0, 0, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToGreen() {
    for (int i = 0; i <= 255; i++) {
        lookupTable[i] = qRgba(0, i, 0, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToBlue() {
    for (int i = 0; i <= 255; i++) {
        lookupTable[i] = qRgba(0, 0, i, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToLuminance() {   
    int minD = round((minDisplay - minImage) * 255 / (double) (maxImage - minImage));
    int maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

    qDebug() << "maximum real display is " << maxDisplay;
    qDebug() << "minimum real display is " << minDisplay;
    qDebug() << "minimum display is " << minD << " and maximum display is " << maxD;

    for (int i = 0; i < minD; i++) {
        lookupTable[i] = qRgba(0, 0, 0, _alpha);
    }
    for (int i = minD; i <= maxD; i++) {
        int value = round((i - minD) * 255 / (maxD - minD));
        lookupTable[i] = qRgba(value, value, value, _alpha);
    }
    for (int i = maxD + 1; i <= 255; i++) {
        lookupTable[i] = qRgba(255, 255, 255, _alpha);
    }
}
