#ifndef ABOUT_DIALOG_HPP
#define ABOUT_DIALOG_HPP

#include <QtWidgets>

class LogoLabel : public QLabel {

    Q_OBJECT

    public:
        LogoLabel(QWidget *parent);

    protected:
        void mouseReleaseEvent(QMouseEvent*);

};

#include "ui_about.h"

class AboutDialog : public QDialog {

    Q_OBJECT

    public:
        AboutDialog(QWidget *parent);

    public slots:
        void showLicense(QString);

    protected:
        void showResourceText(QString, QString);

    private:
        Ui::AboutDialog mUi;

};

#endif
