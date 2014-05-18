#ifndef RESULT_WINDOW_HPP
#define RESULT_WINDOW_HPP

#include <QMainWindow>
#include <QPixmap>
#include <QTemporaryFile>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "Settings.hpp"
#include "Uploader.hpp"
#include "ui_preview.h"

class ResultWindow : public QMainWindow {

    Q_OBJECT

    public:
        explicit ResultWindow(QPixmap screenshot);

    protected:
        void openEditor();
        void cleanupEdit();
        QTemporaryFile *saveToTemp();
        void showTempFileError();

    protected slots:
        void editPressed();
        void editProcessError(QProcess::ProcessError);
        void editFinished();
        void savePressed();
        void uploadPressed();
        void uploadFinished(QNetworkReply*);
        void openSettings();

    private:
        QPixmap mScreenshot;
        QGraphicsScene mScene;
        Ui::PreviewWindow mUi;

        QTemporaryFile *mEditFile;
        QProcess *mEditProcess;
        QNetworkAccessManager mNetworkManager;
        Uploader mUploader;
        Settings mSettings;

};

#endif
