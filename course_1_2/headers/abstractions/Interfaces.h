#pragma once

#include <iosfwd>

class TaxiService;

class IEntity
{
public:
    virtual ~IEntity() = default;
    virtual int getId() const = 0;
};

class IPrintable
{
public:
    virtual ~IPrintable() = default;
    virtual void print(std::ostream& output) const = 0;
};

class IDataStorage
{
public:
    virtual ~IDataStorage() = default;
    virtual void load(TaxiService& service) = 0;
    virtual void save(const TaxiService& service) const = 0;
};
