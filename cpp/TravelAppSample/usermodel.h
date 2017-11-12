#pragma once

#include <QJsonObject>
#include <QString>
#include <QList>

class UserModel
{
public:
    UserModel(QString username);

    QString username();

    QList<QJsonObject> shoppingCart();
    void addToCart(QJsonObject flight);
    void removeFromCart(int index);
    double totalShoppingCartPrice();

    void book();
    QList<QJsonObject> bookings();

private:
    void loadShoppingCart();
    void saveCurrentShoppingCart();

private:
    QString mUsername;
    QList<QJsonObject> mShoppingCart;
};

