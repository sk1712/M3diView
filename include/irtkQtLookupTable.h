#ifndef IRTKQTLOOKUPTABLE_H
#define IRTKQTLOOKUPTABLE_H

#include <QColor>


class irtkQtLookupTable
{
public:

    /// color modes
    enum irtkColorMode {MODE_RED, MODE_GREEN, MODE_BLUE, MODE_LUMINANCE};

    /// the corresponding lookup table
    QRgb *lookupTable;

private:

    /// alpha coefficient
    int _alpha;

    /// minimum and maximum display values
    double minDisplay, maxDisplay;

    /// minimum and maximum image values
    double minImage, maxImage;

    /// color mode
    irtkColorMode _mode;

public:

    /// class constructor
    irtkQtLookupTable();

    /// class destructor
    ~irtkQtLookupTable();

    /// initialize look up table
    void Initialize();

    /// set image alpha value
    void SetAlpha(int a);

    /// get image alpha value
    int GetAlpha() const;

    /// set image minimum displayed value
    void SetMinDisplayValue(double value);

    /// get image minimum displayed value
    double GetMinDisplayValue() const;

    /// set image maximum displayed value
    void SetMaxDisplayValue(double value);

    /// get image maximum displayed value
    double GetMaxDisplayValue() const;

    /// get actual image minimum value
    double GetImageMinValue() const;

    /// get actual image maximum value
    double GetImageMaxValue() const;

    /// set minimum and maximum values
    void SetMinMaxDisplayValues(double min, double max);

    /// set minimum and maximum image values
    void SetMinMaxImageValues(double min, double max);

    /// set color mode
    void SetColorMode(irtkColorMode mode);

    /// get color mode
    int GetColorMode() const;

private:

    void SetColorModeToRed();

    void SetColorModeToGreen();

    void SetColorModeToBlue();

    void SetColorModeToLuminance();
};


inline int irtkQtLookupTable::GetAlpha() const {
    return _alpha;
}

inline void irtkQtLookupTable::SetMinDisplayValue(double value) {
    minDisplay = value;
    Initialize();
}

inline double irtkQtLookupTable::GetMinDisplayValue() const {
    return minDisplay;
}

inline void irtkQtLookupTable::SetMaxDisplayValue(double value) {
    maxDisplay = value;
    Initialize();
}

inline double irtkQtLookupTable::GetMaxDisplayValue() const {
    return maxDisplay;
}

// TO DO : decide whether to use double or integer
inline double irtkQtLookupTable::GetImageMinValue() const {
    return minImage;
}

// TO DO : decide whether to use double or integer
inline double irtkQtLookupTable::GetImageMaxValue() const {
    return maxImage;
}

inline void irtkQtLookupTable::SetMinMaxDisplayValues(double min, double max) {
    minDisplay = min;
    maxDisplay = max;
}

inline void irtkQtLookupTable::SetMinMaxImageValues(double min, double max) {
    minImage = min;
    maxImage = max;
}

inline int irtkQtLookupTable::GetColorMode() const {
    return _mode;
}

#endif // IRTKQTLOOKUPTABLE_H
