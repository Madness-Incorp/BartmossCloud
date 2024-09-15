//
// Created by Oisin Lynch on 05/09/2024.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE

class mainWindow : public QWidget {
Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = nullptr);
    ~mainWindow() override;

private slots:
    void handleActorSelector();
    void sendToClient();


private:
    Ui::mainWindow *ui;
    void filePlacer(char* fileName, int column, int row);
};


#endif //MAINWINDOW_H
