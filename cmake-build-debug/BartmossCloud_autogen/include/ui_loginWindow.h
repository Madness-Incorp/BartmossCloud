/********************************************************************************
** Form generated from reading UI file 'loginWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loginWindow
{
public:
    QLineEdit *UsernameBar;
    QLineEdit *PasswordBar;
    QPushButton *loginButton;

    void setupUi(QWidget *loginWindow)
    {
        if (loginWindow->objectName().isEmpty())
            loginWindow->setObjectName("loginWindow");
        loginWindow->resize(400, 300);
        UsernameBar = new QLineEdit(loginWindow);
        UsernameBar->setObjectName("UsernameBar");
        UsernameBar->setGeometry(QRect(140, 60, 113, 21));
        PasswordBar = new QLineEdit(loginWindow);
        PasswordBar->setObjectName("PasswordBar");
        PasswordBar->setGeometry(QRect(140, 170, 113, 21));
        PasswordBar->setEchoMode(QLineEdit::EchoMode::Password);
        loginButton = new QPushButton(loginWindow);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(150, 230, 91, 32));

        retranslateUi(loginWindow);

        QMetaObject::connectSlotsByName(loginWindow);
    } // setupUi

    void retranslateUi(QWidget *loginWindow)
    {
        loginWindow->setWindowTitle(QCoreApplication::translate("loginWindow", "loginWindow", nullptr));
        loginWindow->setStyleSheet(QCoreApplication::translate("loginWindow", "\n"
"    background-color: #1e1e1e; /* Dark background */\n"
"   ", nullptr));
        UsernameBar->setStyleSheet(QCoreApplication::translate("loginWindow", "\n"
"     border: 2px solid #ff073a; /* Neon red border */\n"
"     color: white; /* Optional: white text color */\n"
"     background-color: #333333; /* Optional: dark background for input */\n"
"    ", nullptr));
        PasswordBar->setStyleSheet(QCoreApplication::translate("loginWindow", "\n"
"     border: 2px solid #ff073a; /* Neon red border */\n"
"     color: white; /* Optional: white text color */\n"
"     background-color: #333333; /* Optional: dark background for input */\n"
"    ", nullptr));
        loginButton->setText(QCoreApplication::translate("loginWindow", "Login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loginWindow: public Ui_loginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
