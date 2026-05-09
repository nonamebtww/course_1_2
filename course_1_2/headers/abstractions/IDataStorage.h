#pragma once

class TaxiService;

class IDataStorage
{
public:
    virtual ~IDataStorage() = default;
    virtual void load(TaxiService& service) = 0;
    virtual void save(const TaxiService& service) const = 0;
};
