//
// Created by Oisin Lynch on 01/11/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Create_Account_Page.h" resolved

#include "create_account_page.h"
#include "ui_Create_Account_Page.h"


Create_Account_Page::Create_Account_Page(QWidget *parent) :
    QWidget(parent), ui(new Ui::Create_Account_Page) {
    ui->setupUi(this);
}

Create_Account_Page::~Create_Account_Page() {
    delete ui;
}
