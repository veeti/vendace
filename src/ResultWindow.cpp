#include <QDateTime>
#include <QFileDialog>

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
    mScreenshot.save(path);
    close();
}
