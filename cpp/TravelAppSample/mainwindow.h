#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "JsonTableModel.h"
#include "usermodel.h"
#include "cbn1qlresult.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();    

private slots:
    void fromTextEdited(const QString&);
    void toTextEdited(const QString&);
    void fromSelectionChanged();
    void toSelectionChanged();
    void buttonFindFlightsPressed();
    void findFlights();
    void addToCartOutbound();
    void addToCartInbound();
    void removeFromCart();
    void book();

private:
    void login();
    CBN1qlResult queryAirport(QString txt);
    void updateShoppingCart();
    CBN1qlResult findFlights(QString from, QString to, QDate when);

private:
    Ui::MainWindow *ui;
    JsonTablemodel *mOutboundFlights;
    JsonTablemodel *mInboundFlights;
    JsonTablemodel *mShoppingCart;
    JsonTablemodel *mBookings;
    UserModel mUser;
};

#endif // MAINWINDOW_H
