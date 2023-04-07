#include <fstream>
#include <sstream>
#include <iostream>

#include "../header/GestaoR.h"
#include "../header/Menu.h"

GestaoR::GestaoR() {
    railwayNetwork = Graph();
}

/**
 * Lê o ficheiro stations.csv e guarda todas as companhias aéreas.
 * Complexidade Temporal O(n) (n é o tamanho do ficheiro retirando o cabeçalho).
 */
void GestaoR::readStations() {
    railwayNetwork.setVertexSet({});

    std::vector<std::pair<std::string, std::vector<std::string>>> municipalities;
    std::vector<std::pair<std::string, std::vector<std::string>>> districts;
    std::pair<std::string, std::vector<std::string>> temp;
    std::string ficheiro = "../resources/stations.csv";
    std::ifstream ifs1;
    ifs1.open(ficheiro, std::ios::in);
    std::string segment;
    getline(ifs1, segment);
    int id = 0;

    while (getline(ifs1, segment)) {
        std::string name, district, municipality, township, line, help;
        std::istringstream stream(segment);

        getline(stream, name, ',');
        getline(stream, district, ',');
        getline(stream, municipality, ',');
        if (std::find(segment.begin(), segment.end(), '"') != segment.end()) {
            getline(stream, help, '"');
            getline(stream, township, '"');
            getline(stream, help, ',');
        }
        else getline(stream, township, ',');
        getline(stream, line, '\r');
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        std::transform(district.begin(), district.end(), district.begin(), ::toupper);
        std::transform(municipality.begin(), municipality.end(), municipality.begin(), ::toupper);
        std::transform(township.begin(), township.end(), township.begin(), ::toupper);
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);

        if (name.length() > getMaxStationLength()) setMaxStationLength((int)name.length());
        if (municipality.length() > getMaxMunicipalityLength()) setMaxMunicipalityLength((int)name.length());
        if (district.length() > getMaxDistrictLength()) setMaxDistrictLength((int)name.length());
        if (line.length() > getMaxLineLength()) setMaxLineLength((int)line.length());

        if (!district.empty() && !municipality.empty()) {
            bool flagM = false;
            for (auto &par : municipalities) {
                if (par.first == municipality) {
                    par.second.push_back(name);
                    flagM = true;
                    break;
                }
            }
            if (!flagM) {
                temp.first = municipality;
                temp.second.push_back(name);
                municipalities.push_back(temp);
            }
            temp.second.clear();
            bool flagD = false;
            for (auto &par : districts) {
                if (par.first == district) {
                    par.second.push_back(name);
                    flagD = true;
                    break;
                }
            }
            if (!flagD) {
                temp.first = district;
                temp.second.push_back(name);
                districts.push_back(temp);
            }
            temp.second.clear();
        }

        railwayNetwork.addVertex(id, name, district, municipality, township, line);
        id++;
    }
    ifs1.close();

    railwayNetwork.setMunicipalities(municipalities);
    railwayNetwork.setDistricts(districts);
}

/**
 * Lê o ficheiro network.csv e vai colocando os aerportos no grafo, assim como guarda todas as cidades diferentes.
 * Complexidade Temporal O(n) (n é o tamanho do ficheiro retirando o cabeçalho).
 */
void GestaoR::readNetwork() {
    std::string ficheiro = "../resources/network.csv";
    std::ifstream ifs2;
    ifs2.open(ficheiro, std::ios::in);
    std::string segment;
    getline(ifs2, segment);

    while (getline(ifs2, segment)) {
        std::string station_A, station_B, capacityS, service;
        double capacity, cost;
        std::istringstream stream(segment);
        getline(stream, station_A, ',');
        getline(stream, station_B, ',');
        getline(stream, capacityS, ',');
        getline(stream, service, '\r');
        capacity = stod(capacityS);
        std::transform(station_A.begin(), station_A.end(), station_A.begin(), ::toupper);
        std::transform(station_B.begin(), station_B.end(), station_B.begin(), ::toupper);
        std::transform(service.begin(), service.end(), service.begin(), ::toupper);
        int idStation_A = railwayNetwork.findVertexName(station_A);
        int idStation_B = railwayNetwork.findVertexName(station_B);
        if (service == "STANDARD") cost = capacity * 2;
        else cost = capacity * 4;
        railwayNetwork.addBidirectionalEdge(idStation_A, idStation_B, capacity, service, cost);
    }
    ifs2.close();
}

std::vector<Vertex *> GestaoR::getVertexSet() const { return railwayNetwork.getVertexSet(); }

