//
// Created by Oisin Lynch on 01/11/2024.
//

#ifndef CREATE_ACCOUNT_PAGE_H
#define CREATE_ACCOUNT_PAGE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Create_Account_Page; }
QT_END_NAMESPACE

class Create_Account_Page : public QWidget {
Q_OBJECT

public:
    explicit Create_Account_Page(QWidget *parent = nullptr);
    ~Create_Account_Page() override;

private:
    Ui::Create_Account_Page *ui;
};


#endif //CREATE_ACCOUNT_PAGE_H
