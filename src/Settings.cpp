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
