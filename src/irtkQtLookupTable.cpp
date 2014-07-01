#include <irtkQtLookupTable.h>

#include <math.h>


QStringList irtkQtLookupTable::_colorStringList;

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
    case MODE_INVERSE:
        SetColorModeToInverse();
        break;
    case MODE_HOTMETAL:
        SetColorModeToHotmetal();
    default:
        qCritical("Unknown color mode");
    }

}

void irtkQtLookupTable::SetColorModeList() {
    _colorStringList << "Red"
                     << "Green"
                     << "Blue"
                     << "Grey"
                     << "Inverse"
                     << "Hotmetal";
}

QStringList irtkQtLookupTable::GetColorModeList() {
    return _colorStringList;
}

void irtkQtLookupTable::SetColorMode(irtkColorMode mode) {
    _mode = mode;
}

void irtkQtLookupTable::SetColorModeToRed() {
    int minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    int maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

    for (int i = 0; i < minD; i++) {
        lookupTable[i] = qRgba(0, 0, 0, _alpha);
    }
    for (int i = minD; i <= maxD; i++) {
        int value = round((i - minD) * 255 / (maxD - minD));
        lookupTable[i] = qRgba(value, 0, 0, _alpha);
    }
    for (int i = maxD + 1; i <= 255; i++) {
        lookupTable[i] = qRgba(255, 0, 0, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToGreen() {
    int minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    int maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

    for (int i = 0; i < minD; i++) {
        lookupTable[i] = qRgba(0, 0, 0, _alpha);
    }
    for (int i = minD; i <= maxD; i++) {
        int value = round((i - minD) * 255 / (maxD - minD));
        lookupTable[i] = qRgba(0, value, 0, _alpha);
    }
    for (int i = maxD + 1; i <= 255; i++) {
        lookupTable[i] = qRgba(0, 255, 0, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToBlue() {
    int minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    int maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

    for (int i = 0; i < minD; i++) {
        lookupTable[i] = qRgba(0, 0, 0, _alpha);
    }
    for (int i = minD; i <= maxD; i++) {
        int value = round((i - minD) * 255 / (maxD - minD));
        lookupTable[i] = qRgba(0, 0, value, _alpha);
    }
    for (int i = maxD + 1; i <= 255; i++) {
        lookupTable[i] = qRgba(0, 0, 255, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToLuminance() {   
    int minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    int maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

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

void irtkQtLookupTable::SetColorModeToInverse() {
    int minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    int maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

    for (int i = 0; i < minD; i++) {
        lookupTable[i] = qRgba(255, 255, 255, _alpha);
    }
    for (int i = minD; i <= maxD; i++) {
        int value = round((maxD - i) * 255 / (maxD - minD));
        lookupTable[i] = qRgba(value, value, value, _alpha);
    }
    for (int i = maxD + 1; i <= 255; i++) {
        lookupTable[i] = qRgba(0, 0, 0, _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToHotmetal() {
    int minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    int maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

    for (int i = 0; i < minD; i++) {
        lookupTable[i] = qRgba(0, 0, 0, _alpha);
    }
    for (int i = minD; i <= maxD; i++) {
        int value = round((i - minD) * 255 / (maxD - minD));
        lookupTable[i] = qRgba(255, value, 0, _alpha);
    }
    for (int i = maxD + 1; i <= 255; i++) {
        lookupTable[i] = qRgba(255, 255, 0, _alpha);
    }
}
