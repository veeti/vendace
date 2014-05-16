#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "ResultWindow.hpp"

ResultWindow::ResultWindow(QPixmap screenshot) : mScreenshot(screenshot) {
    mUi.setupUi(this);
    mScene.addPixmap(mScreenshot);
    mUi.graphicsView->setScene(&mScene);

    connect(mUi.editButton, SIGNAL(pressed()), this, SLOT(editPressed()));
    connect(mUi.saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
}

void ResultWindow::editPressed() {
    mEditFile = new QTemporaryFile(QDir::tempPath() + "/vendace-XXXXXX.png", this);
    if (mEditFile->open()) {
        mScreenshot.save(mEditFile->fileName(), "PNG");

        mEditProcess = new QProcess(this);
        QStringList args;
        args << mEditFile->fileName();
        mEditProcess->start("/usr/bin/gimp", args); // FIXME configurable editor

        connect(mEditProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(editFinished()));
    }
}

void ResultWindow::editFinished() {
    mScreenshot.load(mEditFile->fileName(), "PNG");
    mScene.addPixmap(mScreenshot);

    delete mEditProcess;
    delete mEditFile;
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
