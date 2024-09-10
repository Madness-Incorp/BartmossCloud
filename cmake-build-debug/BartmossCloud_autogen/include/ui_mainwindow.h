/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QPushButton *ServerButton;
    QPushButton *ClientButton;
    QLabel *actorTitle;
    QWidget *fileFrame;

    void setupUi(QWidget *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName("mainWindow");
        mainWindow->resize(479, 363);
        ServerButton = new QPushButton(mainWindow);
        ServerButton->setObjectName("ServerButton");
        ServerButton->setGeometry(QRect(20, 90, 81, 32));
        ClientButton = new QPushButton(mainWindow);
        ClientButton->setObjectName("ClientButton");
        ClientButton->setGeometry(QRect(20, 150, 81, 32));
        actorTitle = new QLabel(mainWindow);
        actorTitle->setObjectName("actorTitle");
        actorTitle->setGeometry(QRect(160, 20, 141, 20));
        fileFrame = new QWidget(mainWindow);
        fileFrame->setObjectName("fileFrame");
        fileFrame->setGeometry(QRect(110, 50, 361, 301));

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QWidget *mainWindow)
    {
        mainWindow->setWindowTitle(QCoreApplication::translate("mainWindow", "mainWindow", nullptr));
        ServerButton->setText(QCoreApplication::translate("mainWindow", "Server", nullptr));
        ClientButton->setText(QCoreApplication::translate("mainWindow", "Client", nullptr));
        actorTitle->setText(QCoreApplication::translate("mainWindow", "Server Files", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
