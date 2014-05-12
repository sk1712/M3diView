#ifndef IRTKQTLOOKUPTABLE_H
#define IRTKQTLOOKUPTABLE_H

#include <QColor>


class irtkQtLookupTable
{
    /// alpha coefficient
    int _alpha;

    /// minimum and maximum display values
    int minDisplay, maxDisplay;

    /// minimum and maximum image values
    double minImage, maxImage;

public:

    QRgb *lookupTable;

    /// class constructor
    irtkQtLookupTable(int min = 0, int max = 255);

    /// class destructor
    ~irtkQtLookupTable();

    /// initialize look up table
    void Initialize();

    /// set image alpha value
    void SetAlpha(int a);

    /// get image alpha value
    int GetAlpha() const;

    /// set image minimum displayed value
    void SetMinDisplayValue(int value);

    /// get image minimum displayed value
    int GetMinDisplayValue() const;

    /// set image maximum displayed value
    void SetMaxDisplayValue(int value);

    /// get image maximum displayed value
    int GetMaxDisplayValue() const;

    /// get actual image minimum value
    int GetImageMinValue() const;

    /// get actual image maximum value
    int GetImageMaxValue() const;

    /// set minimum and maximum values
    void SetMinMaxDisplayValues(int min, int max);

    /// set minimum and maximum image values
    void SetMinMaxImageValues(double min, double max);
};


inline int irtkQtLookupTable::GetAlpha() const {
    return _alpha;
}

inline void irtkQtLookupTable::SetMinDisplayValue(int value) {
    minDisplay = value;
}

inline int irtkQtLookupTable::GetMinDisplayValue() const {
    return minDisplay;
}

inline void irtkQtLookupTable::SetMaxDisplayValue(int value) {
    maxDisplay = value;
}

inline int irtkQtLookupTable::GetMaxDisplayValue() const {
    return maxDisplay;
}

inline int irtkQtLookupTable::GetImageMinValue() const {
    return minImage;
}

inline int irtkQtLookupTable::GetImageMaxValue() const {
    return maxImage;
}

inline void irtkQtLookupTable::SetMinMaxDisplayValues(int min, int max) {
    minDisplay = min;
    maxDisplay = max;
}

inline void irtkQtLookupTable::SetMinMaxImageValues(double min, double max) {
    minImage = min;
    maxImage = max;
}

#endif // IRTKQTLOOKUPTABLE_H
