//
// Created by Oisin Lynch on 05/09/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainWindow.h" resolved

#include "mainwindow.h"
#include <iostream>
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

extern "C"{
    char **listFiles(const char* directoryPath);
}

std::map<std::string, char*> filemap;

mainWindow::mainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    auto *grid_layout = new QGridLayout();
    ui->fileFrame->setLayout(grid_layout);


    if(ui->ClientButton && ui->ServerButton) {
        std::cout << "Hello World";
        connect(ui->ClientButton, &QPushButton::clicked, this, &mainWindow::handleActorSelector);
        connect(ui->ServerButton, &QPushButton::clicked, this, &mainWindow::handleActorSelector);
    }else {
        qDebug() << "Buttons are not initialized";
    }

}

mainWindow::~mainWindow() {
    delete ui;
}

void mainWindow::handleActorSelector() {
    auto *buttonClicked = qobject_cast<QPushButton*>(sender());

    if(buttonClicked->objectName().toStdString() == "ServerButton") {
        ui->actorTitle->setText("Server Files");
    }else {
        ui->actorTitle->setText("Client Files");
        char** files = listFiles("/Users/oisin/Coding/ClientFolder");
        int i = 0;
        while (files[i] != nullptr) {
            std::cout << files[i] << std::endl;
            filePlacer(files[i], 0, i);
            i++;
        }
    }

    std::cout << "The button that was clicked was: " << buttonClicked->objectName().toStdString() << std::endl;
}

void mainWindow::filePlacer(char* fileName, const int column, const int row) {

    QPushButton *file;
    char shortened[11];
    if(strlen(fileName) > 10) {
       for(int i = 0; i < 8; i++) {
           shortened[i] = fileName[i];
       }
        shortened[8] = '.';
        shortened[9] = '.';
        shortened[10] = '\0';
        file = new QPushButton(shortened);
    }else {
        file = new QPushButton(fileName);
    }

    filemap[shortened] = fileName;

    file->setMaximumSize(100,30);
    connect(file, &QPushButton::clicked, this, &mainWindow::sendToClient);
    auto *layout = (QGridLayout*)ui->fileFrame->layout();
    layout->addWidget(file, row, column);
}

void mainWindow::sendToClient() {

    char* fileToSend;
    auto *buttonClicked = qobject_cast<QPushButton*>(sender());

    if(buttonClicked->objectName().toStdString() != "ServerButton" && buttonClicked->objectName().toStdString() != "ClientButton") {
        std::string stringinFile = buttonClicked->objectName().toStdString();
        fileToSend = filemap.find(stringinFile)->second;
    }
    else
    {
        return;
    }


    std::cout << "The button that was clicked was: " << buttonClicked->objectName().toStdString() << std::endl;

    const char* fifoPath = "/Users/oisin/CLionProjects/pipingTest/my_fifo";

    printf("%s", "FIFO WAIT");

    int fd = open(fifoPath, O_WRONLY);

    if(fd == -1) {
        perror("Error opening FIFO");
        return;
    }

    ssize_t bytesWritten = write(fd, fileToSend, strlen(fileToSend) + 1);
    if(bytesWritten <= 0) {
        perror("Error writting to FIFO");
        ::close(fd);
        return;
    }

    std::cout << "Written to FIFO" << std::endl;

    ::close(fd);
}

