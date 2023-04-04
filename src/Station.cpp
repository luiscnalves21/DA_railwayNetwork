#include "../header/Station.h"

Station::Station(const std::string &name, const std::string &district, const std::string &municipality,
            const std::string &township, const std::string &line) {
    name_ = name;
    district_ = district;
    municipality_ = municipality;
    township_ = township;
    line_ = line;
}

std::string Station::getName() const { return name_; }

std::string Station::getDistrict() const { return district_; }

std::string Station::getMunicipality() const { return municipality_; }

std::string Station::getTownship() const { return township_; }

std::string Station::getLine() const { return line_; }

// TODO OPERATORS

bool Station::operator==(const Station &t) const { return false; }

bool Station::operator<(const Station &t) const { return false; }
