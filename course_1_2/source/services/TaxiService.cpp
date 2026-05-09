#include "services/TaxiService.h"

#include <algorithm>
#include <stdexcept>

TaxiService::TaxiService()
    : nextClientId(1), nextDriverId(1), nextVehicleId(1), nextOrderId(1)
{
    tariffs.push_back(std::make_unique<EconomyTariff>());
    tariffs.push_back(std::make_unique<ComfortTariff>());
}

Client& TaxiService::addClient(const std::string& name, const std::string& phone)
{
    return addClientWithId(nextClientId++, name, phone);
}

Client& TaxiService::addClientWithId(int id, const std::string& name, const std::string& phone)
{
    clients.emplace_back(id, name, phone);
    nextClientId = std::max(nextClientId, id + 1);
    return clients.back();
}

bool TaxiService::updateClient(int id, const std::string& name, const std::string& phone)
{
    Client* client = findClient(id);
    if (client == nullptr)
    {
        return false;
    }

    client->setName(name);
    client->setPhone(phone);
    return true;
}

bool TaxiService::removeClient(int id)
{
    if (clientHasOrders(id))
    {
        return false;
    }

    const auto oldSize = clients.size();
    clients.erase(
        std::remove_if(clients.begin(), clients.end(), [id](const Client& client) { return client.getId() == id; }),
        clients.end());
    return clients.size() != oldSize;
}

Vehicle& TaxiService::addVehicle(const std::string& category, const std::string& brand, const std::string& model, const std::string& plateNumber)
{
    return addVehicleWithId(nextVehicleId++, category, brand, model, plateNumber, true);
}

Vehicle& TaxiService::addVehicleWithId(int id, const std::string& category, const std::string& brand, const std::string& model, const std::string& plateNumber, bool available)
{
    if (category == "Economy")
    {
        vehicles.push_back(std::make_unique<EconomyCar>(id, brand, model, plateNumber, available));
    }
    else if (category == "Comfort")
    {
        vehicles.push_back(std::make_unique<ComfortCar>(id, brand, model, plateNumber, available));
    }
    else
    {
        throw std::runtime_error("Неизвестная категория автомобиля: " + category);
    }

    nextVehicleId = std::max(nextVehicleId, id + 1);
    return *vehicles.back();
}

bool TaxiService::updateVehicle(int id, const std::string& brand, const std::string& model, const std::string& plateNumber)
{
    Vehicle* vehicle = findVehicle(id);
    if (vehicle == nullptr)
    {
        return false;
    }

    vehicle->setBrand(brand);
    vehicle->setModel(model);
    vehicle->setPlateNumber(plateNumber);
    return true;
}

bool TaxiService::removeVehicle(int id)
{
    if (vehicleIsAssignedToDriver(id) || vehicleHasActiveOrder(id))
    {
        return false;
    }

    const auto oldSize = vehicles.size();
    vehicles.erase(
        std::remove_if(vehicles.begin(), vehicles.end(), [id](const std::unique_ptr<Vehicle>& vehicle) { return vehicle->getId() == id; }),
        vehicles.end());
    return vehicles.size() != oldSize;
}

Driver& TaxiService::addDriver(const std::string& name, const std::string& phone, int vehicleId, int experienceYears)
{
    return addDriverWithId(nextDriverId++, name, phone, vehicleId, experienceYears, true);
}

Driver& TaxiService::addDriverWithId(int id, const std::string& name, const std::string& phone, int vehicleId, int experienceYears, bool available)
{
    if (findVehicle(vehicleId) == nullptr)
    {
        throw std::runtime_error("Автомобиль с указанным ID не найден.");
    }
    if (vehicleIsAssignedToDriver(vehicleId, id))
    {
        throw std::runtime_error("Этот автомобиль уже закреплен за другим водителем.");
    }
    if (experienceYears < 0)
    {
        throw std::runtime_error("Стаж водителя не может быть отрицательным.");
    }

    drivers.emplace_back(id, name, phone, vehicleId, experienceYears, available);
    nextDriverId = std::max(nextDriverId, id + 1);
    return drivers.back();
}

