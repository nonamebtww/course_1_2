#pragma once

#include "abstractions/IEntity.h"
#include "abstractions/IPrintable.h"

#include <ostream>
#include <string>

enum class OrderStatus
{
    Created,
    OnWay,
    Completed,
    Cancelled
};

std::string orderStatusToString(OrderStatus status);
OrderStatus orderStatusFromString(const std::string& value);
bool isTerminalStatus(OrderStatus status);

class Order final : public IEntity, public IPrintable
{
public:
    Order(
        int id,
        int clientId,
        int driverId,
        int vehicleId,
        std::string tariffName,
        std::string startAddress,
        std::string destinationAddress,
        double distanceKm,
        double price,
        OrderStatus status = OrderStatus::Created);

    int getId() const override;
    int getClientId() const;
    int getDriverId() const;
    int getVehicleId() const;
    const std::string& getTariffName() const;
    const std::string& getStartAddress() const;
    const std::string& getDestinationAddress() const;
    double getDistanceKm() const;
    double getPrice() const;
    OrderStatus getStatus() const;

    void setStatus(OrderStatus value);
    void print(std::ostream& output) const override;

private:
    int id;
    int clientId;
    int driverId;
    int vehicleId;
    std::string tariffName;
    std::string startAddress;
    std::string destinationAddress;
    double distanceKm;
    double price;
    OrderStatus status;
};
