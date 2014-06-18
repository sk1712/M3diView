#ifndef IRTKQTIMAGEOBJECT_H
#define IRTKQTIMAGEOBJECT_H

#include <irtkImage.h>
#include <irtkTransformation.h>

#include <irtkQtLookupTable.h>

#include <QString>


class irtkQtImageObject
{

    /// Flag showing if image is visible
    bool _visible;

    /// Image full path
    QString _path;

    /// Image file name
    QString _fileName;

    /// Corresponding irtk image
    irtkImage* _image;

    /// Corresponding lookuptable
    irtkQtLookupTable* _lookupTable;

public:

    /// Image object constructor
    irtkQtImageObject(const QString &path);

    /// Image object destructor
    ~irtkQtImageObject();

    /// Check if image is visible
    bool IsVisible() const;

    /// Set if image visible
    void SetVisible(bool visible);

    /// Get image filename
    QString GetFileName() const;

    /// Get image full path
    QString GetPath() const;

    /// Get image
    irtkImage* GetImage() const;

    /// Get lookup table
    irtkQtLookupTable* GetLookupTable() const;

    /// Create image object
    void CreateImage();

    /// Delete image and lookup table
    void DeleteImage();

    /// Set opacity for the corresponding image
    void SetOpacity(int value);

    /// Get opacity for the corresponding image
    int GetOpacity();

    /// Get minimum value for the corresponding image
    double GetMinImageValue();

    /// Get maximum value for the corresponding image
    double GetMaxImageValue();

    /// Get minimum display value for the corresponding image
    void SetMinDisplayValue(double min);

    /// Get minimum display value for the corresponding image
    double GetMinDisplayValue() const;

    /// Set maximum display value for the corresponding image
    void SetMaxDisplayValue(double max);

    /// Get maximum display value for the corresponding image
    double GetMaxDisplayValue() const;

    /// Set colormap for the corresponding image
    void SetColormap(irtkQtLookupTable::irtkColorMode mode);

    /// Get colormap for the corresponding image
    int GetColormap() const;

    /// Convert image to the desired coordinate system
    void ConvertImageToTarget();
};


inline bool irtkQtImageObject::IsVisible() const {
    return _visible;
}

inline void irtkQtImageObject::SetVisible(bool visible) {
    _visible = visible;
}

inline QString irtkQtImageObject::GetFileName() const {
    return _fileName;
}

inline QString irtkQtImageObject::GetPath() const {
    return _path;
}

inline irtkImage* irtkQtImageObject::GetImage() const {
    return _image;
}

inline irtkQtLookupTable* irtkQtImageObject::GetLookupTable() const {
    return _lookupTable;
}

inline void irtkQtImageObject::SetOpacity(int value) {
    if (_lookupTable) {
        _lookupTable->SetAlpha(value);
        _lookupTable->Initialize();
    }
}

inline int irtkQtImageObject::GetOpacity() {
    int opacity = 0;

    if (_lookupTable) {
        opacity = _lookupTable->GetAlpha();
    }

    return opacity;
}

inline double irtkQtImageObject::GetMinImageValue() {
    double minValue = 0;

    if (_lookupTable) {
        minValue = _lookupTable->GetImageMinValue();
    }

    return minValue;
}

inline double irtkQtImageObject::GetMaxImageValue() {
    double maxValue = 0;

    if (_lookupTable) {
        maxValue = _lookupTable->GetImageMaxValue();
    }

    return maxValue;
}

inline void irtkQtImageObject::SetMinDisplayValue(double min) {
    if (_lookupTable) {
        _lookupTable->SetMinDisplayValue(min);
        _lookupTable->Initialize();
    }
}

inline double irtkQtImageObject::GetMinDisplayValue() const {
    double minDisplay = 0;

    if (_lookupTable) {
        minDisplay = _lookupTable->GetMinDisplayValue();
    }

    return minDisplay;
}

inline void irtkQtImageObject::SetMaxDisplayValue(double max) {
    if (_lookupTable) {
        _lookupTable->SetMaxDisplayValue(max);
        _lookupTable->Initialize();
    }
}

inline double irtkQtImageObject::GetMaxDisplayValue() const {
    double maxDisplay = 0;

    if (_lookupTable) {
        maxDisplay = _lookupTable->GetMaxDisplayValue();
    }

    return maxDisplay;
}

inline void irtkQtImageObject::SetColormap(irtkQtLookupTable::irtkColorMode mode) {
    if (_lookupTable) {
        _lookupTable->SetColorMode(mode);
        _lookupTable->Initialize();
    }
}

inline int irtkQtImageObject::GetColormap() const {
    int colormapIndex = -1;

    if (_lookupTable) {
        colormapIndex = _lookupTable->GetColorMode();
    }

    return colormapIndex;
}

#endif // IRTKQTIMAGEOBJECT_H
