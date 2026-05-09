#include "models/Tariff.h"

std::string EconomyTariff::getName() const
{
    return "Эконом";
}

std::string EconomyTariff::getRequiredVehicleCategory() const
{
    return "Economy";
}

double EconomyTariff::calculatePrice(double distanceKm) const
{
    return 100.0 + distanceKm * 20.0;
}

std::string ComfortTariff::getName() const
{
    return "Комфорт";
}

std::string ComfortTariff::getRequiredVehicleCategory() const
{
    return "Comfort";
}

double ComfortTariff::calculatePrice(double distanceKm) const
{
    return 150.0 + distanceKm * 35.0;
}
