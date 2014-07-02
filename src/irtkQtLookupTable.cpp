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
    case MODE_JACOBIAN:
        SetColorModeToJacobian();
        break;
    case MODE_JACOBIAN_EXPANSION:
        SetColorModeToJacobianExpansion();
        break;
    case MODE_JACOBIAN_CONTRACTION:
        SetColorModeToJacobianContraction();
        break;
    case MODE_HOTMETAL:
        SetColorModeToHotmetal();
        break;
    case MODE_RAINBOW:
        SetColorModeToRainbow();
        break;
    default:
        qCritical("Unknown color mode");
    }

}

void irtkQtLookupTable::SetColorModeList() {
    // Colors should be in the same order as in irtkColor mode enumeration

    _colorStringList << "Red"
                     << "Green"
                     << "Blue"
                     << "Grey"
                     << "Inverse"
                     << "Jacobian"
                     << "Jacobian/Expansion"
                     << "Jacobian/Contraction"
                     << "Hotmetal"
                     << "Rainbow";
}

QStringList irtkQtLookupTable::GetColorModeList() {
    return _colorStringList;
}

void irtkQtLookupTable::SetColorMode(irtkColorMode mode) {
    _mode = mode;
}

void irtkQtLookupTable::SetColorModeToRed() {
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

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
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

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
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

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
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

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
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

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

void irtkQtLookupTable::SetColorModeToJacobian() {
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));
    double hundred = (100.0 - minImage) * 255 / (maxImage - minImage);

    QColor color;

    for (int i = 0; i < ((minD < hundred) ? minD : hundred); i++) {
        color.setHsvF(180.0 / 360.0, 1, 1);
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
    for (int i = minD; i <= hundred; i++) {
        color.setHsvF(- (i - hundred) / (double)(minD - hundred) * 60.0 / 360.0 + 240.0 / 360.0, 1, 1);
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
    for (int i = hundred; i <= maxD; i++) {
        color.setHsvF((i - hundred) / (double)(maxD - hundred) * 60.0 / 360.0, 1, 1);
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
    for (int i = ((maxD > hundred) ? maxD : hundred); i <= 255; i++) {
        color.setHsvF(60.0 / 360.0, 1, 1);
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToJacobianExpansion() {
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));
    double hundred = (100.0 - minImage) * 255 / (maxImage - minImage);

    int i, _min, _max;
    _min = ((minD > hundred) ? minD : hundred);
    _max = ((maxD > hundred) ? maxD : hundred);

    QColor color;

    for (i = 0; i < _min; i++) {
        lookupTable[i] = qRgba(0, 0, 0, 0);
    }
    for (i = _min; i <= _max; i++) {
        color.setHsvF((i - _min) / (double)(_max - _min + 1) * 60.0 / 360.0, 1, 1,
                      (i - _min) / (double)(_max - _min + 1));
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
    for (i = _max+1; i <= 255; i++) {
        color.setHsvF(60.0 / 360.0, 1, 1);
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
}

void irtkQtLookupTable::SetColorModeToJacobianContraction() {
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));
    double hundred = (100.0 - minImage) * 255 / (maxImage - minImage);

    int i, _min, _max;
    _min = ((minD < hundred) ? minD : hundred);
    _max = ((maxD < hundred) ? maxD : hundred);

    QColor color;

    for (i = 0; i < _min; i++) {
        color.setHsvF(180.0 / 360.0, 1, 1);
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
    for (i = _min; i <= _max; i++) {
        color.setHsvF((i - _max) / (double)(_max - _min + 1) * 60.0 / 360.0 + 240.0 / 360.0, 1, 1,
                      -(i - _max) / (double)(_max - _min + 1));
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
    for (i = _max+1; i <= 255; i++) {
        lookupTable[i] = qRgba(0, 0, 0, 0);
    }
}

void irtkQtLookupTable::SetColorModeToHotmetal() {
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

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

void irtkQtLookupTable::SetColorModeToRainbow() {
    double minD = round((minDisplay - minImage) * 255 / (maxImage - minImage));
    double maxD = round((maxDisplay - minImage) * 255 / (maxImage - minImage));

    QColor color;

    for (int i = 0; i < minD; i++) {
        color.setHsvF(2.0/3.0, 1, 1);
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
    for (int i = minD; i <= maxD; i++) {
        color.setHsvF((maxD - i) / (double)(maxD - minD) * 2.0/3.0, 1, 1);
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
    for (int i = maxD+1; i <= 255; i++) {
        color.setHsvF(0, 1, 1);
        lookupTable[i] = qRgba(color.red(), color.green(), color.blue(), _alpha);
    }
}
