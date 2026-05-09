#pragma once

#include <string>

class Tariff
{
public:
    virtual ~Tariff() = default;
    virtual std::string getName() const = 0;
    virtual std::string getRequiredVehicleCategory() const = 0;
    virtual double calculatePrice(double distanceKm) const = 0;
};

class EconomyTariff final : public Tariff
{
public:
    std::string getName() const override;
    std::string getRequiredVehicleCategory() const override;
    double calculatePrice(double distanceKm) const override;
};

class ComfortTariff final : public Tariff
{
public:
    std::string getName() const override;
    std::string getRequiredVehicleCategory() const override;
    double calculatePrice(double distanceKm) const override;
};
