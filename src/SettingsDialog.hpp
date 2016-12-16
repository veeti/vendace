#ifndef SETTINGS_DIALOG_HPP
#define SETTINGS_DIALOG_HPP

#include <QDialog>

#include "Settings.hpp"
#include "ui_settings.h"

class SettingsDialog : public QDialog {

    Q_OBJECT

    public:
        SettingsDialog(QWidget *parent);

    protected slots:
        void editorPathChanged(QString);
        void uploadUrlChanged(QString);
        void uploadKeyChanged(QString);
        void uploadHttpsChanged(bool);
        void browseEditorPath();
        void copyToChanged(int);

    private:
        Ui::SettingsDialog mUi;
        Settings mSettings;

};

#endif
