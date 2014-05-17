#ifndef RESULT_WINDOW_HPP
#define RESULT_WINDOW_HPP

#include <QMainWindow>
#include <QPixmap>
#include <QTemporaryFile>
#include <QProcess>

#include "Settings.hpp"
#include "ui_preview.h"

class ResultWindow : public QMainWindow {

    Q_OBJECT

    public:
        explicit ResultWindow(QPixmap screenshot);

    protected:
        void openEditor();
        void cleanupEdit();

    protected slots:
        void editPressed();
        void editProcessError(QProcess::ProcessError);
        void editFinished();
        void savePressed();
        void openSettings();

    private:
        QPixmap mScreenshot;
        QGraphicsScene mScene;
        Ui::PreviewWindow mUi;

        QTemporaryFile *mEditFile;
        QProcess *mEditProcess;
        Settings mSettings;

};

#endif
