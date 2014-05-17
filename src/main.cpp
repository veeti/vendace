#include <QApplication>
#include <QCoreApplication>
#include "ScreenshotWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("vendace");
    QCoreApplication::setApplicationName("vendace");

    ScreenshotWindow window;
    window.show();

    return app.exec();
}
