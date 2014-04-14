#ifndef IRTKQTIMAGEOBJECT_H
#define IRTKQTIMAGEOBJECT_H

#include <QString>

class irtkQtImageObject
{

    /// flag showing if image is visible
    bool _visible;

    /// image opacity when displayed
    int _opacity;

    /// image full path
    QString _path;

    /// image file name
    QString _fileName;

public:

    irtkQtImageObject(const QString &path);

    ~irtkQtImageObject();

    void SetOpacity(int opacity);

    int GetOpacity() const;

    bool IsVisible() const;

    void SetVisible(bool visible);

    QString GetFileName() const;

    QString GetPath() const;
};


inline void irtkQtImageObject::SetOpacity(int opacity) {
    _opacity = opacity;
}

inline int irtkQtImageObject::GetOpacity() const {
    return _opacity;
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
