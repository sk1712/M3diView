#ifndef IRTKQTLOOKUPTABLE_H
#define IRTKQTLOOKUPTABLE_H

#include <QColor>


class irtkQtLookupTable
{
public:

    /// Color modes
    enum irtkColorMode {MODE_RED, MODE_GREEN, MODE_BLUE, MODE_LUMINANCE, MODE_INVERSE};

    /// The corresponding lookup table
    QRgb *lookupTable;

private:

    /// Alpha coefficient
    int _alpha;

    /// Minimum and maximum display values
    double minDisplay, maxDisplay;

    /// Minimum and maximum image values
    double minImage, maxImage;

    /// Color mode
    irtkColorMode _mode;

    /// String list with names of color modes
    static QStringList _colorStringList;

public:

    /// Class constructor
    irtkQtLookupTable();

    /// Class destructor
    ~irtkQtLookupTable();

    /// Initialize look up table
    void Initialize();

    /// Fill the values of _colorStringList
    static void SetColorModeList();

    /// Get the values of _colorStringList
    static QStringList GetColorModeList();

    /// Set image alpha value
    void SetAlpha(int a);

    /// Get image alpha value
    int GetAlpha() const;

    /// Set image minimum displayed value
    void SetMinDisplayValue(double value);

    /// Get image minimum displayed value
    double GetMinDisplayValue() const;

    /// Set image maximum displayed value
    void SetMaxDisplayValue(double value);

    /// Get image maximum displayed value
    double GetMaxDisplayValue() const;

    /// Get actual image minimum value
    double GetImageMinValue() const;

    /// Get actual image maximum value
    double GetImageMaxValue() const;

    /// Set minimum and maximum values
    void SetMinMaxDisplayValues(double min, double max);

    /// Get minimum and maximum image values
    void GetMinMaxImageValues(double & min, double & max);

    /// Set minimum and maximum image values
    void SetMinMaxImageValues(double min, double max);

    /// Set color mode
    void SetColorMode(irtkColorMode mode);

    /// Get color mode
    int GetColorMode() const;

private:

    /// Red color mode
    void SetColorModeToRed();

    /// Green color mode
    void SetColorModeToGreen();

    /// Blue color mode
    void SetColorModeToBlue();

    /// Color mode luminance
    void SetColorModeToLuminance();

    /// Color mode inverse luminance
    void SetColorModeToInverse();
};


inline int irtkQtLookupTable::GetAlpha() const {
    return _alpha;
}

inline void irtkQtLookupTable::SetMinDisplayValue(double value) {
    minDisplay = value;
}

inline double irtkQtLookupTable::GetMinDisplayValue() const {
    return minDisplay;
}

inline void irtkQtLookupTable::SetMaxDisplayValue(double value) {
    maxDisplay = value;
}

inline double irtkQtLookupTable::GetMaxDisplayValue() const {
    return maxDisplay;
}

inline double irtkQtLookupTable::GetImageMinValue() const {
    return minImage;
}

inline double irtkQtLookupTable::GetImageMaxValue() const {
    return maxImage;
}

inline void irtkQtLookupTable::SetMinMaxDisplayValues(double min, double max) {
    minDisplay = min;
    maxDisplay = max;
}

inline void irtkQtLookupTable::GetMinMaxImageValues(double &min, double &max) {
    min = minImage;
    max = maxImage;
}

inline void irtkQtLookupTable::SetMinMaxImageValues(double min, double max) {
    minImage = min;
    maxImage = max;
}

inline int irtkQtLookupTable::GetColorMode() const {
    return _mode;
}

#endif // IRTKQTLOOKUPTABLE_H
