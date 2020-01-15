#include <QApplication>

#include "MainWindow.h"

int main(int argc, char **argv)
{
    if (argc >= 2) {
        MainWindow::debugMode = true;
    }

    QApplication app{argc, argv};

    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
