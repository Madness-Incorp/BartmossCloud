//
// Created by Oisin Lynch on 04/09/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_loginWindow.h" resolved

#include "loginwindow.h"

#include <iostream>
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
    if(editAccountDatabase::testConnection() != 0) {
        QMessageBox::warning(this, "Connection to Database Failed", "Cannot connect to Server DataBase");
        exit(EXIT_FAILURE);
    }
}

loginWindow::~loginWindow() {
    delete ui;
}

void loginWindow::handleLogin() {

    if(editAccountDatabase::checkAccountDetailsorCreateAccount(ui->UsernameBar->text().toStdString(), ui->PasswordBar->text().toStdString(), 1) == 1) {
        showMainWindow();
        this->close();
    }else {
        QMessageBox::warning(this, "Login Failed", "Invalid Username or Password");
        exit(EXIT_FAILURE);
    }
}

void loginWindow::showMainWindow() {
    if(!mainwindow) {
        mainwindow = new mainWindow();
        mainwindow->show();
    }
}

void loginWindow::showCreateAccountWindow() {
    if(!createAccountWindow) {
        createAccountWindow = new class createAccountWindow();
        createAccountWindow->show();
        bool connected = connect(createAccountWindow, &QObject::destroyed, this, &loginWindow::onCreateAccountWindowClosed);
        if (!connected) {
            std::cerr << "Failed to connect the destroyed signal!" << std::endl;
        }
    }
}

void loginWindow::onCreateAccountWindowClosed() {
    std::cout << "Create Account Window Closed" << std::endl;
    // This function is called when createAccountWindow is closed
    createAccountWindow = nullptr;// Reset the pointer
    this->close();
    showMainWindow();
}


