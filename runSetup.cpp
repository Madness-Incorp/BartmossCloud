#include <QApplication>
#include "setuppage.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    setUpPage set_up_page;
    set_up_page.show();
    printf("Running!!!");

    return QApplication::exec();
}
