#ifndef IRTKQTIMAGEOBJECT_H
#define IRTKQTIMAGEOBJECT_H

#include <QString>

class irtkQtImageObject
{

    /// flag showing if image is visible
    bool _visible;

    /// image full path
    QString _path;

    /// image file name
    QString _fileName;

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

#endif // IRTKQTIMAGEOBJECT_H
