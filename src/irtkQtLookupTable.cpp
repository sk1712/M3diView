#include <irtkQtLookupTable.h>

irtkQtLookupTable::irtkQtLookupTable(int min, int max) {
    lookupTable = new QRgb[max-min+1];

    for (int i = 0; i <= 255; i ++) {
        lookupTable[i] = qRgb(i, i, i);
    }
}

irtkQtLookupTable::~irtkQtLookupTable() {
    delete lookupTable;
}
