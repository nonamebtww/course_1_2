#pragma once

#include "abstractions/IEntity.h"
#include "abstractions/IPrintable.h"

#include <ostream>
#include <string>

class Person : public IEntity, public IPrintable
{
public:
    Person(int id, std::string name, std::string phone);
    ~Person() override = default;

    int getId() const override;
    const std::string& getName() const;
    const std::string& getPhone() const;

    void setName(const std::string& value);
    void setPhone(const std::string& value);

    virtual std::string getRole() const = 0;
    void print(std::ostream& output) const override;

private:
    int id;
    std::string name;
    std::string phone;
};

class Client final : public Person
{
public:
    Client(int id, std::string name, std::string phone);
    std::string getRole() const override;
};

class Driver final : public Person
{
public:
    Driver(int id, std::string name, std::string phone, int vehicleId, int experienceYears, bool available = true);

    std::string getRole() const override;
    int getVehicleId() const;
    int getExperienceYears() const;
    bool isAvailable() const;

    void setVehicleId(int value);
    void setExperienceYears(int value);
    void setAvailable(bool value);
    void print(std::ostream& output) const override;

private:
    int vehicleId;
    int experienceYears;
    bool available;
};
