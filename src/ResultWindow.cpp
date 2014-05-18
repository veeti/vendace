#include <QClipboard>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDir>

#include "Uploader.hpp"
#include "ResultWindow.hpp"
#include "SettingsDialog.hpp"

ResultWindow::ResultWindow(QPixmap screenshot) : mScreenshot(screenshot) {
    mUi.setupUi(this);
    mScene.addPixmap(mScreenshot);
    mUi.graphicsView->setScene(&mScene);
    mUi.uploadProgress->setVisible(false);

    // User interface signals
    connect(mUi.editButton, SIGNAL(pressed()), this, SLOT(editPressed()));
    connect(mUi.saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(mUi.uploadButton, SIGNAL(pressed()), this, SLOT(uploadPressed()));
    connect(mUi.settingsAction, SIGNAL(triggered(bool)), this, SLOT(openSettings()));

    // Upload signals
    connect(&mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadFinished(QNetworkReply*)));
}

void ResultWindow::editPressed() {
    if (mSettings.editorPath().isEmpty()) {
        QMessageBox::information(this, tr("Editor not set"), tr("You haven't selected the editor to use yet. You can set an editor from File -> Settings."));
    } else {
        openEditor();
    }
}

void ResultWindow::openEditor() {
    mEditFile = saveToTemp();
    if (mEditFile == NULL) {
        showTempFileError();
    } else {
        mEditProcess = new QProcess(this);
        connect(mEditProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(editProcessError(QProcess::ProcessError)));
        connect(mEditProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(editFinished()));

        QStringList args;
        args << mEditFile->fileName();
        mEditProcess->start(mSettings.editorPath(), args);

        mUi.editButton->setEnabled(false);
    }
}

void ResultWindow::editProcessError(QProcess::ProcessError error) {
    QMessageBox::critical(this, tr("Error"), tr("The editor could not be started."));
    cleanupEdit();
}

void ResultWindow::editFinished() {
    mScreenshot.load(mEditFile->fileName(), "PNG");
    mScene.addPixmap(mScreenshot);
    cleanupEdit();
}

void ResultWindow::cleanupEdit() {
    mUi.editButton->setEnabled(true);

    if (mEditFile != NULL) {
        mEditFile->deleteLater();
        mEditFile = NULL;
    }

    if (mEditProcess != NULL) {
        mEditProcess->deleteLater();
        mEditProcess = NULL;
    }
}

void ResultWindow::savePressed() {
    QString filename = QDateTime::currentDateTime().toString() + ".png";
    QString path = QFileDialog::getSaveFileName(this, tr("Save screenshot"), filename, "PNG image (*.png)");
    if (!path.isEmpty()) {
        if (mScreenshot.save(path)) {
            close();
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Failed to save screenshot."));
        }
    }
}

void ResultWindow::uploadPressed() {
    // Show progress
    mUi.uploadProgress->setVisible(true);
    mUi.uploadButton->setEnabled(false);

    // Save image to temporary file
    QTemporaryFile *temp = saveToTemp();
    if (temp == NULL) {
        showTempFileError();
    } else {
        // Send request
        QNetworkRequest req = mUploader.makeRequest();
        QHttpMultiPart *params = mUploader.makeMultiPart(temp);
        QNetworkReply *reply = mNetworkManager.post(req, params);
        params->setParent(reply);
    }
}

void ResultWindow::uploadFinished(QNetworkReply *reply) {
    // Hide progress
    mUi.uploadProgress->setVisible(false);
    mUi.uploadButton->setEnabled(true);

    if (reply->error() == QNetworkReply::NoError) {
        // Copy URL
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(mUploader.getUrlFromReply(reply));
        QMessageBox::information(this, tr("Upload complete"), tr("The image link has been copied to your clipboard."));
        close();
    } else {
        QMessageBox::critical(this, tr("Upload error"), tr("The upload failed. Try again later."));
    }

    reply->deleteLater();
}

QTemporaryFile* ResultWindow::saveToTemp() {
    QTemporaryFile *file = new QTemporaryFile(QDir::tempPath() + "/vendace-XXXXXXX.png", this);
    if (file->open() && mScreenshot.save(file->fileName(), "PNG")) {
        return file;
    } else {
        file->deleteLater();
        return NULL;
    }
}

void ResultWindow::openSettings() {
    SettingsDialog settings(this);
    settings.exec();
}

void ResultWindow::showTempFileError() {
    QMessageBox::critical(this, tr("Failed to save"), tr("Couldn't save the screenshot to a temporary file."));
}
