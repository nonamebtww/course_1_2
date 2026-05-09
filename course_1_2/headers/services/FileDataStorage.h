#pragma once

#include "abstractions/Interfaces.h"

#include <filesystem>
#include <string>

class FileDataStorage final : public IDataStorage
{
public:
    explicit FileDataStorage(std::string directory);

    void load(TaxiService& service) override;
    void save(const TaxiService& service) const override;

private:
    std::filesystem::path pathFor(const std::string& fileName) const;

    std::filesystem::path directory;
};
