#ifndef IRTKQTIMAGEOBJECT_H
#define IRTKQTIMAGEOBJECT_H

#include <QString>

class irtkQtImageObject
{

    /// flag showing if image is visible
    bool _visible;

    /// image opacity when displayed
    int _opacity;

    /// minimum and maximum display values
    int minDisplay, maxDisplay;

    /// image full path
    QString _path;

    /// image file name
    QString _fileName;

public:

    /// image object constructor
    irtkQtImageObject(const QString &path);

    /// image object destructor
    ~irtkQtImageObject();

    /// set image opacity
    void SetOpacity(int opacity);

    /// get image opacity
    int GetOpacity() const;

    /// set image minimum displayed value
    void SetMinDisplayValue(int value);

    /// get image minimum displayed value
    int GetMinDisplayValue() const;

    /// set image maximum displayed value
    void SetMaxDisplayValue(int value);

    /// get image maximum displayed value
    int GetMaxDisplayValue() const;

    /// set minimum and maximum values
    void SetMinMaxDisplayValues(int min, int max);

    /// check if image is visible
    bool IsVisible() const;

    /// set if image visible
    void SetVisible(bool visible);

    /// get image filename
    QString GetFileName() const;

    /// get image full path
    QString GetPath() const;
};


inline void irtkQtImageObject::SetOpacity(int opacity) {
    _opacity = opacity;
}

inline int irtkQtImageObject::GetOpacity() const {
    return _opacity;
}

inline void irtkQtImageObject::SetMinDisplayValue(int value) {
    minDisplay = value;
}

inline int irtkQtImageObject::GetMinDisplayValue() const {
    return minDisplay;
}

inline void irtkQtImageObject::SetMaxDisplayValue(int value) {
    maxDisplay = value;
}

inline int irtkQtImageObject::GetMaxDisplayValue() const {
    return maxDisplay;
}

inline void irtkQtImageObject::SetMinMaxDisplayValues(int min, int max) {
    minDisplay = min;
    maxDisplay = max;
}

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

#endif // IRTKQTIMAGEOBJECT_H
