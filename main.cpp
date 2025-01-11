#include <QApplication>
#include "loginwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    loginWindow login_window;
    login_window.show();
    printf("Running!!!");
    return QApplication::exec();
}

