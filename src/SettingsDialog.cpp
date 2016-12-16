#include <QFileDialog>

#include "SettingsDialog.hpp"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    mUi.setupUi(this);
    mUi.editorPath->setText(mSettings.editorPath());

    if(mSettings.uploadUrl().length() == 0) {
        mSettings.setUploadUrl(SETTING_UPLOAD_URL);
    }
    if(mSettings.uploadKey().length() == 0) {
        mSettings.setUploadKey(SETTING_UPLOAD_KEY);
    }
    mUi.uploadUrl->setText(mSettings.uploadUrl());
    mUi.uploadKey->setText(mSettings.uploadKey());

    if(mSettings.uploadHttps() == SETTING_UPLOAD_HTTPS) {
        mUi.uploadHttps->setChecked(true);
    } else {
        mUi.uploadHttps->setChecked(false);
    }

    // Add copy options
    mUi.copyTo->insertItem(0, tr("Clipboard"));
    mUi.copyTo->insertItem(1, tr("Selection"));
    if (mSettings.copyTo() == SETTING_COPY_TO_SELECTION) {
        mUi.copyTo->setCurrentIndex(1);
    }

    connect(mUi.editorPath, SIGNAL(textChanged(QString)), this, SLOT(editorPathChanged(QString)));
    connect(mUi.uploadUrl, SIGNAL(textChanged(QString)), this, SLOT(uploadUrlChanged(QString)));
    connect(mUi.uploadKey, SIGNAL(textChanged(QString)), this, SLOT(uploadKeyChanged(QString)));
    connect(mUi.uploadHttps, SIGNAL(clicked(bool)), this, SLOT(uploadHttpsChanged(bool)));
    connect(mUi.editorPathBrowse, SIGNAL(pressed()), this, SLOT(browseEditorPath()));
    connect(mUi.copyTo, SIGNAL(currentIndexChanged(int)), this, SLOT(copyToChanged(int)));
    connect(mUi.closeButton, SIGNAL(pressed()), this, SLOT(close()));
}

void SettingsDialog::editorPathChanged(QString path) {
    mSettings.setEditorPath(path);
}

void SettingsDialog::uploadUrlChanged(QString url) {
    if(mSettings.uploadUrl().length() == 0) {
        mSettings.setUploadUrl(SETTING_UPLOAD_URL);
    } else {
        mSettings.setUploadUrl(url);
    }
}

void SettingsDialog::uploadKeyChanged(QString key) {
    if(mSettings.uploadKey().length() == 0) {
        mSettings.setUploadKey(SETTING_UPLOAD_KEY);
    } else {
        mSettings.setUploadKey(key);
    }
}

void SettingsDialog::uploadHttpsChanged(bool mode) {
    if(mode) {
        mSettings.setUploadHttps(SETTING_UPLOAD_HTTPS);
    } else {
        mSettings.setUploadHttps("false");
    }
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
