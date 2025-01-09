#ifndef SETUPPAGE_H
#define SETUPPAGE_H
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class setUpPage : public QWidget {
    Q_OBJECT

public:
    explicit setUpPage(QWidget *parent = nullptr);

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *nextButton;
    QPushButton *finishButton;
    void setUp1();
    void setUp2();
    void setUp3();
    void setUp4();
    void finishSetup();
    void clearLayout();

private slots:
    void goToStep2() {
        setUp2();
    }

    void goToStep3() {
        setUp3();
    }

    void goToStep4() {
        setUp4();
    }
};

#endif // SETUPPAGE_H
