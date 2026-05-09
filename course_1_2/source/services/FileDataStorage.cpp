#include "services/FileDataStorage.h"

#include "services/TaxiService.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace
{
std::vector<std::string> split(const std::string& line)
{
    std::vector<std::string> parts;
    std::stringstream stream(line);
    std::string part;
    while (std::getline(stream, part, '|'))
    {
        parts.push_back(part);
    }
    return parts;
}

std::string cleanField(std::string value)
{
    for (char& ch : value)
    {
        if (ch == '|' || ch == '\n' || ch == '\r')
        {
            ch = ' ';
        }
    }
    return value;
}

bool parseBool(const std::string& value)
{
    return value == "1" || value == "true" || value == "да";
}

void ensureFieldCount(const std::vector<std::string>& fields, std::size_t expected, const std::string& fileName)
{
    if (fields.size() != expected)
    {
        throw std::runtime_error("Некорректная строка в файле " + fileName);
    }
}
}

FileDataStorage::FileDataStorage(std::string directory)
    : directory(std::move(directory))
{
}

void FileDataStorage::load(TaxiService& service)
{
    service.clear();

    {
        std::ifstream input(pathFor("clients.txt"));
        std::string line;
        while (std::getline(input, line))
        {
            if (line.empty())
            {
                continue;
            }
            const auto fields = split(line);
            ensureFieldCount(fields, 3, "clients.txt");
            service.addClientWithId(std::stoi(fields[0]), fields[1], fields[2]);
        }
    }

    {
        std::ifstream input(pathFor("cars.txt"));
        std::string line;
        while (std::getline(input, line))
        {
            if (line.empty())
            {
                continue;
            }
            const auto fields = split(line);
            ensureFieldCount(fields, 6, "cars.txt");
            service.addVehicleWithId(std::stoi(fields[0]), fields[1], fields[2], fields[3], fields[4], parseBool(fields[5]));
        }
    }

    {
        std::ifstream input(pathFor("drivers.txt"));
        std::string line;
        while (std::getline(input, line))
        {
            if (line.empty())
            {
                continue;
            }
            const auto fields = split(line);
            ensureFieldCount(fields, 6, "drivers.txt");
            service.addDriverWithId(
                std::stoi(fields[0]),
                fields[1],
                fields[2],
                std::stoi(fields[3]),
                std::stoi(fields[4]),
                parseBool(fields[5]));
        }
    }

    {
        std::ifstream input(pathFor("orders.txt"));
        std::string line;
        while (std::getline(input, line))
        {
            if (line.empty())
            {
                continue;
            }
            const auto fields = split(line);
            ensureFieldCount(fields, 10, "orders.txt");
            service.addOrderWithId(
                std::stoi(fields[0]),
                std::stoi(fields[1]),
                std::stoi(fields[2]),
                std::stoi(fields[3]),
                fields[4],
                fields[5],
                fields[6],
                std::stod(fields[7]),
                std::stod(fields[8]),
                orderStatusFromString(fields[9]));
        }
    }
}

void FileDataStorage::save(const TaxiService& service) const
{
    std::filesystem::create_directories(directory);

    {
        std::ofstream output(pathFor("clients.txt"));
        for (const Client& client : service.getClients())
        {
            output << client.getId() << '|'
                   << cleanField(client.getName()) << '|'
                   << cleanField(client.getPhone()) << '\n';
        }
    }

    {
        std::ofstream output(pathFor("cars.txt"));
        for (const auto& vehicle : service.getVehicles())
        {
            output << vehicle->getId() << '|'
                   << vehicle->getCategory() << '|'
                   << cleanField(vehicle->getBrand()) << '|'
                   << cleanField(vehicle->getModel()) << '|'
                   << cleanField(vehicle->getPlateNumber()) << '|'
                   << (vehicle->isAvailable() ? 1 : 0) << '\n';
        }
    }

    {
        std::ofstream output(pathFor("drivers.txt"));
        for (const Driver& driver : service.getDrivers())
        {
            output << driver.getId() << '|'
                   << cleanField(driver.getName()) << '|'
                   << cleanField(driver.getPhone()) << '|'
                   << driver.getVehicleId() << '|'
                   << driver.getExperienceYears() << '|'
                   << (driver.isAvailable() ? 1 : 0) << '\n';
        }
    }

    {
        std::ofstream output(pathFor("orders.txt"));
        for (const Order& order : service.getOrders())
        {
            output << order.getId() << '|'
                   << order.getClientId() << '|'
                   << order.getDriverId() << '|'
                   << order.getVehicleId() << '|'
                   << cleanField(order.getTariffName()) << '|'
                   << cleanField(order.getStartAddress()) << '|'
                   << cleanField(order.getDestinationAddress()) << '|'
                   << order.getDistanceKm() << '|'
                   << order.getPrice() << '|'
                   << orderStatusToString(order.getStatus()) << '\n';
        }
    }
}

std::filesystem::path FileDataStorage::pathFor(const std::string& fileName) const
{
    return directory / fileName;
}