double GestaoR::edmondsKarp(const std::string &source, const std::string &target) {
    int sourceId = railwayNetwork.findVertexName(source);
    int targetId = railwayNetwork.findVertexName(target);
    if (sourceId == -1) {
        Menu::estacaoNaoEncontrada();
        return -1.0;
    }
    else if (sourceId == -1 || targetId == -1) {
        Menu::estacaoNaoExiste();
        return -1.0;
    }
    else if (sourceId == targetId) {
        Menu::estacoesIguais();
        return -1.0;
    }
    railwayNetwork.edmondsKarp(sourceId, targetId);

    double flow = 0.0;
    for (Edge *edge : railwayNetwork.getVertexSet().at(targetId)->getIncoming()) {
        flow += edge->getFlow();
    }
    return flow;
}

std::pair<std::string, std::string> GestaoR::maxEdmondsKarp() {
    double flow = 0.0, tempFlow;
    std::pair<std::string, std::string> result;
    for (int i = 0; i < getVertexSet().size()-1; i++) {
        for (int j = i+1; j < getVertexSet().size(); j++) {
            tempFlow = edmondsKarp(getVertexSet().at(i)->getName(), getVertexSet().at(j)->getName());
            if (tempFlow > flow) {
                flow = tempFlow;
                result.first = getVertexSet().at(i)->getName();
                result.second = getVertexSet().at(j)->getName();
            }
        }
    }
    return result;
}

double GestaoR::maxEKtopK(const std::vector<std::string> &names) {
    double flow = 0.0;
    for (int i = 0; i < names.size()-1; i++) {
        for (int j = i+1; j < names.size(); j++) {
            flow += edmondsKarp(names.at(i), names.at(j));
        }
    }
    return flow;
}

void GestaoR::topK(bool mORd, int k) {
    std::vector<std::pair<std::string, double>> order;
    std::vector<std::pair<std::string, std::vector<std::string>>> muniORdist;
    if (mORd) muniORdist = railwayNetwork.getMunicipalities();
    else muniORdist = railwayNetwork.getDistricts();
    for (auto &i : muniORdist) {
        order.emplace_back(i.first, maxEKtopK(i.second));
    }
    std::sort(order.begin(), order.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });
    drawTopKs(order, mORd, k);
}

double GestaoR::maxFlowOrigin(const std::string &origin) {
    int id = (int)railwayNetwork.getVertexSet().size();
    double flow;
    std::string name = "tempVertex";
    railwayNetwork.addVertex(id, name, "", "", "", "");
    for (Vertex *vertex : railwayNetwork.getVertexSet()) {
        if (vertex->getAdj().size() <= 1 && vertex->getName() != origin) {
            railwayNetwork.addBidirectionalEdge(id, vertex->getId(), INF, "", 0.0);
        }
    }
    flow = edmondsKarp(origin, name);
    railwayNetwork.removeVertex(id);
    return flow;
}

double GestaoR::dijkstraShortestPathCost(const std::string &source, const std::string &target, std::vector<std::string> &path, double &minWeigth) {
    int sourceId = railwayNetwork.findVertexName(source);
    int targetId = railwayNetwork.findVertexName(target);
    if (sourceId == -1) {
        Menu::estacaoNaoEncontrada();
        return -1.0;
    }
    else if (sourceId == -1 || targetId == -1) {
        Menu::estacaoNaoExiste();
        return -1.0;
    }
    else if (sourceId == targetId) {
        Menu::estacoesIguais();
        return -1.0;
    }
    railwayNetwork.dijkstraShortestPath(sourceId);
    Vertex *targetVertex = railwayNetwork.findVertexId(railwayNetwork.findVertexName(target));
    Vertex *temp = targetVertex;
    path.push_back(temp->getName());
    // min-weight cycle
    while (temp->getPath() != nullptr) {
        path.push_back(temp->getPath()->getOrig()->getName());
        if (temp->getPath()->getWeight() < minWeigth)
            minWeigth = temp->getPath()->getWeight();
        temp = temp->getPath()->getOrig();
    }
    // cost cycle
    double cost = 0.0;
    while (targetVertex->getPath() != nullptr) {
        if (targetVertex->getPath()->getService() == "STANDARD")
            cost += (minWeigth * 2);
        else cost += (minWeigth * 4);
        targetVertex = targetVertex->getPath()->getOrig();
    }
    return cost;
}

