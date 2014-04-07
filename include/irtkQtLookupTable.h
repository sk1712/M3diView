#ifndef IRTKQTLOOKUPTABLE_H
#define IRTKQTLOOKUPTABLE_H

#include <QColor>

class irtkQtLookupTable
{
public:

    QRgb *lookupTable;

    irtkQtLookupTable(int min = 0, int max = 255);

    ~irtkQtLookupTable();
};


#endif // IRTKQTLOOKUPTABLE_H
