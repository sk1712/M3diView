#ifndef IRTKQTIMAGEOBJECT_H
#define IRTKQTIMAGEOBJECT_H

#include <irtkImage.h>

#include <irtkQtLookupTable.h>

#include <QString>


class irtkQtImageObject
{

    /// flag showing if image is visible
    bool _visible;

    /// image full path
    QString _path;

    /// image file name
    QString _fileName;

    /// the corresponding irtk image
    irtkImage* _image;

    /// the corresponding lookuptable
    irtkQtLookupTable* _lookupTable;

public:

    /// image object constructor
    irtkQtImageObject(const QString &path);

    /// image object destructor
    ~irtkQtImageObject();

    /// check if image is visible
    bool IsVisible() const;

    /// set if image visible
    void SetVisible(bool visible);

    /// get image filename
    QString GetFileName() const;

    /// get image full path
    QString GetPath() const;

    /// get image
    irtkImage* GetImage() const;

    /// get lookup table
    irtkQtLookupTable* GetLookupTable() const;

    /// create image object
    void CreateImage();

    /// delete image and lookup table
    void DeleteImage();

    /// set opacity for the corresponding image
    void SetOpacity(int value);

    /// get opacity for the corresponding image
    int GetOpacity();

    /// get minimum value for the corresponding image
    double GetMinImageValue();

    /// get maximum value for the corresponding image
    double GetMaxImageValue();

    /// set minimum display value for the corresponding image
    void SetMinDisplayValue(double min);

    /// get minimum display value for the corresponding image
    double GetMinDisplayValue();

    /// set maximum display value for the corresponding image
    void SetMaxDisplayValue(double max);

    /// get maximum display value for the corresponding image
    double GetMaxDisplayValue() const;

    /// set colormap for the corresponding image
    void SetColormap(irtkQtLookupTable::irtkColorMode mode);

    /// get colormap for the corresponding image
    int GetColormap() const;
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
    }
}

inline double irtkQtImageObject::GetMinDisplayValue() {
    double minDisplay = 0;

    if (_lookupTable) {
        minDisplay = _lookupTable->GetMinDisplayValue();
    }

    return minDisplay;
}

inline void irtkQtImageObject::SetMaxDisplayValue(double max) {
    if (_lookupTable) {
        _lookupTable->SetMaxDisplayValue(max);
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
