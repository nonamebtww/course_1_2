#include "models/Person.h"

#include <utility>

Person::Person(int id, std::string name, std::string phone)
    : id(id), name(std::move(name)), phone(std::move(phone))
{
}

int Person::getId() const
{
    return id;
}

const std::string& Person::getName() const
{
    return name;
}

const std::string& Person::getPhone() const
{
    return phone;
}

void Person::setName(const std::string& value)
{
    name = value;
}

void Person::setPhone(const std::string& value)
{
    phone = value;
}

void Person::print(std::ostream& output) const
{
    output << "[" << getId() << "] " << getRole() << ": " << getName() << ", телефон: " << getPhone();
}

Client::Client(int id, std::string name, std::string phone)
    : Person(id, std::move(name), std::move(phone))
{
}

std::string Client::getRole() const
{
    return "Клиент";
}

Driver::Driver(int id, std::string name, std::string phone, int vehicleId, int experienceYears, bool available)
    : Person(id, std::move(name), std::move(phone)),
      vehicleId(vehicleId),
      experienceYears(experienceYears),
      available(available)
{
}

std::string Driver::getRole() const
{
    return "Водитель";
}

int Driver::getVehicleId() const
{
    return vehicleId;
}

int Driver::getExperienceYears() const
{
    return experienceYears;
}

bool Driver::isAvailable() const
{
    return available;
}

void Driver::setVehicleId(int value)
{
    vehicleId = value;
}

void Driver::setExperienceYears(int value)
{
    experienceYears = value;
}

void Driver::setAvailable(bool value)
{
    available = value;
}

void Driver::print(std::ostream& output) const
{
    Person::print(output);
    output << ", автомобиль ID: " << vehicleId
           << ", стаж: " << experienceYears
           << ", статус: " << (available ? "свободен" : "занят");
}
