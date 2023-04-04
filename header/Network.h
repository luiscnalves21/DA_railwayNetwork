#ifndef NETWORK_H
#define NETWORK_H

#include <string>

class Network {
    public:
        Network(const std::string &station_A, const std::string &station_B, const int &capacity, const std::string &service);

        std::string getStation_A() const;
        std::string getStation_B() const;
        int getCapacity() const;
        std::string getService() const;

        bool operator==(const Network &t) const;
        bool operator<(const Network& t) const;
    private:
        std::string station_A_;
        std::string station_B_;
        int capacity_;
        std::string service_;
};

#endif
