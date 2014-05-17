#include "Settings.hpp"

void Settings::setEditorPath(QString path) {
    mSettings.setValue(KEY_EDITOR_PATH, path);
}

QString Settings::editorPath() const {
    return mSettings.value(KEY_EDITOR_PATH).toString();
}
