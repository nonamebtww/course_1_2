#include "models/Vehicle.h"

#include <utility>

Vehicle::Vehicle(int id, std::string brand, std::string model, std::string plateNumber, bool available)
    : id(id),
      brand(std::move(brand)),
      model(std::move(model)),
      plateNumber(std::move(plateNumber)),
      available(available)
{
}

int Vehicle::getId() const
{
    return id;
}

const std::string& Vehicle::getBrand() const
{
    return brand;
}

const std::string& Vehicle::getModel() const
{
    return model;
}

const std::string& Vehicle::getPlateNumber() const
{
    return plateNumber;
}

bool Vehicle::isAvailable() const
{
    return available;
}

void Vehicle::setBrand(const std::string& value)
{
    brand = value;
}

void Vehicle::setModel(const std::string& value)
{
    model = value;
}

void Vehicle::setPlateNumber(const std::string& value)
{
    plateNumber = value;
}

void Vehicle::setAvailable(bool value)
{
    available = value;
}

void Vehicle::print(std::ostream& output) const
{
    output << "[" << getId() << "] " << getCategory() << ": " << getBrand()
           << " " << getModel()
           << ", номер: " << getPlateNumber()
           << ", статус: " << (available ? "свободен" : "занят");
}

EconomyCar::EconomyCar(int id, std::string brand, std::string model, std::string plateNumber, bool available)
    : Vehicle(id, std::move(brand), std::move(model), std::move(plateNumber), available)
{
}

std::string EconomyCar::getCategory() const
{
    return "Economy";
}

ComfortCar::ComfortCar(int id, std::string brand, std::string model, std::string plateNumber, bool available)
    : Vehicle(id, std::move(brand), std::move(model), std::move(plateNumber), available)
{
}

std::string ComfortCar::getCategory() const
{
    return "Comfort";
}
