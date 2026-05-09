#pragma once

#include "models/Order.h"
#include "models/Person.h"
#include "models/Tariff.h"
#include "models/Vehicle.h"

#include <memory>
#include <string>
#include <vector>

class TaxiService
{
public:
    TaxiService();

    Client& addClient(const std::string& name, const std::string& phone);
    Client& addClientWithId(int id, const std::string& name, const std::string& phone);
    bool updateClient(int id, const std::string& name, const std::string& phone);
    bool removeClient(int id);

    Vehicle& addVehicle(const std::string& category, const std::string& brand, const std::string& model, const std::string& plateNumber);
    Vehicle& addVehicleWithId(int id, const std::string& category, const std::string& brand, const std::string& model, const std::string& plateNumber, bool available);
    bool updateVehicle(int id, const std::string& brand, const std::string& model, const std::string& plateNumber);
    bool removeVehicle(int id);

    Driver& addDriver(const std::string& name, const std::string& phone, int vehicleId, int experienceYears);
    Driver& addDriverWithId(int id, const std::string& name, const std::string& phone, int vehicleId, int experienceYears, bool available);
    bool updateDriver(int id, const std::string& name, const std::string& phone, int vehicleId, int experienceYears);
    bool removeDriver(int id);

    Order& createOrder(
        int clientId,
        const std::string& tariffName,
        const std::string& startAddress,
        const std::string& destinationAddress,
        double distanceKm);
    Order& addOrderWithId(
        int id,
        int clientId,
        int driverId,
        int vehicleId,
        const std::string& tariffName,
        const std::string& startAddress,
        const std::string& destinationAddress,
        double distanceKm,
        double price,
        OrderStatus status);
    bool changeOrderStatus(int orderId, OrderStatus status);
    bool removeOrder(int id);

    Client* findClient(int id);
    const Client* findClient(int id) const;
    Driver* findDriver(int id);
    const Driver* findDriver(int id) const;
    Vehicle* findVehicle(int id);
    const Vehicle* findVehicle(int id) const;
    Order* findOrder(int id);
    const Order* findOrder(int id) const;
    const Tariff* findTariff(const std::string& name) const;

    const std::vector<Client>& getClients() const;
    const std::vector<Driver>& getDrivers() const;
    const std::vector<std::unique_ptr<Vehicle>>& getVehicles() const;
    const std::vector<Order>& getOrders() const;
    std::vector<std::string> getTariffNames() const;

    void clear();

private:
    bool vehicleIsAssignedToDriver(int vehicleId, int exceptDriverId = 0) const;
    bool clientHasOrders(int clientId) const;
    bool driverHasActiveOrder(int driverId) const;
    bool vehicleHasActiveOrder(int vehicleId) const;
    void releaseOrderResources(const Order& order);

    std::vector<Client> clients;
    std::vector<Driver> drivers;
    std::vector<std::unique_ptr<Vehicle>> vehicles;
    std::vector<Order> orders;
    std::vector<std::unique_ptr<Tariff>> tariffs;

    int nextClientId;
    int nextDriverId;
    int nextVehicleId;
    int nextOrderId;
};
