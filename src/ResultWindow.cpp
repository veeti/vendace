#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "ResultWindow.hpp"
#include "SettingsDialog.hpp"

ResultWindow::ResultWindow(QPixmap screenshot) : mScreenshot(screenshot) {
    mUi.setupUi(this);
    mScene.addPixmap(mScreenshot);
    mUi.graphicsView->setScene(&mScene);

    connect(mUi.editButton, SIGNAL(pressed()), this, SLOT(editPressed()));
    connect(mUi.saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(mUi.settingsAction, SIGNAL(triggered(bool)), this, SLOT(openSettings()));
}

void ResultWindow::editPressed() {
    if (mSettings.editorPath().isEmpty()) {
        QMessageBox::information(this, tr("Editor not set"), tr("You haven't selected the editor to use yet. You can set an editor from File -> Settings."));
    } else {
        openEditor();
    }
}

void ResultWindow::openEditor() {
    mEditFile = new QTemporaryFile(QDir::tempPath() + "/vendace-XXXXXX.png", this);
    if (mEditFile->open()) {
        mScreenshot.save(mEditFile->fileName(), "PNG");

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

void ResultWindow::openSettings() {
    SettingsDialog settings(this);
    settings.exec();
}
