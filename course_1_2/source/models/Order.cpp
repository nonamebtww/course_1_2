#include "models/Order.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>

std::string orderStatusToString(OrderStatus status)
{
    switch (status)
    {
    case OrderStatus::Created:
        return "создан";
    case OrderStatus::OnWay:
        return "в пути";
    case OrderStatus::Completed:
        return "завершен";
    case OrderStatus::Cancelled:
        return "отменен";
    default:
        return "неизвестно";
    }
}

OrderStatus orderStatusFromString(const std::string& value)
{
    if (value == "создан" || value == "Created")
    {
        return OrderStatus::Created;
    }
    if (value == "в пути" || value == "OnWay")
    {
        return OrderStatus::OnWay;
    }
    if (value == "завершен" || value == "Completed")
    {
        return OrderStatus::Completed;
    }
    if (value == "отменен" || value == "Cancelled")
    {
        return OrderStatus::Cancelled;
    }
    throw std::runtime_error("Неизвестный статус заказа: " + value);
}

bool isTerminalStatus(OrderStatus status)
{
    return status == OrderStatus::Completed || status == OrderStatus::Cancelled;
}

Order::Order(
    int id,
    int clientId,
    int driverId,
    int vehicleId,
    std::string tariffName,
    std::string startAddress,
    std::string destinationAddress,
    double distanceKm,
    double price,
    OrderStatus status)
    : id(id),
      clientId(clientId),
      driverId(driverId),
      vehicleId(vehicleId),
      tariffName(std::move(tariffName)),
      startAddress(std::move(startAddress)),
      destinationAddress(std::move(destinationAddress)),
      distanceKm(distanceKm),
      price(price),
      status(status)
{
}

int Order::getId() const
{
    return id;
}

int Order::getClientId() const
{
    return clientId;
}

int Order::getDriverId() const
{
    return driverId;
}

int Order::getVehicleId() const
{
    return vehicleId;
}

const std::string& Order::getTariffName() const
{
    return tariffName;
}

const std::string& Order::getStartAddress() const
{
    return startAddress;
}

const std::string& Order::getDestinationAddress() const
{
    return destinationAddress;
}

double Order::getDistanceKm() const
{
    return distanceKm;
}

double Order::getPrice() const
{
    return price;
}

OrderStatus Order::getStatus() const
{
    return status;
}

void Order::setStatus(OrderStatus value)
{
    status = value;
}

void Order::print(std::ostream& output) const
{
    std::ostringstream priceOutput;
    priceOutput << std::fixed << std::setprecision(2) << price;

    output << "[" << id << "] клиент ID: " << clientId
           << ", водитель ID: " << driverId
           << ", авто ID: " << vehicleId
           << ", тариф: " << tariffName
           << ", маршрут: " << startAddress << " -> " << destinationAddress
           << ", дистанция: " << distanceKm << " км"
           << ", цена: " << priceOutput.str()
           << ", статус: " << orderStatusToString(status);
}
