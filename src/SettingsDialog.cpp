#include <QFileDialog>

#include "SettingsDialog.hpp"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    mUi.setupUi(this);
    mUi.editorPath->setText(mSettings.editorPath());

    connect(mUi.editorPath, SIGNAL(textChanged(QString)), this, SLOT(editorPathChanged(QString)));
    connect(mUi.editorPathBrowse, SIGNAL(pressed()), this, SLOT(browseEditorPath()));
    connect(mUi.closeButton, SIGNAL(pressed()), this, SLOT(close()));
}

void SettingsDialog::editorPathChanged(QString path) {
    mSettings.setEditorPath(path);
}

void SettingsDialog::browseEditorPath() {
    QString path = QFileDialog::getOpenFileName(this, tr("Choose editor"), "/usr/bin/");
    if (!path.isEmpty()) {
        mUi.editorPath->setText(path);
    }
}
