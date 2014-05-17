#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QString>
#include <QSettings>

#define KEY_EDITOR_PATH "Editor/Path"

class Settings {

    public:
        void setEditorPath(QString path);
        QString editorPath() const;

    protected:
        QSettings mSettings;

};

#endif
