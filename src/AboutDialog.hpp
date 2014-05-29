#ifndef ABOUT_DIALOG_HPP
#define ABOUT_DIALOG_HPP

#include <QtWidgets>

#include "ui_about.h"

class AboutDialog : public QDialog {

    Q_OBJECT

    public:
        AboutDialog(QWidget *parent);

    public slots:
        void showLicense(QString);

    private:
        Ui::AboutDialog mUi;

};

#endif
