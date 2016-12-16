#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QClipboard>
#include <QString>
#include <QSettings>

#define KEY_EDITOR_PATH "Editor/Path"
#define KEY_COPY_TO "Upload/CopyTo"
#define KEY_UPLOAD_URL "Upload/Url"
#define KEY_UPLOAD_KEY "Upload/Key"
#define KEY_UPLOAD_HTTPS "Upload/Https"
#define SETTING_COPY_TO_CLIPBOARD "Clipboard"
#define SETTING_COPY_TO_SELECTION "Selection"
#define SETTING_UPLOAD_URL "https://api.imgur.com/3/image"
#define SETTING_UPLOAD_KEY "Client-ID ede801c7c97f184"
#define SETTING_UPLOAD_HTTPS "true"

class Settings {

    public:
        void setEditorPath(QString path);
        QString editorPath() const;
        void setCopyTo(QString mode);
        QString copyTo() const;
        QClipboard::Mode copyToAsMode() const;
        void setUploadUrl(QString url);
        QString uploadUrl() const;
        void setUploadKey(QString key);
        QString uploadKey() const;
        void setUploadHttps(QString key);
        QString uploadHttps() const;

    protected:
        QSettings mSettings;

};

#endif
