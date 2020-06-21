#include "browser.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("Crusta");
    QApplication::setOrganizationDomain("crustabrowser.com");
    QApplication::setApplicationName("Crusta");
    QApplication::setApplicationVersion("2.0.0");
    QApplication::setApplicationDisplayName("Crusta");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication::setStyle(QStyleFactory::create("fusion"));

    QApplication app(argc, argv);
    app.setStyleSheet(Browser::readFile(":/styles/default.css"));

    crusta->run();

    int status = app.exec();
    delete crusta;

    return status;
}
