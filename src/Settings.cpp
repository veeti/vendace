#include "Settings.hpp"

void Settings::setEditorPath(QString path) {
    mSettings.setValue(KEY_EDITOR_PATH, path);
}

QString Settings::editorPath() const {
    return mSettings.value(KEY_EDITOR_PATH).toString();
}

void Settings::setCopyTo(QString mode) {
    mSettings.setValue(KEY_COPY_TO, mode);
}

QString Settings::copyTo() const {
    return mSettings.value(KEY_COPY_TO, SETTING_COPY_TO_CLIPBOARD).toString();
}

QClipboard::Mode Settings::copyToAsMode() const {
    if (copyTo() == SETTING_COPY_TO_SELECTION) {
        return QClipboard::Selection;
    }
    return QClipboard::Clipboard;
}

void Settings::setUploadUrl(QString url) {
    mSettings.setValue(KEY_UPLOAD_URL, url);
}

QString Settings::uploadUrl() const {
    return mSettings.value(KEY_UPLOAD_URL).toString();
}

void Settings::setUploadKey(QString key) {
    mSettings.setValue(KEY_UPLOAD_KEY, key);
}

QString Settings::uploadKey() const {
    return mSettings.value(KEY_UPLOAD_KEY).toString();
}

void Settings::setUploadHttps(QString mode) {
    mSettings.setValue(KEY_UPLOAD_HTTPS, mode);
}

QString Settings::uploadHttps() const {
    return mSettings.value(KEY_UPLOAD_HTTPS).toString();
}
