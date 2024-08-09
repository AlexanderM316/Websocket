#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    Server server(23456); // Укажите желаемый порт
    return a.exec();
}
