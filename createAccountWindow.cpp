//
// Created by Oisin Lynch on 01/11/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_createAccountWindow.h" resolved

#include "createaccountwindow.h"
#include <iostream>
#include <QMessageBox>
#include "editAccountDatabase.h"
#include "ui_createAccountWindow.h"


createAccountWindow::createAccountWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::createAccountWindow) {
    ui->setupUi(this);
    connect(ui->createButton, &QPushButton::clicked, this, &createAccountWindow::handleAccountCreation);
}

createAccountWindow::~createAccountWindow() {
    delete ui;
}

void createAccountWindow::handleAccountCreation() {

    const std::string Username = ui->createUsernameBar->text().toStdString();
    const std::string Password = ui->createPasswordBar->text().toStdString();

    if(Username.empty() || Password.empty()) {
        QMessageBox::warning(this, "Empty username or Password", "Please enter a Username and Password");
    }

    if(Username.length() < 8 || Username.length() > 10 || Password.length() < 8 || Password.length() > 10) {
        QMessageBox::warning(this, "Length not suitable", "Username and Password must be between 8 and 10 characters");
    }

    std::cout << "Account Created" << std::endl;
    editAccountDatabase::addNewAccount(Username, Password);
    std::cout << "Done Creating" << std::endl;
    this->close();
}

