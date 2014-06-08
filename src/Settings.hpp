#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QClipboard>
#include <QString>
#include <QSettings>

#define KEY_EDITOR_PATH "Editor/Path"
#define KEY_COPY_TO "Upload/CopyTo"
#define SETTING_COPY_TO_CLIPBOARD "Clipboard"
#define SETTING_COPY_TO_SELECTION "Selection"

class Settings {

    public:
        void setEditorPath(QString path);
        QString editorPath() const;
        void setCopyTo(QString mode);
        QString copyTo() const;
        QClipboard::Mode copyToAsMode() const;

    protected:
        QSettings mSettings;

};

#endif
