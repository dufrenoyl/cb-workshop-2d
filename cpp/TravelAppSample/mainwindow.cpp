#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <cmath>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbdatasourcefactory.h"
#include "login.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mOutboundFlights(new JsonTablemodel(this)),
    mInboundFlights(new JsonTablemodel(this)),
    mShoppingCart(new JsonTablemodel(this)),
    mBookings(new JsonTablemodel(this)),
    mUser("N/A")
{
    ui->setupUi(this);
    connect(ui->fromLineEdit, SIGNAL(textEdited(const QString&)), SLOT(fromTextEdited(const QString&)));
    connect(ui->toLineEdit, SIGNAL(textEdited(const QString&)), SLOT(toTextEdited(const QString&)));
    connect(ui->findPushButton, SIGNAL(clicked(bool)), SLOT(buttonFindFlightsPressed()));
    connect(ui->acFromlistWidget, SIGNAL(itemSelectionChanged()), SLOT(fromSelectionChanged()));
    connect(ui->acTolistWidget, SIGNAL(itemSelectionChanged()), SLOT(toSelectionChanged()));
    connect(ui->pbAddToCartOutbound, SIGNAL(clicked(bool)), SLOT(addToCartOutbound()));
    connect(ui->pbAddToCartInbound, SIGNAL(clicked(bool)), SLOT(addToCartInbound()));
    connect(ui->pbRemoveFromCart, SIGNAL(clicked(bool)), SLOT(removeFromCart()));
    connect(ui->pbBook, SIGNAL(clicked(bool)), SLOT(book()));

    mOutboundFlights->addMapping("name", "Airline");
    mOutboundFlights->addMapping("flight", "Flight");
    mOutboundFlights->addMapping("utc", "Departure");
    mOutboundFlights->addMapping("sourceairport", "From");
    mOutboundFlights->addMapping("destinationairport", "To");
    mOutboundFlights->addMapping("equipment", "Aircraft");
    mOutboundFlights->addMapping("price", "Price");
    ui->tvOutboundFlights->setModel(mOutboundFlights);

    mInboundFlights->addMapping("name", "Airline");
    mInboundFlights->addMapping("flight", "Flight");
    mInboundFlights->addMapping("utc", "Departure");
    mInboundFlights->addMapping("sourceairport", "From");
    mInboundFlights->addMapping("destinationairport", "To");
    mInboundFlights->addMapping("equipment", "Aircraft");
    mInboundFlights->addMapping("price", "Price");
    ui->tvInboundFlights->setModel(mInboundFlights);

    mShoppingCart->addMapping("name", "Airline");
    mShoppingCart->addMapping("flight", "Flight");
    mShoppingCart->addMapping("utc", "Departure");
    mShoppingCart->addMapping("sourceairport", "From");
    mShoppingCart->addMapping("destinationairport", "To");
    mShoppingCart->addMapping("equipment", "Aircraft");
    mShoppingCart->addMapping("price", "Price");
    ui->tvShoppingCart->setModel(mShoppingCart);

    mBookings->addMapping("name", "Airline");
    mBookings->addMapping("flight", "Flight");
    mBookings->addMapping("utc", "Departure");
    mBookings->addMapping("sourceairport", "From");
    mBookings->addMapping("destinationairport", "To");
    mBookings->addMapping("equipment", "Aircraft");
    mBookings->addMapping("price", "Price");
    ui->tvBookings->setModel(mBookings);
    ui->tabWidget->setCurrentIndex(0);

    ui->LeaveDateEdit->setDate(QDate::currentDate());
    ui->ReturnDateEdit->setDate(QDate::currentDate());

    login();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login()
{
    Login login;
    bool loggedIn = false;
    while (!loggedIn)
    {
        if (login.exec() == QDialog::Accepted)
        {
            loggedIn = login.createUserOrLogin();
            if (!loggedIn)
            {
                QMessageBox::critical(NULL, "Could not log in", "Error logging in, please try again");
            }
        }
        else
        {
            exit(0);
        }
    }
    mUser = UserModel(login.userName());
    ui->lblUsername->setText(mUser.username());
    updateShoppingCart();
}

void MainWindow::updateShoppingCart()
{
    QList<QJsonObject> data = mUser.shoppingCart();
    mShoppingCart->setData(data);
    ui->lblTotalPrice->setText("$" + QString::number(mUser.totalShoppingCartPrice()));
    ui->tabWidget->setTabText(2, QString("Shopping Cart (%1)").arg(data.count()));

    data = mUser.bookings();
    mBookings->setData(data);
    ui->tabWidget->setTabText(3, QString("Bookings (%1)").arg(data.count()));
}

CBN1qlResult MainWindow::queryAirport(QString txt)
{
    QString queryPrep;
    if (txt.length() == 3)
    {
        queryPrep = "SELECT airportname FROM `travel-sample` WHERE faa = '" + txt.toUpper() + "'";
    }
    else if (txt.length() == 4 && (txt==txt.toUpper()||txt==txt.toLower()))
    {
        queryPrep = "SELECT airportname FROM `travel-sample` WHERE icao = '" + txt.toUpper() + "'";
    }
    else
    {
        queryPrep = "SELECT airportname FROM `travel-sample` WHERE airportname LIKE '" + txt.toUpper() + "'";
    }

    return CBDataSourceFactory::Instance().QueryN1ql(queryPrep);
}

void MainWindow::fromTextEdited(const QString& txt)
{
    ui->acFromlistWidget->clear();

    CBN1qlResult result = queryAirport(txt);

    for (QList<QJsonObject>::const_iterator it = result.items.cbegin(); it != result.items.cend(); ++it)
    {
        QString name = it->value("airportname").toString();
        ui->acFromlistWidget->addItem(name);
    }
}

void MainWindow::toTextEdited(const QString& txt)
{
    ui->acTolistWidget->clear();

    CBN1qlResult result = queryAirport(txt);
    for (QList<QJsonObject>::const_iterator it = result.items.cbegin(); it != result.items.cend(); ++it)
    {
        QString name = (*it)["airportname"].toString();
        ui->acTolistWidget->addItem(name);
    }
}

void MainWindow::fromSelectionChanged()
{
    QList<QListWidgetItem*> items = ui->acFromlistWidget->selectedItems();
    if (items.count() > 0)
    {
        QString item = items.first()->text();
        ui->fromLineEdit->setText(item);
        ui->acFromlistWidget->clear();
    }
}

void MainWindow::toSelectionChanged()
{
    QList<QListWidgetItem*> items = ui->acTolistWidget->selectedItems();
    if (items.count() > 0)
    {
        QString item = items.first()->text();
        ui->toLineEdit->setText(item);
        ui->acTolistWidget->clear();
    }
}

void MainWindow::buttonFindFlightsPressed()
{
    ui->findPushButton->setEnabled(false);
    ui->findPushButton->setText("Please wait...");
    QTimer::singleShot(100, this, SLOT(findFlights()));
}

#define pi 3.14159265358979323846

double deg2rad(double deg)
{
  return (deg * pi / 180);
}
double rad2deg(double rad)
{
  return (rad * 180 / pi);
}

double distance(double lat1, double lon1, double lat2, double lon2, char unit)
{
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;
  switch(unit) {
    case 'M':
      break;
    case 'K':
      dist = dist * 1.609344;
      break;
    case 'N':
      dist = dist * 0.8684;
      break;
  }
  return (dist);
}

void MainWindow::findFlights()
{
    QString from = ui->fromLineEdit->text();
    QString to = ui->toLineEdit->text();
    QDate leaveDate = ui->LeaveDateEdit->date();
    QDate returnDate = ui->ReturnDateEdit->date();

    CBN1qlResult result = findFlights(from, to, leaveDate);
    mOutboundFlights->setData(result.items);

    if (ui->roundTripCheckbox->isChecked())
    {
        result = findFlights(to, from, returnDate);
        mInboundFlights->setData(result.items);
        ui->lblInboundLeg->setVisible(true);
        ui->tvInboundFlights->setVisible(true);
        ui->pbAddToCartInbound->setVisible(true);
    }
    else
    {
        mInboundFlights->setData(QList<QJsonObject>());
        ui->lblInboundLeg->setVisible(false);
        ui->tvInboundFlights->setVisible(false);
        ui->pbAddToCartInbound->setVisible(false);
    }

    ui->tabWidget->setCurrentIndex(1);
    ui->findPushButton->setEnabled(true);
    ui->findPushButton->setText("Find Flights");

}

CBN1qlResult MainWindow::findFlights(QString from, QString to, QDate when)
{
    QString queryPrep = "SELECT faa as fromAirport,geo FROM `travel-sample` WHERE airportname = '" + from +
        "' UNION SELECT faa as toAirport,geo FROM `travel-sample` WHERE airportname = '" + to + "'";

    CBN1qlResult result = CBDataSourceFactory::Instance().QueryN1ql(queryPrep);

    QString queryFrom;
    double startLat;
    double startLon;

    QString queryTo;
    double endLat;
    double endLon;

    for (QList<QJsonObject>::const_iterator it = result.items.cbegin(); it != result.items.cend(); ++it)
    {
        if (it->contains("toAirport"))
        {
            queryTo = it->value("toAirport").toString();
            endLat = it->value("geo").toObject().value("lat").toDouble();
            endLon = it->value("geo").toObject().value("lon").toDouble();
        }
        else if (it->contains("fromAirport"))
        {
            queryFrom = it->value("fromAirport").toString();
            startLat = it->value("geo").toObject().value("lat").toDouble();
            startLon = it->value("geo").toObject().value("lon").toDouble();
        }
    }

    double dist = distance(startLat, startLon, endLat, endLon, 'K');
    double flightTime = round(dist / 800); // avgKmHour
    double price = round(dist * 0.1); // distanceCostMultiplier
    int travellers = ui->travelersComboBox->currentText().toInt();

    int dayofWeek = when.dayOfWeek();
    if (dayofWeek == 7)
    {
        dayofWeek = 0;
    }

    queryPrep = "SELECT r.id, a.name, s.flight, s.utc, r.sourceairport, r.destinationairport, r.equipment FROM `travel-sample` r UNNEST r.schedule s JOIN `travel-sample` a ON KEYS r.airlineid WHERE r.sourceairport='" + queryFrom +
    "' AND r.destinationairport='" + queryTo + "' AND s.day=" + QString::number(dayofWeek) + " ORDER BY a.name";

    result = CBDataSourceFactory::Instance().QueryN1ql(queryPrep);

    for (QList<QJsonObject>::iterator it = result.items.begin(); it != result.items.end(); ++it)
    {
        it->insert("flighttime", flightTime);
        it->insert("price", travellers * round(price * ((100 - (floor(rand() % 20 + 1))) / 100)));
    }
    return result;
}

void MainWindow::addToCartOutbound()
{
    QModelIndexList selection = ui->tvOutboundFlights->selectionModel()->selectedRows();
    if (selection.count() != 1)
    {
        return;
    }

    QJsonObject selectedFlight = mOutboundFlights->jsonData(selection.first());

    QString airlineName = selectedFlight["name"].toString();
    QList<QJsonObject> inbound = mInboundFlights->jsonData();
    QList<QJsonObject> inboundCleaned;
    for (QList<QJsonObject>::const_iterator it = inbound.cbegin(); it != inbound.cend(); ++it)
    {
        if (it->value("name") == airlineName)
        {
            inboundCleaned.append(*it);
        }
    }
    mInboundFlights->setData(inboundCleaned);

    mUser.addToCart(selectedFlight);
    updateShoppingCart();
}

void MainWindow::addToCartInbound()
{
    QModelIndexList selection = ui->tvInboundFlights->selectionModel()->selectedRows();
    if (selection.count() != 1)
    {
        return;
    }

    mUser.addToCart(mInboundFlights->jsonData(selection.first()));
    updateShoppingCart();
}

void MainWindow::removeFromCart()
{
    QModelIndexList selection = ui->tvShoppingCart->selectionModel()->selectedRows();
    if (selection.count() != 1)
    {
        return;
    }

    mUser.removeFromCart(selection.first().row());
    updateShoppingCart();
}

void MainWindow::book()
{
    mUser.book();
    updateShoppingCart();
}



