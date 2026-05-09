#pragma once

#include "abstractions/IDataStorage.h"
#include "models/Order.h"
#include "services/TaxiService.h"

#include <string>

class ConsoleMenu
{
public:
    ConsoleMenu(TaxiService& service, IDataStorage& storage);
    void run();

private:
    void printMenu() const;
    void addClient();
    void listClients() const;
    void editClient();
    void deleteClient();
    void addVehicle();
    void listVehicles() const;
    void editVehicle();
    void deleteVehicle();
    void addDriver();
    void listDrivers() const;
    void editDriver();
    void deleteDriver();
    void createOrder();
    void listOrders() const;
    void changeOrderStatus();
    void saveData() const;
    void exitMenu();

    std::string readLine(const std::string& prompt, bool allowEmpty = false) const;
    int readInt(const std::string& prompt) const;
    double readDouble(const std::string& prompt) const;
    bool readYesNo(const std::string& prompt) const;
    std::string readVehicleCategory() const;
    std::string readTariffName() const;
    OrderStatus readOrderStatus() const;

    TaxiService& service;
    IDataStorage& storage;
    bool running;
};
