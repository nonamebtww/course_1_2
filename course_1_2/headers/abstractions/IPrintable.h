#pragma once

#include <iosfwd>

class IPrintable
{
public:
    virtual ~IPrintable() = default;
    virtual void print(std::ostream& output) const = 0;
};
