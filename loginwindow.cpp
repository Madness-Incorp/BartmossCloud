//
// Created by Oisin Lynch on 04/09/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_loginWindow.h" resolved

#include "loginwindow.h"
#include <QWidget>
#include "ui_loginWindow.h"
#include <QMessageBox>

#include "createaccountwindow.h"
#include "editAccountDatabase.h"
#include "mainwindow.h"
#include "ui_mainWindow.h"



loginWindow::loginWindow(QWidget *parent):
    QWidget(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &loginWindow::handleLogin);
    connect(ui->createAccount, &QPushButton::clicked, this, &loginWindow::showCreateAccountWindow);
}

loginWindow::~loginWindow() {
    delete ui;
}

void loginWindow::handleLogin() {

    if(ui->UsernameBar->text() == "Hello" && ui->PasswordBar->text() == "12345") {
        showMainWindow();
        this->close();
    }else {
        QMessageBox::warning(this, "Login Failed", "Invalid Username or Password");
    }
}

void loginWindow::showMainWindow() {
    if(!mainwindow) {
        mainwindow = new mainWindow();
        mainwindow->show();
    }
}

void loginWindow::showCreateAccountWindow() {
    if(!createAccountWindow && editAccountDatabase::testConnection(true) == 1) {
        createAccountWindow = new class createAccountWindow();
        createAccountWindow->show();
    }else {
        QMessageBox::warning(this, "Connection to Database Failed", "Cannot connect to Server DataBase");
        exit(EXIT_FAILURE);
    }
}


