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

    /// image min and max values
    double imageMin, imageMax;

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
    int GetMinImageValue();

    /// get maximum value for the corresponding image
    int GetMaxImageValue();

    /// get minimum display value for the corresponding image
    int GetMinDisplayValue();

    /// get maximum display value for the corresponding image
    int GetMaxDisplayValue();

    /// set colormap for the corresponding image
    void SetColormap(irtkQtLookupTable::irtkColorMode mode);
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
    _lookupTable->SetAlpha(value);
}

inline int irtkQtImageObject::GetOpacity() {
    return _lookupTable->GetAlpha();
}

inline int irtkQtImageObject::GetMinImageValue() {
    return _lookupTable->GetImageMinValue();
}

inline int irtkQtImageObject::GetMaxImageValue() {
    return _lookupTable->GetImageMaxValue();
}

inline int irtkQtImageObject::GetMinDisplayValue() {
    return _lookupTable->GetMinDisplayValue();
}

inline int irtkQtImageObject::GetMaxDisplayValue() {
    return _lookupTable->GetMaxDisplayValue();
}

inline void irtkQtImageObject::SetColormap(irtkQtLookupTable::irtkColorMode mode) {
    _lookupTable->SetColorMode(mode);
}

#endif // IRTKQTIMAGEOBJECT_H
