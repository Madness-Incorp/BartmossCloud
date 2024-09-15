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


using namespace std;
extern "C"{
    char **listFiles(const char* directoryPath);
    char **fileToArray(char *listOfFiles, size_t lofSize);
}

char* getServerFiles();
void deleteWidgets(QWidget* widget);

const char* fifoPath = "/Users/oisin/CLionProjects/pipingTest/my_fifo";
int flag = 0;
char** filling;

std::map<std::string, char*> filemap;

mainWindow::mainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    auto *grid_layout = new QGridLayout();
    ui->fileFrame->setLayout(grid_layout);


    if(ui->ClientButton && ui->ServerButton) {
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
        deleteWidgets(ui->fileFrame);  // Assuming this is safe

        cout << "HelloTesting!!" << endl;
        if(flag == 0) {
            char* filesList = getServerFiles();  // Do not free this!
            filling = fileToArray(filesList, strlen(filesList));
            flag++;
        }

        int i = 0;
        while (filling[i] != nullptr) {
            std::cout << filling[i] << std::endl;
            filePlacer(filling[i], 0, i);  // Display or process the file
            i++;
        }
    } else {
        ui->actorTitle->setText("Client Files");
        char** files = listFiles("/Users/oisin/Coding/ClientFolder");
        deleteWidgets(ui->fileFrame);

        int i = 0;
        while (files[i] != nullptr) {
            std::cout << files[i] << std::endl;
            filePlacer(files[i], 0, i);
            i++;
        }

        // Free the files array from listFiles if necessary
        free(files);
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
        std::string stringinFile = buttonClicked->text().toStdString();
        std::cout << stringinFile << std::endl;
        fileToSend = filemap.find(stringinFile)->second;
    }
    else
    {
        return;
    }


    std::cout << "The button that was clicked was: " << buttonClicked->objectName().toStdString() << std::endl;

    printf("%s: file %s\n", "FIFO WAIT", fileToSend);

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

char* getServerFiles() {
    static char* storedArray = nullptr;  // Initialize storedArray to nullptr
    extern int flag;  // Assuming flag is defined globally

    if (flag == 0) {
        int fd = open(fifoPath, O_RDONLY);
        if (fd == -1) {
            perror("Error opening FIFO");
            exit(EXIT_FAILURE);
        }

        ssize_t bufferSize = 1;
        size_t length = 0;
        char* filesArray = (char*)malloc(bufferSize * sizeof(char));
        if (filesArray == nullptr) {
            perror("Error allocating memory");
            close(fd);
            return nullptr;
        }

        ssize_t readIn;
        char byte;
        while ((readIn = read(fd, &byte, 1)) > 0) {
            cout << byte << endl;
            cout << "Start read" << endl;
            if (length >= bufferSize - 1) {
                bufferSize *= 2;
                char* newBuffer = (char*)realloc(filesArray, bufferSize * sizeof(char));
                if (newBuffer == nullptr) {
                    perror("Error reallocating memory");
                    close(fd);
                    free(filesArray);
                    return nullptr;
                }
                filesArray = newBuffer;  // Correct assignment
            }
            filesArray[length++] = byte;  // Store the byte
        }
        cout << "LOLOLO" << endl;

        if (readIn == -1) {
            perror("Error reading from FIFO");
            free(filesArray);
            close(fd);
            return nullptr;
        }

        // Add null terminator at the end of the string
        filesArray[length] = '\0';

        storedArray = filesArray;
        close(fd);
    }

    flag = 1;
    return storedArray;
}

void deleteWidgets(QWidget* widget) {
    if(widget == nullptr) return;

    QLayout* layout = widget->layout();
    QLayoutItem* item;

    while((item = layout->takeAt(0)) != nullptr) {
        QWidget* childItem = item->widget();
        delete childItem;
        delete item;
    }
}