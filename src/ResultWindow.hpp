#ifndef RESULT_WINDOW_HPP
#define RESULT_WINDOW_HPP

#include <QMainWindow>
#include <QPixmap>
#include <QTemporaryFile>
#include <QProcess>

#include "ui_preview.h"

class ResultWindow : public QMainWindow {

    Q_OBJECT

    public:
        explicit ResultWindow(QPixmap screenshot);

    protected slots:
        void editPressed();
        void editFinished();
        void savePressed();

    private:
        QPixmap mScreenshot;
        QGraphicsScene mScene;
        Ui::PreviewWindow mUi;

        QTemporaryFile *mEditFile;
        QProcess *mEditProcess;

};

#endif
