//
// Created by Oisin Lynch on 04/09/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_loginWindow.h" resolved

#include "loginwindow.h"
#include <QWidget>
#include "ui_loginWindow.h"
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainWindow.h"



loginWindow::loginWindow(QWidget *parent):
    QWidget(parent),
    ui(new Ui::loginWindow)
{
    ui->setupUi(this);

    connect(ui->loginButton, &QPushButton::clicked, this, &loginWindow::handleLogin);
}

loginWindow::~loginWindow() {
    delete ui;
}

void loginWindow::handleLogin() {
    QString username = ui->UsernameBar->text();
    QString password = ui->PasswordBar->text();

    if(username == "Hello" && password == "12345") {
        QMessageBox::information(this, "Login  Success", "Welcome!");

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