double GestaoR::existStations(const std::string &source, const std::string &target) {
    int sourceId = railwayNetwork.findVertexName(source);
    int targetId = railwayNetwork.findVertexName(target);
    if (sourceId == -1) {
        Menu::estacaoNaoEncontrada();
        return -1.0;
    }
    else if (sourceId == -1 || targetId == -1) {
        Menu::estacaoNaoExiste();
        return -1.0;
    }
    else if (sourceId == targetId) {
        Menu::estacoesIguais();
        return -1.0;
    }
    return existConnection(sourceId, targetId);
}

double GestaoR::existConnection(const int &sourceId, const int &targetId) const {
    for (Edge *edge : getVertexSet().at(sourceId)->getAdj()) {
        if (edge->getDest()->getId() == targetId) {
            return edge->getWeight();
        }
    }
    return 0.0;
}

void GestaoR::breakEdge(const std::string &source, const std::string &target) {
    int sourceId = railwayNetwork.findVertexName(source);
    int targetId = railwayNetwork.findVertexName(target);
    for (Edge *edge: railwayNetwork.getVertexSet().at(sourceId)->getAdj()) {
        if (edge->getDest()->getName() == target) {
            edge->setWeight(0.0);
            break;
        }
    }
    for (Edge *edge: railwayNetwork.getVertexSet().at(targetId)->getAdj()) {
        if (edge->getDest()->getName() == source) {
            edge->setWeight(0.0);
            break;
        }
    }
    std::cout << "\nEdge " << source << " - " << target << " broken." << std::endl;
}

void GestaoR::decreaseEdge(const std::string &source, const std::string &target, double capacity) {
    int sourceId = railwayNetwork.findVertexName(source);
    int targetId = railwayNetwork.findVertexName(target);
    for (Edge *edge: railwayNetwork.getVertexSet().at(sourceId)->getAdj()) {
        if (edge->getDest()->getName() == target) {
            edge->setWeight(capacity);
            break;
        }
    }
    for (Edge *edge: railwayNetwork.getVertexSet().at(targetId)->getAdj()) {
        if (edge->getDest()->getName() == target) {
            edge->setWeight(capacity);
            break;
        }
    }
    std::cout << "\nEdge " << source << " - " << target << " decreased." << std::endl;
}

/**
 * Função para ajudar a centralizar textos.
 * Complexidade Temporal O(1).
 * @param n
 * @param v
 * @return par com o número de espaços e se a palavra é par ou ímpar.
 */
std::pair<int, int> GestaoR::auxCenterDraw(int n, bool v) {
    int pad1 = n;
    int pad2;
    if (!v) pad2 = pad1 + 1;
    else pad2 = pad1;
    return std::pair<int, int>{pad1, pad2};
}

/**
 * Desenho do Menu principal.
 * Complexidade Temporal O(1).
 */
void GestaoR::drawMenu() {
    std::cout << "\n+-------------------------------------------------------------+\n"
                 "|                     RAILWAY MANAGEMENT                      |\n"
                 "+-------------------------------------------------------------+\n"
                 "| [1] - Complete Listings                                     |\n"
                 "| [2] - 2.1 (max-flow)                                        |\n"
                 "| [3] - 2.2 (max-flow pair)                                   |\n"
                 "| [4] - 2.3 (top-k)                                           |\n"
                 "| [5] - 2.4 (max-trains-entire-network)                       |\n"
                 "| [6] - 3.1 (cost)                                            |\n"
                 "| [7] - 4.1 (failures)                                        |\n"
                 "| [8] - 4.2 (report affected stations)                        |\n"
                 "| [R] - Reset Graph                                           |\n"
                 "| [Q] - Exit the application                                  |\n"
                 "+-------------------------------------------------------------+\n";
    std::cout << "\nChoose the option and press ENTER:";
}

/**
 * Desenha um menu secundário para mais opções.
 * Complexidade Temporal O(1).
 */
void GestaoR::drawListagemMenu() {
    std::cout << "\n+-------------------------------------------------------------+\n"
                 "|                     RAILWAY MANAGEMENT                      |\n"
                 "+-------------------------------------------------------------+\n"
                 "| [1] - List stations                                         |\n"
                 "| [2] - List connections                                      |\n"
                 "| [3] - List station connections                              |\n"
                 "| [V] - Go Back                                               |\n"
                 "+-------------------------------------------------------------+\n";
    std::cout << "\nChoose the option and press ENTER:";
}

/**
 * Desenha um menu secundário para mais opções.
 * Complexidade Temporal O(1).
 */
