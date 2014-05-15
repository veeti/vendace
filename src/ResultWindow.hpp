#ifndef RESULT_WINDOW_HPP
#define RESULT_WINDOW_HPP

#include <QMainWindow>
#include <QPixmap>

#include "ui_preview.h"

class ResultWindow : public QMainWindow {

    Q_OBJECT

    public:
        explicit ResultWindow(QPixmap screenshot);

    protected slots:
        void savePressed();

    private:
        QPixmap mScreenshot;
        QGraphicsScene mScene;
        Ui::PreviewWindow mUi;

};

#endif
