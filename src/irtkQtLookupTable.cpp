#include <irtkQtLookupTable.h>

irtkQtLookupTable::irtkQtLookupTable(int min, int max) {
    _alpha = 255;
    minDisplay = 0;
    maxDisplay = 0;
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
    for (int i = 0; i <= 255; i ++) {
        lookupTable[i] = qRgba(i, i, i, _alpha);
    }
}
