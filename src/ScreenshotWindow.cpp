#include <QtWidgets>
#include <QtGui>
#include <QRect>
#include <QSize>

#include "ScreenshotWindow.hpp"
#include "ResultWindow.hpp"

ScreenshotWindow::ScreenshotWindow() : mCropper(NULL) {
    // Get the entire screen size
    QSize screen = QApplication::desktop()->size();
    int w = screen.width();
    int h = screen.height();

    // Snap
    mScreenshot = QGuiApplication::primaryScreen()->grabWindow(0, 0, 0, w, h);

    // Full screen and no title bar
    setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowState(Qt::WindowFullScreen);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    createUi();
    resize(screen);
    move(0, 0);
}

/**
 * Creates the central widget for the window.
 */
void ScreenshotWindow::createUi() {
    // Show the screenshot
    mScreenshotLabel = new QLabel;
    mScreenshotLabel->setPixmap(mScreenshot);

    // Track mouse and pass to main window
    mScreenshotLabel->setMouseTracking(true);
    mScreenshotLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Help text
    mHelpLabel = new QLabel(mScreenshotLabel);
    mHelpLabel->setText(tr("Click, hold and drag to crop. Release to finish.\nPress Escape to cancel and quit."));
    mHelpLabel->setStyleSheet("QLabel { background: #000; color: #fff; font-size: 18px; padding: 10px; }");
    mHelpLabel->move(10, 10);

    setCentralWidget(mScreenshotLabel);
    activateWindow();
}

/**
 * Crops the captured screenshot based on the current crop selection and shows the result window.
 */
void ScreenshotWindow::selectCrop() {
    ResultWindow *result = new ResultWindow(mScreenshot.copy(mCropper->geometry()));
    result->show();
    close();
}

/**
 * Called on key release.
 */
void ScreenshotWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}

/**
 * Called on mouse click.
 */
void ScreenshotWindow::mousePressEvent(QMouseEvent *event) {
    QPoint pos = event->pos();
    // Hide current cropper
    if (mCropper != NULL) {
        mCropper->hide();
        delete mCropper;
    }

    // Create new cropper
    mCropOrigin = pos;
    mCropper = new QRubberBand(QRubberBand::Rectangle, mScreenshotLabel);
    mCropper->setGeometry(QRect(mCropOrigin, QSize()));
    mCropper->show();
}

/**
 * Called on mouse move.
 */
void ScreenshotWindow::mouseMoveEvent(QMouseEvent *event) {
    // Resize cropper if dragging
    if (mCropper != NULL && event->buttons() & Qt::LeftButton == 1) {
        mCropper->setGeometry(QRect(mCropOrigin, event->pos()).normalized());
    }

    // Keep the help label out of the way
    bool mouseInHelp = mHelpLabel->geometry().contains(event->pos());
    bool cropperInHelp = (mCropper != NULL && mHelpLabel->geometry().intersects(mCropper->geometry()));
    mHelpLabel->setVisible(!(mouseInHelp || cropperInHelp));
}

void ScreenshotWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (mCropper != NULL && event->button() == Qt::LeftButton) {
        selectCrop();
    }
}
