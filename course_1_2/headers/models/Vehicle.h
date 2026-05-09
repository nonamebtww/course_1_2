#pragma once

#include "abstractions/IEntity.h"
#include "abstractions/IPrintable.h"

#include <ostream>
#include <string>

class Vehicle : public IEntity, public IPrintable
{
public:
    Vehicle(int id, std::string brand, std::string model, std::string plateNumber, bool available = true);
    ~Vehicle() override = default;

    int getId() const override;
    const std::string& getBrand() const;
    const std::string& getModel() const;
    const std::string& getPlateNumber() const;
    bool isAvailable() const;

    void setBrand(const std::string& value);
    void setModel(const std::string& value);
    void setPlateNumber(const std::string& value);
    void setAvailable(bool value);

    virtual std::string getCategory() const = 0;
    void print(std::ostream& output) const override;

private:
    int id;
    std::string brand;
    std::string model;
    std::string plateNumber;
    bool available;
};

class EconomyCar final : public Vehicle
{
public:
    EconomyCar(int id, std::string brand, std::string model, std::string plateNumber, bool available = true);
    std::string getCategory() const override;
};

class ComfortCar final : public Vehicle
{
public:
    ComfortCar(int id, std::string brand, std::string model, std::string plateNumber, bool available = true);
    std::string getCategory() const override;
};
