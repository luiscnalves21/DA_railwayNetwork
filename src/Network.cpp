#include "../header/Network.h"

Network::Network(const std::string &station_A, const std::string &station_B, const int &capacity,
                 const std::string &service) {
    station_A_ = station_A;
    station_B_ = station_B;
    capacity_ = capacity;
    service_ = service;
}

std::string Network::getStation_A() const { return station_A_; }

std::string Network::getStation_B() const { return station_B_; }

int Network::getCapacity() const { return capacity_; }

std::string Network::getService() const { return service_; }

// TODO OPERATORS

bool Network::operator==(const Network &t) const { return false; }

bool Network::operator<(const Network &t) const { return false; }