void GestaoR::drawBudgetMenu() {
    std::cout << "\n+-------------------------------------------------------------+\n"
                 "|                     RAILWAY MANAGEMENT                      |\n"
                 "+-------------------------------------------------------------+\n"
                 "| [1] - Municipalities                                        |\n"
                 "| [2] - Districts                                             |\n"
                 "| [V] - Go Back                                                |\n"
                 "+-------------------------------------------------------------+\n";
    std::cout << "\nChoose the option and press ENTER:";
}

void GestaoR::drawTopK(const std::string &order, bool header, int &pos, bool mORd) const {
    int maxLength = mORd ? getMaxMunicipalityLength() : getMaxDistrictLength();
    if (maxLength % 2 == 0) maxLength++;
    if (mORd) {
        if (header) {
            std::cout << "\n+------+--------------------------+\n"
                         "| TOP  |       MUNICIPALITY       |\n"
                         "+------+--------------------------+\n";
        }
    }
    else {
        if (header) {
            std::cout << "\n+------+------------------+\n"
                         "| TOP  |     DISTRICT     |\n"
                         "+------+------------------+\n";
        }
    }
    std::cout << "|";
    std::pair<int, int> pad = auxCenterDraw(5 - (int) std::to_string(pos).length(),
                                            (int) std::to_string(pos).length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << pos;
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|";
    std::pair<int, int> padCountry = auxCenterDraw(maxLength - (int) order.length(),
                                                   (int) order.length() % 2 == 0);
    for (int f = 0; f < padCountry.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << order;
    for (int e = 0; e < padCountry.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|" << "\n";
}

void GestaoR::drawTopKs(const std::vector<std::pair<std::string, double>> &order, bool mORd, int k) const {
    bool header = true;
    int i = 1;
    for (int j = 0; j < k; j++) {
        drawTopK(order.at(j).first, header, i, mORd);
        header = false;
        i++;
    }
    if (mORd) std::cout << "+------+--------------------------+\n";
    else std::cout << "+------+------------------+\n";
}

void GestaoR::drawStation(const Vertex *vertex, bool header) const {
    if (header) {
        std::cout << "\n+------------------------------------+--------------------------+\n"
                     "|                NAME                |           LINE          |\n"
                     "+------------------------------------+--------------------------+\n";
    }
    std::cout << "|";
    std::pair<int, int> pad = auxCenterDraw(getMaxStationLength() - (int) vertex->getName().length(),
                                            (int) vertex->getName().length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << vertex->getName();
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|";
    std::pair<int, int> padCountry = auxCenterDraw(getMaxLineLength() - (int) vertex->getLine().length(),
                                                   (int) vertex->getLine().length() % 2 == 0);
    for (int f = 0; f < padCountry.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << vertex->getLine();
    for (int e = 0; e < padCountry.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|" << "\n";
}

void GestaoR::drawStations() const {
    bool header = true;
    for (const Vertex *vertex : getVertexSet()) {
        drawStation(vertex, header);
        header = false;
    }
    std::cout << "+------------------------------------+--------------------------+\n";
}

void GestaoR::drawNetwork() const {
    for (const Vertex *vertex : getVertexSet()) {
        std::cout << "Name: " << vertex->getName() << " " << vertex->getAdj().size() << std::endl;
        for (Edge *edge : vertex->getAdj()) {
            std::cout << "Destination: " << edge->getDest()->getName() << " Capacity: " << edge->getWeight() << " Service: " << edge->getService() << std::endl;
        }
    }
}

void GestaoR::drawStationNetwork(const std::string &name) {
    Vertex *vertex = railwayNetwork.findVertexId(railwayNetwork.findVertexName(name));
    if (vertex == nullptr) return;
    for (Edge *edge : vertex->getAdj()) {
        std::cout << "Destination: " << edge->getDest()->getName() << " Capacity: " << edge->getWeight() << std::endl;
    }
}

void GestaoR::setMaxStationLength(int maxLength) { maxStationLength = maxLength; }

void GestaoR::setMaxMunicipalityLength(int maxLength) { maxMunicipalityLength = maxLength; }

void GestaoR::setMaxDistrictLength(int maxLength) { maxDistrictLength = maxLength; }

void GestaoR::setMaxLineLength(int maxLength) { maxLineLength = maxLength; }

int GestaoR::getMaxStationLength() const { return maxStationLength; }

int GestaoR::getMaxMunicipalityLength() const { return maxMunicipalityLength; }

int GestaoR::getMaxDistrictLength() const { return maxDistrictLength; }

int GestaoR::getMaxLineLength() const { return maxLineLength; }

int GestaoR::getMSize() const { return railwayNetwork.getMSize(); }

int GestaoR::getDSize() const { return railwayNetwork.getDSize(); }
