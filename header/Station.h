#ifndef STATION_H
#define STATION_H

#include <string>

class Station {
    public:
        Station(const std::string &name, const std::string &district, const std::string &municipality, const std::string &township, const std::string &line);

        std::string getName() const;
        std::string getDistrict() const;
        std::string getMunicipality() const;
        std::string getTownship() const;
        std::string getLine() const;

        bool operator==(const Station &t) const;
        bool operator<(const Station& t) const;
    private:
        std::string name_;
        std::string district_;
        std::string municipality_;
        std::string township_;
        std::string line_;
};

#endif
