//
// Created by Oisin Lynch on 08/01/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_setUpPage.h" resolved

#include "setuppage.h"

#include <qboxlayout.h>

#include "loginwindow.h"
#include "ui_setUpPage.h"


setUpPage::setUpPage(QWidget *parent) :
    QWidget(parent){
    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    setUp1();
}

void setUpPage::setUp1() {
    clearLayout();

    label = new QLabel("Bartmoss Cloud Setup", this);
    nextButton = new QPushButton("Next", this);
    connect(nextButton, &QPushButton::clicked,this, &setUpPage::goToStep2);

    currentLayout = new QVBoxLayout();
    currentLayout->setAlignment(Qt::AlignCenter);

    currentLayout->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding));
    currentLayout->addWidget(label);
    currentLayout->addWidget(folderLocationLineEdit);
    currentLayout->addWidget(nextButton);
    currentLayout->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding));

    mainLayout->addLayout(currentLayout);
}

void setUpPage::setUp2() {
    // Clear any previous layout
    clearLayout();

    // Step 2: Ask for folder location with text entry and Next button
    label = new QLabel("Enter Folder Location", this);
    folderLocationLineEdit = new QLineEdit(this);
    nextButton = new QPushButton("Next", this);
    connect(nextButton, &QPushButton::clicked, this, &setUpPage::goToStep3);

    currentLayout = new QVBoxLayout();
    currentLayout->setAlignment(Qt::AlignCenter);

    // Center the label, text field, and next button using spacers
    currentLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));  // Top Spacer
    currentLayout->addWidget(label);
    currentLayout->addWidget(folderLocationLineEdit);
    currentLayout->addWidget(nextButton);
    currentLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));  // Bottom Spacer

    mainLayout->addLayout(currentLayout);
}

void setUpPage::setUp3() {
    // Clear any previous layout
    clearLayout();

    // Step 3: Ask for log location with text entry and Finish button
    label = new QLabel("Enter Log Location", this);
    logLocationLineEdit = new QLineEdit(this);
    finishButton = new QPushButton("Next", this);
    connect(finishButton, &QPushButton::clicked, this, &setUpPage::goToStep4);

    currentLayout = new QVBoxLayout();
    currentLayout->setAlignment(Qt::AlignCenter);

    // Center the label, text field, and finish button using spacers
    currentLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));  // Top Spacer
    currentLayout->addWidget(label);
    currentLayout->addWidget(logLocationLineEdit);
    currentLayout->addWidget(nextButton);
    currentLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));  // Bottom Spacer

    mainLayout->addLayout(currentLayout);
}

void setUpPage::setUp4() {
    // Clear any previous layout
    clearLayout();

    // Step 3: Ask for log location with text entry and Finish button
    label = new QLabel("Enter FIFO Location", this);
    fifoLocationLineEdit = new QLineEdit(this);
    finishButton = new QPushButton("Finish", this);
    connect(finishButton, &QPushButton::clicked, this, &setUpPage::finishSetup);

    currentLayout = new QVBoxLayout();
    currentLayout->setAlignment(Qt::AlignCenter);

    // Center the label, text field, and finish button using spacers
    currentLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));  // Top Spacer
    currentLayout->addWidget(label);
    currentLayout->addWidget(fifoLocationLineEdit);
    currentLayout->addWidget(finishButton);
    currentLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));  // Bottom Spacer

    mainLayout->addLayout(currentLayout);
}

void setUpPage::finishSetup() {
    // Do something with the entered data, like save or validate locations
    const QString folderLocation = folderLocationLineEdit->text();
    const QString logLocation = logLocationLineEdit->text();
    const QString fifoLocation = fifoLocationLineEdit -> text();
    qDebug() << "Folder Location:" << folderLocation;
    qDebug() << "Log Location:" << logLocation;
    qDebug() << "FIFO Location:" << fifoLocation;

    // Close the setup widget after finishing
    close();
    loginWindow login_window;
    login_window.show();
    printf("Running!!!");
}

void setUpPage::clearLayout() {
    QLayoutItem *item;
    while((item = currentLayout -> takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}



