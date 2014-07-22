#ifndef IRTKQTLOOKUPTABLE_H
#define IRTKQTLOOKUPTABLE_H

#include <QColor>

/*
 * Image lookup table class
 */

class irtkQtLookupTable
{
public:

    /// Color modes
    enum irtkColorMode {MODE_RED, MODE_GREEN, MODE_BLUE, MODE_LUMINANCE, MODE_INVERSE,
                        MODE_JACOBIAN, MODE_JACOBIAN_EXPANSION, MODE_JACOBIAN_CONTRACTION,
                        MODE_HOTMETAL, MODE_RAINBOW};

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

    /// Initialize lookup table
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
    void SetMinDisplayValue(const double value);

    /// Get image minimum displayed value
    double GetMinDisplayValue() const;

    /// Set image maximum displayed value
    void SetMaxDisplayValue(const double value);

    /// Get image maximum displayed value
    double GetMaxDisplayValue() const;

    /// Get actual image minimum value
    double GetImageMinValue() const;

    /// Get actual image maximum value
    double GetImageMaxValue() const;

    /// Set minimum and maximum values
    void SetMinMaxDisplayValues(const double min, const double max);

    /// Get minimum and maximum image values
    void GetMinMaxImageValues(double & min, double & max);

    /// Set minimum and maximum image values
    void SetMinMaxImageValues(const double min, const double max);

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

    /// Color mode Jacobian
    void SetColorModeToJacobian();

    /// Color mode Jacobian expansion
    void SetColorModeToJacobianExpansion();

    /// Color mode Jacobian contraction
    void SetColorModeToJacobianContraction();

    /// Color mode hot metal
    void SetColorModeToHotmetal();

    /// Color mode rainbow
    void SetColorModeToRainbow();
};


inline int irtkQtLookupTable::GetAlpha() const {
    return _alpha;
}

inline void irtkQtLookupTable::SetMinDisplayValue(const double value) {
    minDisplay = value;
}

inline double irtkQtLookupTable::GetMinDisplayValue() const {
    return minDisplay;
}

inline void irtkQtLookupTable::SetMaxDisplayValue(const double value) {
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

inline void irtkQtLookupTable::SetMinMaxDisplayValues(const double min, const double max) {
    minDisplay = min;
    maxDisplay = max;
}

inline void irtkQtLookupTable::GetMinMaxImageValues(double &min, double &max) {
    min = minImage;
    max = maxImage;
}

inline void irtkQtLookupTable::SetMinMaxImageValues(const double min, const double max) {
    minImage = min;
    maxImage = max;
}

inline int irtkQtLookupTable::GetColorMode() const {
    return _mode;
}

#endif // IRTKQTLOOKUPTABLE_H
