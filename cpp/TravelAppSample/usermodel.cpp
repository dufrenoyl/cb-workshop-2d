
#include <QJsonObject>
#include <QJsonArray>

#include "usermodel.h"
#include "cbdatasourcefactory.h"

UserModel::UserModel(QString username)
    : mUsername(username)
{

}

QString UserModel::username()
{
    return mUsername;
}

void UserModel::loadShoppingCart()
{
    mShoppingCart.clear();
    QJsonObject result = CBDataSourceFactory::Instance().Get("user::" + mUsername).asJson();
    QJsonArray aShoppingCart = result["shoppingcart"].toArray();
    for (QJsonArray::iterator it = aShoppingCart.begin(); it != aShoppingCart.end(); ++it)
    {
        mShoppingCart.append((*it).toObject());
    }
}

void UserModel::saveCurrentShoppingCart()
{
    QJsonObject result = CBDataSourceFactory::Instance().Get("user::" + mUsername).asJson();
    QJsonArray aShoppingCart;
    for (QList<QJsonObject>::iterator it = mShoppingCart.begin(); it != mShoppingCart.end(); ++it)
    {
        aShoppingCart.append(*it);
    }
    result["shoppingcart"] = aShoppingCart;
    CBDataSourceFactory::Instance().Upsert("user::" + mUsername, result);
}

QList<QJsonObject> UserModel::shoppingCart()
{
    loadShoppingCart();
    return mShoppingCart;
}

void UserModel::addToCart(QJsonObject flight)
{
    mShoppingCart.append(flight);
    saveCurrentShoppingCart();
}

double UserModel::totalShoppingCartPrice()
{
    double result = 0;
    for (QList<QJsonObject>::iterator it = mShoppingCart.begin(); it != mShoppingCart.end(); ++it)
    {
        result += it->value("price").toDouble();
    }
    return result;
}

void UserModel::removeFromCart(int index)
{
    mShoppingCart.removeAt(index);
    saveCurrentShoppingCart();
}

void UserModel::book()
{
    QJsonObject result = CBDataSourceFactory::Instance().Get("user::" + mUsername).asJson();
    QJsonArray aBookings = result["bookings"].toArray();
    for (QList<QJsonObject>::iterator it = mShoppingCart.begin(); it != mShoppingCart.end(); ++it)
    {
        aBookings.append(*it);
    }
    result["bookings"] = aBookings;
    result["shoppingcart"] = QJsonArray(); // clear shopping cart
    CBDataSourceFactory::Instance().Upsert("user::" + mUsername, result);
}

QList<QJsonObject> UserModel::bookings()
{
    QList<QJsonObject> returnvalue;
    QJsonObject result = CBDataSourceFactory::Instance().Get("user::" + mUsername).asJson();
    QJsonArray aShoppingCart = result["bookings"].toArray();
    for (QJsonArray::iterator it = aShoppingCart.begin(); it != aShoppingCart.end(); ++it)
    {
        returnvalue.append((*it).toObject());
    }
    return returnvalue;
}



