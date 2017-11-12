#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    bool createUserOrLogin();

    QString userName();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
