#include <QApplication>
#include "ScreenshotWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ScreenshotWindow window;
    window.show();
    return app.exec();
}
