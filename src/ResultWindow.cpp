#include <QDateTime>
#include <QDesktopWidget>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDir>

#include "Uploader.hpp"
#include "ResultWindow.hpp"
#include "AboutDialog.hpp"
#include "SettingsDialog.hpp"

ResultWindow::ResultWindow(QPixmap screenshot) : mScreenshot(screenshot) {
    mUi.setupUi(this);
    mScene.addPixmap(mScreenshot)->setTransformationMode(Qt::SmoothTransformation);
    mUi.graphicsView->setScene(&mScene);
    mUi.graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    mUi.uploadProgress->setVisible(false);

    // Center on screen
    QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.center() - rect().center());

    // User interface signals
    connect(mUi.editButton, SIGNAL(pressed()), this, SLOT(editPressed()));
    connect(mUi.saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(mUi.uploadButton, SIGNAL(pressed()), this, SLOT(uploadPressed()));
    connect(mUi.settingsAction, SIGNAL(triggered(bool)), this, SLOT(openSettings()));
    connect(mUi.exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(mUi.aboutAction, SIGNAL(triggered(bool)), this, SLOT(openAbout()));
    connect(mUi.aboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

    // Upload signals
    connect(&mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadFinished(QNetworkReply*)));
}

// Methods

/**
 * Cleans up after the edit process.
 */
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

/**
 * Called on window resize. Scales the screenshot to either fit in the preview widget or to its
 * maximum size.
 */
void ResultWindow::resizeEvent(QResizeEvent *event) {
    QRect view = mUi.graphicsView->rect();
    if (mScreenshot.width() >= view.width() || mScreenshot.height() >= view.height()) {
        mUi.graphicsView->fitInView(mScreenshot.rect(), Qt::KeepAspectRatio);
    } else {
        mUi.graphicsView->fitInView(view);
    }
}

/**
 * Saves the current screenshot into a temporary file. May return null on failure.
 */
QTemporaryFile* ResultWindow::saveToTemp() {
    QTemporaryFile *file = new QTemporaryFile(QDir::tempPath() + "/vendace-XXXXXXX.png", this);
    if (file->open() && mScreenshot.save(file->fileName(), "PNG")) {
        return file;
    } else {
        QMessageBox::critical(this, tr("Failed to save"), tr("Couldn't save the screenshot to a temporary file."));
        file->deleteLater();
        return NULL;
    }
}

// Slots

/*
 * Called when the editor button is pressed.
 */
void ResultWindow::editPressed() {
    if (mSettings.editorPath().isEmpty()) {
        QMessageBox::information(this, tr("Editor not set"), tr("You haven't selected the editor to use yet. You can set an editor from File -> Settings."));
    } else {
        openEditor();
    }
}

/**
 * Opens the editor.
 */
void ResultWindow::openEditor() {
    mEditFile = saveToTemp();
    if (mEditFile != NULL) {
        mEditProcess = new QProcess(this);
        connect(mEditProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(editProcessError(QProcess::ProcessError)));
        connect(mEditProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(editFinished()));

        QStringList args;
        args << mEditFile->fileName();
        mEditProcess->start(mSettings.editorPath(), args);

        mUi.editButton->setEnabled(false);
    }
}

/**
 * Handles editor process errors.
 */
void ResultWindow::editProcessError(QProcess::ProcessError error) {
    QMessageBox::critical(this, tr("Error"), tr("The editor could not be started."));
    cleanupEdit();
}

/**
 * Handles the end of the edit process and loads the edited image.
 */
void ResultWindow::editFinished() {
    if (mScreenshot.load(mEditFile->fileName(), "PNG")) {
        mScene.addPixmap(mScreenshot);
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Failed to load edited file."));
    }

    cleanupEdit();
}

/**
 * Called when the upload button is pressed.
 */
void ResultWindow::uploadPressed() {
    // Show progress
    mUi.uploadProgress->setVisible(true);
    mUi.uploadButton->setEnabled(false);

    // Save image to temporary file
    QTemporaryFile *temp = saveToTemp();
    if (temp != NULL) {
        // Send request
        QNetworkRequest req = mUploader.makeRequest();
        QHttpMultiPart *params = mUploader.makeMultiPart(temp);
        QNetworkReply *reply = mNetworkManager.post(req, params);
        params->setParent(reply);
    }
}

/**
 * Called when the upload is finished. Copies the resulting URL to the clipboard or shows an
 * error.
 */
void ResultWindow::uploadFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        // Copy URL
        QClipboard* clipboard = QApplication::clipboard();
        QClipboard::Mode setting = mSettings.copyTo() == SETTING_COPY_TO_CLIPBOARD ? QClipboard::Clipboard : QClipboard::Selection;
        clipboard->setText(mUploader.getUrlFromReply(reply), setting);
        connect(clipboard, SIGNAL(changed(QClipboard::Mode)), this, SLOT(clipboardChanged(QClipboard::Mode)));
        QMessageBox::information(this, tr("Upload complete"), tr("The image link has been copied to your clipboard."));

        // Stick around until clipboard is overwritten
        hide();
    } else {
        QMessageBox::critical(this, tr("Upload error"), tr("The upload failed. Try again later."));
        mUi.uploadProgress->setVisible(false);
        mUi.uploadButton->setEnabled(true);
    }

    reply->deleteLater();
}

/**
 * Called when the system's clipboard is changed. This will close the application as there is no
 * longer any need to stay alive to persist the copied URL.
 */
void ResultWindow::clipboardChanged(QClipboard::Mode mode) {
    QClipboard::Mode setting = mSettings.copyTo() == SETTING_COPY_TO_CLIPBOARD ? QClipboard::Clipboard : QClipboard::Selection;
    if (mode == setting) {
        close();
    }
}

/**
 * Called when the save button is pressed.
 */
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

/**
 * Opens a settings dialog.
 */
void ResultWindow::openSettings() {
    SettingsDialog settings(this);
    settings.exec();
}

/**
 * Opens the about dialog.
 */
void ResultWindow::openAbout() {
    AboutDialog about(this);
    about.exec();
}
