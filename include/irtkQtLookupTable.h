#ifndef IRTKQTLOOKUPTABLE_H
#define IRTKQTLOOKUPTABLE_H

#include <QColor>

class irtkQtLookupTable
{
    int alpha;

public:

    QRgb *lookupTable;

    irtkQtLookupTable(int min = 0, int max = 255);

    ~irtkQtLookupTable();

    void SetAlpha(int a);

    void Initialize();
};


#endif // IRTKQTLOOKUPTABLE_H
