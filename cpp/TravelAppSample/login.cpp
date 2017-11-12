#include "login.h"
#include "ui_login.h"
#include "cbdatasourcefactory.h"
#include "cbdatasource.h"

#include <QCryptographicHash>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

bool Login::createUserOrLogin()
{
    if (ui->lnEdpassword->text().isEmpty() || ui->lnEdUsername->text().isEmpty())
    {
        return false;
    }
    QJsonObject result = CBDataSourceFactory::Instance().Get("user::" + ui->lnEdUsername->text()).asJson();

    QByteArray baPassword = ui->lnEdpassword->text().toUtf8();
    QString hashedPassword = QString(QCryptographicHash::hash(baPassword, QCryptographicHash::Sha3_512).toHex());

    if (ui->cbCreateNewUser->isChecked())
    {
        // check if user exists
        if (!result.contains("username"))
        {
            // create
            QJsonObject newUser;
            newUser["username"] = ui->lnEdUsername->text();
            newUser["password"] = hashedPassword;
            CBDataSourceFactory::Instance().Upsert("user::" + ui->lnEdUsername->text(), newUser);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (result.contains("username"))
        {
            if (result["password"] == hashedPassword)
            {
                return true;
            }
        }
        return false;
    }
}

QString Login::userName()
{
    return ui->lnEdUsername->text();
}

Login::~Login()
{
    delete ui;
}
