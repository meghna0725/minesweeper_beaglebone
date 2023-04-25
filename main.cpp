#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widget MainWindow;
    MainWindow.show();

    return app.exec();
}
