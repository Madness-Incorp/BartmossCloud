#include "setuppage.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "importantFileLocations.h"

extern "C"{
    int setLocation(FILE_LOCATION_STATUS* status, const char* location, const char* logMessage, const char* errorMessage, int type);
    FILE_LOCATION_STATUS* getFileDirectoryStatus();
    int writeLocations(const char* fileLocationType, const char* location);
    int checkIfLocationsSet();
    int writeToLog(const char* action);
}

setUpPage::setUpPage(QWidget *parent) : QWidget(parent) {
    setFixedSize(420,300);
    setUp1();
}

void setUpPage::setUp1() {
    label = new QLabel("Bartmoss Cloud Setup", this);
    label->setGeometry(145, 100, 200, 40);
    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(50, 150, 300, 30);
    lineEdit->hide();
    nextButton = new QPushButton("Next", this);
    nextButton->setGeometry(160, 150, 100, 30); // Positioning the button
    connect(nextButton, &QPushButton::clicked, this, &setUpPage::goToStep2);
}

void setUpPage::setUp2() {
    label->setText("Enter file Locations");
    label->setGeometry(50, 70, 200, 40);
    lineEdit->show();
    nextButton->setGeometry(50, 220, 100, 30); // Position next button
    connect(nextButton, &QPushButton::clicked, this, &setUpPage::goToStep3);
}

void setUpPage::setUp3() {
    label->setText("Enter Log Location");
    connect(nextButton, &QPushButton::clicked, this, &setUpPage::goToStep4);
}

void setUpPage::setUp4() {
    delete nextButton;
    label->setText("Enter FIFO Location");
    finishButton = new QPushButton("Finish", this);
    finishButton->setGeometry(50, 220, 100, 30);
    finishButton->show();// Position finish button
    connect(finishButton, &QPushButton::clicked, this, &setUpPage::finishSetup);
}

void setUpPage::finishSetup() {
    const std::string stdString = lineEdit->text().trimmed().toStdString();
    const char* location = stdString.c_str();
    printf("Location is %s:", location);
    lineEdit->clear();
    this->close();
}

void setUpPage::goToStep2() {
    disconnect(nextButton, nullptr, nullptr, nullptr);
    setUp2();
}

void setUpPage::goToStep3() {
    disconnect(nextButton, nullptr, nullptr, nullptr);
    const std::string stdString = lineEdit->text().trimmed().toStdString();
    const char* location = stdString.c_str();
    FILE_LOCATION_STATUS* status = getFileDirectoryStatus();
    setLocation(status, location, "File Directory Location Set", "Unable to Set Log Location", 1);
    lineEdit->clear();
    setUp3();
}

void setUpPage::goToStep4() {
    disconnect(nextButton, nullptr, nullptr, nullptr);
    printf("The log location is: %s\n", lineEdit->text().toStdString().c_str());
    lineEdit->clear();
    setUp4();
}