bool TaxiService::updateDriver(int id, const std::string& name, const std::string& phone, int vehicleId, int experienceYears)
{
    Driver* driver = findDriver(id);
    if (driver == nullptr || findVehicle(vehicleId) == nullptr || vehicleIsAssignedToDriver(vehicleId, id) || experienceYears < 0)
    {
        return false;
    }

    driver->setName(name);
    driver->setPhone(phone);
    driver->setVehicleId(vehicleId);
    driver->setExperienceYears(experienceYears);
    return true;
}

bool TaxiService::removeDriver(int id)
{
    if (driverHasActiveOrder(id))
    {
        return false;
    }

    const auto oldSize = drivers.size();
    drivers.erase(
        std::remove_if(drivers.begin(), drivers.end(), [id](const Driver& driver) { return driver.getId() == id; }),
        drivers.end());
    return drivers.size() != oldSize;
}

Order& TaxiService::createOrder(
    int clientId,
    const std::string& tariffName,
    const std::string& startAddress,
    const std::string& destinationAddress,
    double distanceKm)
{
    if (findClient(clientId) == nullptr)
    {
        throw std::runtime_error("Клиент с указанным ID не найден.");
    }
    if (distanceKm <= 0.0)
    {
        throw std::runtime_error("Расстояние должно быть больше нуля.");
    }

    const Tariff* tariff = findTariff(tariffName);
    if (tariff == nullptr)
    {
        throw std::runtime_error("Тариф не найден.");
    }

    Driver* selectedDriver = nullptr;
    Vehicle* selectedVehicle = nullptr;
    for (Driver& driver : drivers)
    {
        Vehicle* vehicle = findVehicle(driver.getVehicleId());
        if (driver.isAvailable() && vehicle != nullptr && vehicle->isAvailable() && vehicle->getCategory() == tariff->getRequiredVehicleCategory())
        {
            selectedDriver = &driver;
            selectedVehicle = vehicle;
            break;
        }
    }

    if (selectedDriver == nullptr || selectedVehicle == nullptr)
    {
        throw std::runtime_error("Нет свободного водителя с автомобилем нужного класса.");
    }

    selectedDriver->setAvailable(false);
    selectedVehicle->setAvailable(false);

    orders.emplace_back(
        nextOrderId++,
        clientId,
        selectedDriver->getId(),
        selectedVehicle->getId(),
        tariff->getName(),
        startAddress,
        destinationAddress,
        distanceKm,
        tariff->calculatePrice(distanceKm),
        OrderStatus::Created);

    return orders.back();
}

Order& TaxiService::addOrderWithId(
    int id,
    int clientId,
    int driverId,
    int vehicleId,
    const std::string& tariffName,
    const std::string& startAddress,
    const std::string& destinationAddress,
    double distanceKm,
    double price,
    OrderStatus status)
{
    orders.emplace_back(id, clientId, driverId, vehicleId, tariffName, startAddress, destinationAddress, distanceKm, price, status);
    nextOrderId = std::max(nextOrderId, id + 1);
    return orders.back();
}

bool TaxiService::changeOrderStatus(int orderId, OrderStatus status)
{
    Order* order = findOrder(orderId);
    if (order == nullptr || isTerminalStatus(order->getStatus()))
    {
        return false;
    }

    order->setStatus(status);
    if (isTerminalStatus(status))
    {
        releaseOrderResources(*order);
    }
    return true;
}

bool TaxiService::removeOrder(int id)
{
    Order* order = findOrder(id);
    if (order == nullptr)
    {
        return false;
    }
    if (!isTerminalStatus(order->getStatus()))
    {
        releaseOrderResources(*order);
    }

    const auto oldSize = orders.size();
    orders.erase(
        std::remove_if(orders.begin(), orders.end(), [id](const Order& order) { return order.getId() == id; }),
        orders.end());
    return orders.size() != oldSize;
}

Client* TaxiService::findClient(int id)
{
    auto it = std::find_if(clients.begin(), clients.end(), [id](const Client& client) { return client.getId() == id; });
    return it == clients.end() ? nullptr : &(*it);
}

const Client* TaxiService::findClient(int id) const
{
    auto it = std::find_if(clients.begin(), clients.end(), [id](const Client& client) { return client.getId() == id; });
    return it == clients.end() ? nullptr : &(*it);
}

