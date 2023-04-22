#include "snake.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widget snake;
    snake.show();

    return app.exec();
}
