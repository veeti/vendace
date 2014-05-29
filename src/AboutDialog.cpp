#include "AboutDialog.hpp"
#include "vendace.hpp"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    mUi.setupUi(this);

    // Format version
    QString version = QString("%1.%2.%3").arg(VENDACE_MAJOR).arg(VENDACE_MINOR).arg(VENDACE_PATCH);
    mUi.versionLabel->setText(mUi.versionLabel->text().arg(version));

    connect(mUi.closeButton, SIGNAL(pressed()), this, SLOT(close()));
    connect(mUi.licenseLabel, SIGNAL(linkActivated(QString)), this, SLOT(showLicense(QString)));
}

void AboutDialog::showLicense(QString link) {
    if (link == "#license") {
        QFile license(":/text/LICENSE.md");
        license.open(QIODevice::ReadOnly);
        QMessageBox::information(this, tr("License"), license.readAll());
        license.close();
    } else {
        QDesktopServices::openUrl(QUrl(link));
    }
}
