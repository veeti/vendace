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
        showResourceText("LICENSE.md", tr("License"));
    } else if (link == "#contributors") {
        showResourceText("CONTRIBUTORS.md", tr("Contributors"));
    } else {
        QDesktopServices::openUrl(QUrl(link));
    }
}

void AboutDialog::showResourceText(QString filename, QString dialogTitle) {
    QFile file(":/text/" + filename);
    if (file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, dialogTitle, file.readAll());
    }
    file.close();
}

LogoLabel::LogoLabel(QWidget *parent = NULL) : QLabel(parent) {
}

void LogoLabel::mouseReleaseEvent(QMouseEvent *event) {
    QDesktopServices::openUrl(QUrl("http://vendace.org"));
}
