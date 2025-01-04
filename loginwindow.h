//
// Created by Oisin Lynch on 04/09/2024.
//

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QLineEdit>
#include <QPushButton>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class loginWindow; }
QT_END_NAMESPACE

class loginWindow : public QWidget {
Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = nullptr);
    ~loginWindow() override;

private slots:
    void handleLogin();

private:
    Ui::loginWindow *ui;
    QWidget *mainwindow{};
    void showMainWindow();
    QWidget *createAccountWindow{};
    void showCreateAccountWindow();
    void onCreateAccountWindowClosed();
};

#endif //LOGINWINDOW_H
