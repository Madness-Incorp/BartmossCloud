//
// Created by Oisin Lynch on 01/11/2024.
//

#ifndef CREATEACCOUNTWINDOW_H
#define CREATEACCOUNTWINDOW_H
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class createAccountWindow; }
QT_END_NAMESPACE

class createAccountWindow : public QWidget {
Q_OBJECT

public:
    explicit createAccountWindow(QWidget *parent = nullptr);
    ~createAccountWindow() override;

private slots:
    void handleAccountCreation();

private:
    Ui::createAccountWindow *ui;
};


#endif //CREATEACCOUNTWINDOW_H
