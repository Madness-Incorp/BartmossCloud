#include <QApplication>

#include "fileLocationFunctions.h"
#include "loginwindow.h"
#include "setuppage.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    if(checkIfLocationsSet() == 0) {
        setUpPage set_up_page;
        set_up_page.show();
    }else {
        loginWindow login_window;
        login_window.show();
    }
    printf("Running!!!");

    return QApplication::exec();
}

