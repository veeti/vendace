#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>

#include "ResultWindow.hpp"

ResultWindow::ResultWindow(QPixmap screenshot) : mScreenshot(screenshot) {
    mUi.setupUi(this);
    mScene.addPixmap(mScreenshot);
    mUi.graphicsView->setScene(&mScene);

    connect(mUi.saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
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
