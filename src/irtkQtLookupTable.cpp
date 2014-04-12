#include <irtkQtLookupTable.h>

irtkQtLookupTable::irtkQtLookupTable(int min, int max) {
    alpha = 255;
    lookupTable = new QRgb[max-min+1];
}

irtkQtLookupTable::~irtkQtLookupTable() {
    delete lookupTable;
}

void irtkQtLookupTable::SetAlpha(int a) {
    alpha = a;
    Initialize();
}

void irtkQtLookupTable::Initialize() {
    for (int i = 0; i <= 255; i ++) {
        lookupTable[i] = qRgba(i, i, i, alpha);
    }
}
