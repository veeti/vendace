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

/**
 * A window that displays a preview of the captured screenshot and options for the image.
 */
class ResultWindow : public QMainWindow {

    Q_OBJECT

    public:
        explicit ResultWindow(QPixmap screenshot);

    protected:
        /**
         * Deletes any variables used in the edit process, if any, and unlocks the edit button.
         */
        void cleanupEdit();

        /**
         * Saves the screenshot into a temporary file for manipulation or uploading.
         * @return Pointer to temporary file or null on failure
         */
        QTemporaryFile *saveToTemp();

    protected slots:
        // Edit
        void editPressed();
        void openEditor();
        void editProcessError(QProcess::ProcessError);
        void editFinished();

        // Upload
        void uploadPressed();
        void uploadFinished(QNetworkReply*);

        // Save
        void savePressed();

        // Menu
        void openSettings();

        // Window
        void resizeEvent(QResizeEvent*);

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
