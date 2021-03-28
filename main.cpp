#include "MainApp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainApp mainApp;

    QObject::connect(&mainApp, &MainApp::closeAppSignal, &a, &QApplication::quit);

    mainApp.setFixedSize(820, 500);
    mainApp.show();

    return a.exec();
}
