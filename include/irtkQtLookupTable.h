#ifndef IRTKQTLOOKUPTABLE_H
#define IRTKQTLOOKUPTABLE_H

#include <QColor>

class irtkQtLookupTable
{
    /// alpha coefficient
    int alpha;

public:

    QRgb *lookupTable;

    /// class constructor
    irtkQtLookupTable(int min = 0, int max = 255);

    /// class destructor
    ~irtkQtLookupTable();

    /// set alpha value for the look up table
    void SetAlpha(int a);

    /// initialize look up table
    void Initialize();
};


#endif // IRTKQTLOOKUPTABLE_H
