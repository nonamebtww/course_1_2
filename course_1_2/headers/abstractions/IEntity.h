#pragma once

class IEntity
{
public:
    virtual ~IEntity() = default;
    virtual int getId() const = 0;
};
