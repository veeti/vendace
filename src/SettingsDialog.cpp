#include <QFileDialog>

#include "SettingsDialog.hpp"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    mUi.setupUi(this);
    mUi.editorPath->setText(mSettings.editorPath());

    // Add copy options
    mUi.copyTo->insertItem(0, tr("Clipboard"));
    mUi.copyTo->insertItem(1, tr("Selection"));
    if (mSettings.copyTo() == SETTING_COPY_TO_SELECTION) {
        mUi.copyTo->setCurrentIndex(1);
    }

    connect(mUi.editorPath, SIGNAL(textChanged(QString)), this, SLOT(editorPathChanged(QString)));
    connect(mUi.editorPathBrowse, SIGNAL(pressed()), this, SLOT(browseEditorPath()));
    connect(mUi.copyTo, SIGNAL(currentIndexChanged(int)), this, SLOT(copyToChanged(int)));
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

void SettingsDialog::copyToChanged(int index) {
    if (index == 0) {
        mSettings.setCopyTo(SETTING_COPY_TO_CLIPBOARD);
    } else if (index == 1) {
        mSettings.setCopyTo(SETTING_COPY_TO_SELECTION);
    }
}