Driver* TaxiService::findDriver(int id)
{
    auto it = std::find_if(drivers.begin(), drivers.end(), [id](const Driver& driver) { return driver.getId() == id; });
    return it == drivers.end() ? nullptr : &(*it);
}

const Driver* TaxiService::findDriver(int id) const
{
    auto it = std::find_if(drivers.begin(), drivers.end(), [id](const Driver& driver) { return driver.getId() == id; });
    return it == drivers.end() ? nullptr : &(*it);
}

Vehicle* TaxiService::findVehicle(int id)
{
    auto it = std::find_if(vehicles.begin(), vehicles.end(), [id](const std::unique_ptr<Vehicle>& vehicle) { return vehicle->getId() == id; });
    return it == vehicles.end() ? nullptr : it->get();
}

const Vehicle* TaxiService::findVehicle(int id) const
{
    auto it = std::find_if(vehicles.begin(), vehicles.end(), [id](const std::unique_ptr<Vehicle>& vehicle) { return vehicle->getId() == id; });
    return it == vehicles.end() ? nullptr : it->get();
}

Order* TaxiService::findOrder(int id)
{
    auto it = std::find_if(orders.begin(), orders.end(), [id](const Order& order) { return order.getId() == id; });
    return it == orders.end() ? nullptr : &(*it);
}

const Order* TaxiService::findOrder(int id) const
{
    auto it = std::find_if(orders.begin(), orders.end(), [id](const Order& order) { return order.getId() == id; });
    return it == orders.end() ? nullptr : &(*it);
}

const Tariff* TaxiService::findTariff(const std::string& name) const
{
    auto it = std::find_if(tariffs.begin(), tariffs.end(), [&name](const std::unique_ptr<Tariff>& tariff) { return tariff->getName() == name; });
    return it == tariffs.end() ? nullptr : it->get();
}

const std::vector<Client>& TaxiService::getClients() const
{
    return clients;
}

const std::vector<Driver>& TaxiService::getDrivers() const
{
    return drivers;
}

const std::vector<std::unique_ptr<Vehicle>>& TaxiService::getVehicles() const
{
    return vehicles;
}

const std::vector<Order>& TaxiService::getOrders() const
{
    return orders;
}

std::vector<std::string> TaxiService::getTariffNames() const
{
    std::vector<std::string> names;
    for (const auto& tariff : tariffs)
    {
        names.push_back(tariff->getName());
    }
    return names;
}

void TaxiService::clear()
{
    clients.clear();
    drivers.clear();
    vehicles.clear();
    orders.clear();
    nextClientId = 1;
    nextDriverId = 1;
    nextVehicleId = 1;
    nextOrderId = 1;
}

bool TaxiService::vehicleIsAssignedToDriver(int vehicleId, int exceptDriverId) const
{
    return std::any_of(drivers.begin(), drivers.end(), [vehicleId, exceptDriverId](const Driver& driver) {
        return driver.getId() != exceptDriverId && driver.getVehicleId() == vehicleId;
    });
}

bool TaxiService::clientHasOrders(int clientId) const
{
    return std::any_of(orders.begin(), orders.end(), [clientId](const Order& order) { return order.getClientId() == clientId; });
}

bool TaxiService::driverHasActiveOrder(int driverId) const
{
    return std::any_of(orders.begin(), orders.end(), [driverId](const Order& order) {
        return order.getDriverId() == driverId && !isTerminalStatus(order.getStatus());
    });
}

bool TaxiService::vehicleHasActiveOrder(int vehicleId) const
{
    return std::any_of(orders.begin(), orders.end(), [vehicleId](const Order& order) {
        return order.getVehicleId() == vehicleId && !isTerminalStatus(order.getStatus());
    });
}

void TaxiService::releaseOrderResources(const Order& order)
{
    Driver* driver = findDriver(order.getDriverId());
    if (driver != nullptr)
    {
        driver->setAvailable(true);
    }

    Vehicle* vehicle = findVehicle(order.getVehicleId());
    if (vehicle != nullptr)
    {
        vehicle->setAvailable(true);
    }
}
