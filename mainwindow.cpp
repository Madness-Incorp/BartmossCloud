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
#include <QPushButton>

#include "LinkedList.h"


using namespace std;
extern "C"{
    char **GUIlistFiles(const char* directoryPath);
    char **GUIfileToArray(char *listOfFiles, size_t lofSize);
}

LinkedList* getServerFiles();
void deleteWidgets(QWidget* widget);

const char* fifoPath = "/Users/oisin/CLionProjects/pipingTest/my_fifo";
int flag = 0;
char mode = '\0';
char** filling;
LinkedList* fileLinkedList = nullptr;

std::map<std::string, char*> filemap;

mainWindow::mainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    auto *grid_layout = new QGridLayout();
    ui->fileFrame->setLayout(grid_layout);

    fileLinkedList = getServerFiles();
    if (fileLinkedList == nullptr) {
        std::cerr << "Error: Could not retrieve server files." << std::endl;
        return;
    }
    flag++;


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
    const auto *buttonClicked = qobject_cast<QPushButton*>(sender());

    if (buttonClicked->objectName().toStdString() == "ServerButton") {
        mode = 'D';
        ui->actorTitle->setText("Server Files");
        deleteWidgets(ui->fileFrame);  // Assuming this is safe to clear previous widgets

        cout << "HelloTesting!!" << endl;

        // Now loop through the linked list using getHead()
        const fileNode* current = fileLinkedList->getHead();  // Start at the head of the linked list
        int i = 0;
        while (current != nullptr) {
            std::cout << "Placing file: " << current->filename << std::endl;

            if(current->filename != " ") {
                filePlacer(current->filename, 0, i);
            }

            current = current->next;
            i++;
        }
    }else {
        ui->actorTitle->setText("Client Files");
        char** files = GUIlistFiles("/Users/oisin/Coding/ClientFolder");
        deleteWidgets(ui->fileFrame);

        mode = 'U';

        int i = 0;
        while (files[i] != nullptr) {
            std::cout << files[i] << std::endl;
            filePlacer(files[i], 0, i);
            i++;
        }

        free(files);
    }
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
    auto *layout = qobject_cast<QGridLayout*>(ui->fileFrame->layout());
    layout->addWidget(file, row, column);
}

void mainWindow::sendToClient() const{

    char* fileToSend;
    const auto *buttonClicked = qobject_cast<QPushButton*>(sender());

    if(buttonClicked->objectName().toStdString() != "ServerButton" && buttonClicked->objectName().toStdString() != "ClientButton") {
        const std::string stringinFile = buttonClicked->text().toStdString();
        std::cout << stringinFile << std::endl;
        fileToSend = filemap.find(stringinFile)->second;
    }
    else
    {
        return;
    }

    printf("%s: file %s\n", "FIFO WAIT", fileToSend);

    const int fifoID = open(fifoPath, O_WRONLY);
    if(fifoID == -1) {
        perror("Error opening FIFO");
        return;
    }

    std::cout << "Sending Mode: " << mode << endl;
    if(write(fifoID, &mode, sizeof(char)) <= 0) {
        perror("Error writing to FIFO2");
        ::close(fifoID);
        return;
    }

    ::close(fifoID);

    const int fifoID2 = open(fifoPath, O_WRONLY);
    if(fifoID2 == -1) {
        perror("Error opening FIFO");
        return;
    }

    std::cout << "Mode sent" << std::endl;
    std::cout<< "Now start to send " << fileToSend << std::endl;

    if(write(fifoID2, fileToSend, strlen(fileToSend) + 1) <= 0) {
        perror("Error writing to FIFO3");
        ::close(fifoID2);
        return;
    }

    std::cout << "Written to FIFO" << std::endl;

    ::close(fifoID2);
}

LinkedList* getServerFiles() {
    char** storedArray = nullptr;  // Initialize storedArray to nullptr

    if (flag == 0) {
        const int fd = open(fifoPath, O_RDONLY);
        if (fd == -1) {
            perror("Error opening FIFO");
            exit(EXIT_FAILURE);
        }

        ssize_t bufferSize = 1;
        size_t length = 0;
        auto filesArray = new char[bufferSize];


        ssize_t readIn;
        char byte;
        while ((readIn = read(fd, &byte, 1)) > 0) {
            if (length >= bufferSize - 1) {
                bufferSize *= 2;
                const auto newBuffer = (char*)realloc(filesArray, bufferSize * sizeof(char));
                if (newBuffer == nullptr) {
                    perror("Error reallocating memory");
                    close(fd);
                    free(filesArray);
                    return nullptr;
                }
                filesArray = newBuffer;
            }
            filesArray[length++] = byte;  // Store the byte
        }

        if (readIn == -1) {
            perror("Error reading from FIFO");
            free(filesArray);
            close(fd);
            return nullptr;
        }

        // Add null terminator at the end of the string
        filesArray[length] = '\0';
        const size_t size = strlen(filesArray);
        printf("filesArray: %s\n", filesArray);
        storedArray = GUIfileToArray(filesArray, size);

        close(fd);
    }

    flag = 1;
    int size = 0;
    if(storedArray == nullptr) {
        return nullptr;
    }
    while (storedArray[size] != nullptr) {
        size++;
    }
    std::cerr << "Size: " << size << std::endl;


    if ( storedArray[0] == nullptr) {
        std::cerr << "No files found or failed to split." << std::endl;
        return nullptr;
    }

    // Create a linked list with the array of filenames
    auto * fileLinkedList = new LinkedList();
    fileLinkedList->createLinkedList(storedArray, size);

    // Free the memory allocated for the file list
    for (int i = 0; i < size; i++) {
        free(storedArray[i]);
    }
    free(storedArray);

    return fileLinkedList;  // Return the linked list
}

void deleteWidgets(QWidget* widget) {
    if(widget == nullptr) return;

    QLayout* layout = widget->layout();
    QLayoutItem* item;

    while((item = layout->takeAt(0)) != nullptr) {
        const QWidget* childItem = item->widget();
        delete childItem;
        delete item;
    }
}