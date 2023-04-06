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

        if (district != "" && municipality != "") {
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
        double capacity;
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
        railwayNetwork.addBidirectionalEdge(idStation_A, idStation_B, capacity, service);
    }
    ifs2.close();
}

std::vector<Vertex *> GestaoR::getVertexSet() const { return railwayNetwork.getVertexSet(); }

double GestaoR::edmondsKarp(const std::string &source, const std::string &target) {
    int sourceId = railwayNetwork.findVertexName(source);
    int targetId = railwayNetwork.findVertexName(target);
    if (sourceId == -1 || targetId == -1) {
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

void GestaoR::combineFA(std::vector<std::pair<std::string, std::string>> &edgesNames, std::vector<Vertex *> &vertexs) {
    for (int i = 0; i < vertexs.size(); i++) {
        for (int j = 0; j < vertexs.size(); j++) {
            if (i != j) {
                bool flag = false;
                for (const std::pair<std::string, std::string> edgeName : edgesNames) {
                    if (vertexs.at(i)->getName() == edgeName.first && vertexs.at(j)->getName() == edgeName.second || vertexs.at(i)->getName() == edgeName.second && vertexs.at(j)->getName() == edgeName.first) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    edgesNames.emplace_back(vertexs.at(i)->getName(), vertexs.at(j)->getName());
                }
            }
        }
    }
}

void GestaoR::dfsFA(std::vector<Edge *> &maxEdges, Vertex *vertex, double max, std::vector<Vertex *> &vertexs) {
    vertex->setVisited(true);
    for (Edge *adj : vertex->getAdj()) {
        if (!adj->getDest()->isVisited() && adj->getWeight() == max) {
            vertexs.push_back(adj->getDest());
            dfsFA(maxEdges, adj->getDest(), max, vertexs);
        }
    }
}

void GestaoR::fullAdvantage() {
    double max = 0.0;
    std::vector<Edge *> maxEdges;
    for (Vertex *vertex : railwayNetwork.getVertexSet()) {
        for (Edge *edge : vertex->getAdj()) {
            if (edge->getWeight() > max) {
                max = edge->getWeight();
                maxEdges.clear();
                maxEdges.push_back(edge);
            }
            else if (edge->getWeight() == max) {
                bool flag = false;
                for (Edge *e : maxEdges) {
                    if (e->getOrig() == edge->getOrig() && e->getDest() == edge->getDest() || e->getOrig() == edge->getDest() && e->getDest() == edge->getOrig()) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    maxEdges.push_back(edge);
                }
            }
        }
    }

    for (Vertex *vertex : railwayNetwork.getVertexSet()) vertex->setVisited(false);

    std::vector<std::pair<std::string, std::string>> edgesNames;
    std::pair<std::string, std::string> edgeName;
    for (Edge *e : maxEdges) {
        edgeName.first = e->getOrig()->getName();
        edgeName.second = e->getDest()->getName();
        edgesNames.push_back(edgeName);
    }
    std::vector<Vertex *> vertexs;
    for (Edge *edge : maxEdges) {
        if (!edge->getOrig()->isVisited()) {
            vertexs.push_back(edge->getOrig());

            dfsFA(maxEdges, edge->getOrig(), max, vertexs);

            combineFA(edgesNames, vertexs);
        }
        vertexs.clear();
    }
    std::sort(edgesNames.begin(), edgesNames.end(), [](std::pair<std::string, std::string> a, std::pair<std::string, std::string> b){
        return a.first < b.first;
    });
    drawFullAdvantages(edgesNames);
    std::cout << "\nExistem " << edgesNames.size() << " pares de estacoes com o maximo de comboios a " << max << "." << std::endl;
}

void GestaoR::dfsMR(Vertex *vertex, Municipality &municipality) {
    vertex->setVisited(true);
    municipality.numberOfStations++;
    for (Edge *edge : vertex->getAdj()) {
        // TODO VER ISTO
        if (!edge->getDest()->isVisited() || edge->getDest()->isVisited() && edge->getDest()->getMunicipality() != municipality.municipality) {
            if (edge->getDest()->getMunicipality() == municipality.municipality) {
                municipality.numberOfTrains += edge->getWeight();
                dfsMR(edge->getDest(), municipality);
            }
            else {
                municipality.numberOfTrains += (edge->getWeight()/2);
            }
        }
        else {
           // municipality.numberOfTrains += (edge->getWeight()/2);
        }
    }
}

std::vector<GestaoR::Municipality> GestaoR::managementRailway() {
    std::vector<Municipality> municipalities;
    Municipality municipality;
    for (Vertex *vertex : railwayNetwork.getVertexSet()) vertex->setVisited(false);
    for (Vertex *vertex : railwayNetwork.getVertexSet()) {
        if (!vertex->isVisited() && !vertex->getMunicipality().empty()) {
            municipality.municipality = vertex->getMunicipality();
            dfsMR(vertex, municipality);
            municipalities.push_back(municipality);
            municipality.numberOfTrains = 0.0;
            municipality.numberOfStations = 0;
        }
    }
    std::sort(municipalities.begin(), municipalities.end(), [](const Municipality &a, const Municipality &b) {
        return a.municipality < b.municipality;
    });
    std::vector<Municipality> orderMunicipalities;
    bool flag = true;
    int currIndex = -1;
    for (const Municipality &municipality1 : municipalities) {
        if (currIndex != -1) {
            if (orderMunicipalities.at(currIndex).municipality != municipality1.municipality) {
                flag = true;
            }
        }
        if (flag) {
            orderMunicipalities.push_back(municipality1);
            flag = false;
            currIndex++;
        }
        else if (orderMunicipalities.at(currIndex).municipality == municipality1.municipality) {
            orderMunicipalities.at(currIndex).numberOfTrains += municipality1.numberOfTrains;
            orderMunicipalities.at(currIndex).numberOfStations += municipality1.numberOfStations;
        }
    }
    return orderMunicipalities;
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
            "|                       RAILWAY NETWORK                       |\n"
            "+-------------------------------------------------------------+\n"
            "| [1] - Listagens Completas                                   |\n"
            "| [2] - 2.1                                                   |\n"
            "| [3] - Pares de estacoes com a maior quantidade de comboios  |\n"
            "| [4] - Top K concelhos que necessitam de maior orcamento     |\n"
            "| [5] - 2.2                                                   |\n"
            "| [6] - 2.3                                                   |\n"
            "| [Q] - Sair da aplicacao                                     |\n"
            "+-------------------------------------------------------------+\n";
    std::cout << "\nEscolha a opcao e pressione ENTER:";
}

/**
 * Desenha um menu secundário para mais opções.
 * Complexidade Temporal O(1).
 */
void GestaoR::drawListagemMenu() {
    std::cout << "\n+-----------------------------------------------------+\n"
                 "|                   RAILWAY NETWORK                   |\n"
                 "+-----------------------------------------------------+\n"
                 "| [1] - Listar Estacoes                               |\n"
                 "| [2] - Listar Ligacoes                               |\n"
                 "| [3] - Listar Ligacoes de uma estacao                |\n"
                 "| [V] - Voltar                                        |\n"
                 "+-----------------------------------------------------+\n";
    std::cout << "\nEscolha a opcao e pressione ENTER:";
}

/**
 * Desenha um menu secundário para mais opções.
 * Complexidade Temporal O(1).
 */
void GestaoR::drawBudgetMenu() {
    std::cout << "\n+-----------------------------------------------------+\n"
                 "|                   RAILWAY NETWORK                   |\n"
                 "+-----------------------------------------------------+\n"
                 "| [1] - Concelhos                                     |\n"
                 "| [2] - Distritos                                     |\n"
                 "| [V] - Voltar                                        |\n"
                 "+-----------------------------------------------------+\n";
    std::cout << "\nEscolha a opcao e pressione ENTER:";
}

void GestaoR::drawTopK(const std::string &order, bool header, int &pos, bool mORd) const {
    int maxLength = mORd ? getMaxMunicipalityLength() : getMaxDistrictLength();
    if (maxLength % 2 == 0) maxLength++;
    if (mORd) {
        if (header) {
            std::cout << "\n+------+--------------------------+\n"
                         "| TOP  |         CONCELHO         |\n"
                         "+------+--------------------------+\n";
        }
    }
    else {
        if (header) {
            std::cout << "\n+------+------------------+\n"
                         "| TOP  |     DISTRITO     |\n"
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
                     "|          NOME DA ESTACAO           |           LINHA          |\n"
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
        std::cout << "Destino: " << edge->getDest()->getName() << " Capacidade: " << edge->getWeight() << std::endl;
    }
}

void GestaoR::drawFullAdvantage(const std::pair<std::string, std::string> &edgeName, bool header) const {
    if (header) {
        std::cout << "\n+------------------------------------+------------------------------------+\n"
                "|             ESTACAO 1              |              ESTACAO 2             |\n"
                "+------------------------------------+------------------------------------+\n";
    }
    std::cout << "|";
    std::pair<int, int> pad = auxCenterDraw(getMaxStationLength() - (int) edgeName.first.length(),
                                       (int) edgeName.first.length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << edgeName.first;
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|";
    std::pair<int, int> padCountry = auxCenterDraw(getMaxStationLength() - (int) edgeName.second.length(),
                                              (int) edgeName.second.length() % 2 == 0);
    for (int f = 0; f < padCountry.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << edgeName.second;
    for (int e = 0; e < padCountry.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|" << "\n";
}

void GestaoR::drawFullAdvantages(const std::vector<std::pair<std::string, std::string>> &edgesNames) const {
    bool header = true;
    for (const std::pair<std::string, std::string>& edgeName : edgesNames) {
        drawFullAdvantage(edgeName, header);
        header = false;
    }
    std::cout << "+------------------------------------+------------------------------------+\n";
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

/**
 * Desenha uma cidade (parâmetro cc) e identifica se é a 1ª cidade a ser escrita para desenhar o cabeçalho da tabela (parâmetro header).
 * Complexidade Temporal O(n).
 * @param cc
 * @param header
 */
 /*
void GestaoR::drawCity(const CityCountry &cc, bool header) const {
    if (header) {
        cout << "\n+-------------------------------+---------------------------------+\n"
                "|             CITY              |             COUNTRY             |\n"
                "+-------------------------------+---------------------------------+\n";
    }
    cout << "|";
    pair<int, int> pad = auxCenterDraw(getMaxCityLength() - (int) cc.getCity().length(),
                                       (int) cc.getCity().length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << cc.getCity();
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|";
    pair<int, int> padCountry = auxCenterDraw(getMaxCountryLength() - (int) cc.getCountry().length(),
                                              (int) cc.getCountry().length() % 2 == 0);
    for (int f = 0; f < padCountry.first; f++) {
        cout << " ";
        ++f;
    }
    cout << cc.getCountry();
    for (int e = 0; e < padCountry.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|" << "\n";
}
*/
/**
 * Desenha todas as cidades e chama a função drawCity para desenhar uma de cada vez.
 * Complexidade Temporal O(n^2).
 * @param citiesaux
 */
 /*
void GestaoR::drawCities(const vector<CityCountry> &citiesaux) const {
    bool v = true;
    for (const CityCountry &s: citiesaux) {
        drawCity(s, v);
        v = false;
    }
    cout << "+-------------------------------+---------------------------------+\n";
}
*/
/**
 * Desenha uma aeroporto e identifica se é o 1º aeroporto a ser escrito para desenhar o cabeçalho da tabela (parâmetro header).
 * Complexidade Temporal O(n).
 * @param code
 * @param header
 */
 /*
void GestaoR::drawAirport(const string &code, bool header) {
    if (header) {
        cout << "\n+-----+--------------------------------------------------------+---------------------------------+\n"
                "| COD |                          NAME                          |             COUNTRY             |\n"
                "+-----+--------------------------------------------------------+---------------------------------+\n";
    }
    cout << "| " << code << " |";
    pair<int, int> pad = auxCenterDraw(getMaxAirportNameLength() - (int) flightNetwork_.getAirportName(code).length(),
                                       (int) flightNetwork_.getAirportName(code).length() % 2 == 1);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << flightNetwork_.getAirportName(code);
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|";
    pair<int, int> padCountry = auxCenterDraw(
            getMaxCountryLength() - (int) flightNetwork_.getAirportCountry(code).length(),
            (int) flightNetwork_.getAirportCountry(code).length() % 2 == 0);
    for (int f = 0; f < padCountry.first; f++) {
        cout << " ";
        ++f;
    }
    cout << flightNetwork_.getAirportCountry(code);
    for (int e = 0; e < padCountry.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|" << "\n";
}
*/
/**
 * Desenha todos os aeroportos e chama a função drawAirport para desenhar uma de cada vez.
 * Complexidade Temporal O(n^2).
 * @param airports
 */
 /*
void GestaoR::drawAirports(const vector<string> &airports) {
    bool v = true;
    for (int i = 1; i < airports.size(); i++) {
        drawAirport(airports.at(i), v);
        v = false;
    }
    cout << "+-----+--------------------------------------------------------+---------------------------------+\n";
}*/
/**
 * Desenha uma companhia aérea e identifica se é a 1ª companhia a ser escrito para desenhar o cabeçalho da tabela (parâmetro header).
 * Complexidade Temporal O(n).
 * @param a
 * @param header
 */
 /*
void GestaoR::drawAirline(const Airline &a, bool header) const {
    if (header) {
        cout << "\n+-----+------------------------------------------+--------------------------------------+\n"
                "| COD |                  NAME                    |                COUNTRY               |\n"
                "+-----+------------------------------------------+--------------------------------------+\n";
    }
    cout << "| " << a.getCode() << " |";
    pair<int, int> pad = auxCenterDraw(getMaxAirlineNameLength() - (int) a.getName().length(),
                                       (int) a.getName().length() % 2 == 1);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << a.getName();
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|";
    pair<int, int> padCountry = auxCenterDraw(getMaxAirlineCountryLength() - (int) a.getCountry().length(),
                                              (int) a.getCountry().length() % 2 == 1);
    for (int f = 0; f < padCountry.first; f++) {
        cout << " ";
        ++f;
    }
    cout << a.getCountry();
    for (int e = 0; e < padCountry.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|" << "\n";
}
*/
/**
 * Desenha todas as companhias aéreas e chama a função drawAirline para desenhar uma de cada vez.
 * Complexidade Temporal O(n^2).
 * @param airlinesaux
 */
 /*
void GestaoR::drawAirlines(const vector<Airline> &airlinesaux) const {
    bool v = true;
    for (const Airline &s: airlinesaux) {
        drawAirline(s, v);
        v = false;
    }
    cout << "+-----+------------------------------------------+--------------------------------------+\n";
}
*/
/**
 * Desenha um voo, representado pelo nome do aeroporto de destino (parâmetro name) e nome da companhia aérea (parâmetro airline), desenhando o cabeçalho para o 1º desenho (parâmetro header)
 * Complexidade Temporal O(n)
 * @param name
 * @param airline
 * @param header
 */
 /*
void GestaoR::drawFlight(const string &name, const string &airline, bool header) const {
    if (header) {
        cout
                << "\n+--------------------------------------------------------+------------------------------------------+\n"
                   "|                   AIRPORT DESTINATION                  |                  AIRLINE                 |\n"
                   "+--------------------------------------------------------+------------------------------------------+\n";
    }
    cout << "|";
    pair<int, int> pad = auxCenterDraw(getMaxAirportNameLength() - (int) name.length(), (int) name.length() % 2 == 1);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << name;
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|";
    pad = auxCenterDraw(getMaxAirlineNameLength() - (int) airline.length(), (int) airline.length() % 2 == 1);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << airline;
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|\n";
}*/
/**
 * Vai buscar os voos de um aeroporto (parâmetro code) e chama a função drawFlight para desenhar um a um.
 * Complexidade Temporal O(n^2).
 * @param code
 *//*
void GestaoR::drawFlights(const string &code) {
    bool v = true;
    list<pair<int, string>> airportFlights = flightNetwork_.getAirportFlights(code);
    for (const auto &flight: airportFlights) {
        string cd = flightNetwork_.getAirportName(flight.first);
        string airlinename;
        for (const auto &airline: airlines) if (airline.getCode() == flight.second) airlinename = airline.getName();
        drawFlight(cd, airlinename, v);
        v = false;
    }
    cout << "+--------------------------------------------------------+------------------------------------------+\n";
}
*/
/**
 * Conta as companhias aéreas que têm voos num aeroporto (parâmetro code) e desenha no terminal.
 * Complexidade Temporal O(n).
 * @param code
 *//*
void GestaoR::drawNumberOfAirlines(const string &code) {
    set<string> temp;
    list<pair<int, string>> aux = flightNetwork_.getAirportFlights(code);
    int size;
    string helpme;

    for (const auto &i: aux) temp.insert(i.second);
    size = (int) temp.size();
    if (size < 10) helpme = "0" + to_string(size);
    else helpme = to_string(size);

    cout << "\n+---------+----------+\n"
            "| AIRPORT | AIRLINES |\n"
            "+---------+----------+\n";

    cout << "|   " << code << "   |";
    pair<int, int> pad = auxCenterDraw(9 - (int) helpme.length(), helpme.length() % 2 == 1);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << helpme;
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|\n";
    cout << "+---------+----------+\n";
}*/
/**
 * Conta os destinos/aeroportos a que se pode chegar a partir de um aeroporto (parâmetro code) e desenha no terminal.
 * Complexidade Temporal O(n).
 * @param code
 */
 /*
void GestaoR::drawNumberOfTargets(const string &code) {
    set<string> temp;
    list<pair<int, string>> aux = flightNetwork_.getAirportFlights(code);
    int size;
    string helpme;

    for (const auto &i: aux) temp.insert(flightNetwork_.getAirportCode(i.first));

    size = (int) temp.size();
    if (size < 10) helpme = "00" + to_string(size);
    else if (size >= 10 && size < 100) helpme = "0" + to_string(size);
    else helpme = to_string(size);

    cout << "\n+---------+-------------+\n"
            "| AIRPORT | DESTINATION |\n"
            "+---------+-------------+\n";
    cout << "|   " << code << "   |";
    pair<int, int> pad = auxCenterDraw(12 - (int) helpme.length(), helpme.length() % 2 == 1);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << helpme;
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|\n";
    cout << "+---------+-------------+\n";
}*/
/**
 * Conta os países a que se pode chegar a partir de um aeroporto (parâmetro code) e desenha no terminal.
 * Complexidade Temporal O(n).
 * @param code
 */
 /*
void GestaoR::drawNumberOfCountries(const string &code) {
    set<string> temp;
    list<pair<int, string>> aux = flightNetwork_.getAirportFlights(code);
    int size;
    string helpme;

    for (const auto &i: aux) temp.insert(flightNetwork_.getAirportCountry(i.first));

    size = (int) temp.size();
    if (size < 10) helpme = "0" + to_string(size);
    else helpme = to_string(size);

    cout << "\n+---------+-----------+\n"
            "| AIRPORT | COUNTRIES |\n"
            "+---------+-----------+\n";
    cout << "|   " << code << "   |";
    pair<int, int> pad = auxCenterDraw(10 - (int) helpme.length(), helpme.length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << helpme;
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|\n";
    cout << "+---------+-----------+\n";
}
*/
/**
 * Busca todos os aeroportos diferentes acessíveis a partir do inicial e dentro de y voos (usa BFS).
 * Complexidade Temporal O(n^2).
 * @param code
 * @param y
 * @return um set de strings com o código dos aeroportos que são acessíveis a partir do inicial.
 */
 /*
set<int> GestaoR::yAirports(const string &code, int y) {
    queue<int> search;
    set<int> aux;
    set<int> airportsReached;
    search.push(flightNetwork_.getAirportInfo(code));

    for (auto &help: flightNetwork_.airports) {
        help.visited = false;
        help.dist = -1;
    }

    flightNetwork_.airports[flightNetwork_.getAirportInfo(code)].dist = 0;
    flightNetwork_.airports[flightNetwork_.getAirportInfo(code)].visited = true;
    bool flag = false;

    while (!search.empty()) {
        int currkey = search.front();
        for (const auto &flight: flightNetwork_.airports[currkey].flights) {
            if (!flightNetwork_.airports[flight.dest].visited) {
                flightNetwork_.airports[flight.dest].visited = true;
                search.push(flight.dest);
                aux.insert(flight.dest);
                flightNetwork_.airports[flight.dest].dist = flightNetwork_.airports[currkey].dist + 1;
                if (flightNetwork_.airports[flight.dest].dist > y) flag = true;
            }
        }
        search.pop();
        if (flag) break;
    }


    for (auto k: aux) {
        if (k == flightNetwork_.getAirportInfo(code) || flightNetwork_.airports[k].dist > y) continue;
        else airportsReached.insert(k);
    }
    return airportsReached;
}
*/
/**
 * Busca todas as cidades diferentes acessíveis a partir de um aeroporto inicial e dentro de y voos (Chama a função Yairports, logo usa BFS).
 * Complexidade Temporal O(n^2).
 * @param code
 * @param y
 * @return um set da classe CityCountry de forma a verificar se cidades com o mesmo nome podem ser de países diferentes.
 *//*
set<CityCountry> GestaoR::yCities(const string &code, int y) {
    set<int> airportsReached = yAirports(code, y);
    set<CityCountry> citiesReached;

    for (auto i: airportsReached) {
        CityCountry cc(flightNetwork_.airports[i].city, flightNetwork_.airports[i].country);
        citiesReached.insert(cc);
    }

    return citiesReached;
}*/

/**
 * Busca todos os países diferentes acessíveis a partir de um aeroporto inicial e dentro de y voos (Chama a função Yairports, logo usa BFS).
 * Complexidade Temporal O(n^2).
 * @param code
 * @param y
 * @return um set de strings com os países acessíveis.
 *//*
set<string> GestaoR::yCountries(const string &code, int y) {
    set<int> airportsReached = yAirports(code, y);
    set<string> countriesReached;
    for (auto i: airportsReached) countriesReached.insert(flightNetwork_.airports[i].country);
    return countriesReached;
}
*/
/**
 * Desenha uma tabela com o código do aeroporto inicial e o número de destinos acessíveis a partir desse.
 * Complexidade Temporal O(n^2) (chama Yairports).
 * @param code
 * @param y
 *//*
void GestaoR::drawYAirports(const string &code, int y) {
    set<int> airportsReached = yAirports(code, y);

    int size = (int) airportsReached.size();
    string helpme;

    cout << "\n+---------+-------------+\n"
            "| AIRPORT | DESTINATION |\n"
            "+---------+-------------+\n";
    cout << "|   " << code << "   |";

    if (size < 10) helpme = "000" + to_string(size);
    else if (size >= 10 && size < 100) helpme = "00" + to_string(size);
    else if (size >= 100 && size < 1000)helpme = "0" + to_string(size);
    else helpme = to_string(size);

    pair<int, int> pad = auxCenterDraw(12 - (int) helpme.length(), helpme.length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << helpme;
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|\n";
    cout << "+---------+-------------+\n";
}*/

/**
 * Desenha uma tabela com o código do aeroporto inicial e o número de cidades acessíveis.
 * Complexidade Temporal O(n^2) (chama Ycities que por sua vez chama Yairports).
 * @param code
 * @param y
 *//*
void GestaoR::drawYCities(const string &code, int y) {
    set<CityCountry> citiesReached = yCities(code, y);

    int size = (int) citiesReached.size();
    string helpme;

    cout << "\n+---------+--------+\n"
            "| AIRPORT | CITIES |\n"
            "+---------+--------+\n";
    cout << "|   " << code << "   |";

    if (size < 10) helpme = "00" + to_string(size);
    else if (size >= 10 && size < 100) helpme = "0" + to_string(size);
    else helpme = to_string(size);

    pair<int, int> pad = auxCenterDraw(7 - (int) helpme.length(), helpme.length() % 2 == 1);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << helpme;
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|\n";
    cout << "+---------+--------+\n";
}*/

/**
 * Desenha uma tabela com o código do aeroporto inicial e o número de países acessíveis.
 * Complexidade Temporal O(n^2) (chama Ycountries que por sua vez chama Yairports).
 * @param code
 * @param y
 *//*
void GestaoR::drawYCountries(const string &code, int y) {
    set<string> countriesReached = yCountries(code, y);

    int size = (int) countriesReached.size();
    string helpme;

    cout << "\n+---------+-----------+\n"
            "| AIRPORT | COUNTRIES |\n"
            "+---------+-----------+\n";
    cout << "|   " << code << "   |";

    if (size < 10) helpme = "00" + to_string(size);
    else if (size >= 10 && size < 100) helpme = "0" + to_string(size);
    else helpme = to_string(size);

    pair<int, int> pad = auxCenterDraw(10 - (int) helpme.length(), helpme.length() % 2 == 1);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << helpme;
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|\n";
    cout << "+---------+-----------+\n";
}
*/
/**
 * Faz uma breadth-first search a partir do node v e atualiza a variável max para a maior distância possível.
 * Complexidade Temporal O(|V|+|E|).
 * @param v
 * @return a distância máxima a partir do node v.
 *//*
int GestaoR::bfsDiameter(int v) {
    int max = 0;
    for (auto &help: flightNetwork_.airports) {
        help.visited = false;
        help.dist = -1;
    }
    queue<int> q;
    q.push(v);
    flightNetwork_.airports[v].visited = true;
    flightNetwork_.airports[v].dist = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto &f: flightNetwork_.airports[u].flights) {
            int w = f.dest;
            if (!flightNetwork_.airports[w].visited) {
                q.push(w);
                flightNetwork_.airports[w].visited = true;
                flightNetwork_.airports[w].dist = flightNetwork_.airports[u].dist + 1;
                if (flightNetwork_.airports[w].dist > max) { max = flightNetwork_.airports[w].dist; }
            }
        }
    }
    return max;
}*/

/**
 * Faz um breadth-first search a partir do node v e guarda na variável max a maior distância possível para o mesmo país.
 * Complexidade Temporal O(|V| + |E|).
 * @param v
 * @param country
 * @return o diâmetro máximo a partir do node v.
 *//*
int GestaoR::bfsDiameterCountry(int v, string country) {
    int max = 0;
    for (auto &help: flightNetwork_.airports) {
        help.visited = false;
        help.dist = -1;
    }
    queue<int> q;
    q.push(v);
    flightNetwork_.airports[v].visited = true;
    flightNetwork_.airports[v].dist = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto &f: flightNetwork_.airports[u].flights) {
            int w = f.dest;
            if (!flightNetwork_.airports[w].visited) {
                q.push(w);
                flightNetwork_.airports[w].visited = true;
                flightNetwork_.airports[w].dist = flightNetwork_.airports[u].dist + 1;
                if (flightNetwork_.airports[w].dist > max &&
                    flightNetwork_.airports[w].country == country) { max = flightNetwork_.airports[w].dist; }
            }
        }
    }
    return max;
}
*/
/**
 * Desenha o diâmetro do grafo geral.
 * Complexidade Temporal O(|V|*(|V|+|E|)).
 *//*
void GestaoR::drawDiameter() {
    int max = 0;
    int count;
    for (int i = 1; i < flightNetwork_.airports.size(); i++) {
        count = bfsDiameter(i);
        if (count > max) { max = count; }
    }

    string helpme = to_string(max);

    cout << "\n+----------+\n"
            "| DIAMETER |\n"
            "+----------+\n";
    cout << "|    " << helpme << "    |\n";
    cout << "+----------+\n";
}
*/
/**
 * Desenha o diâmetro do grafo para o respetivo país.
 * Complexidade Temporal O(|V|*(|V|+|E|)).
 * @param country
 *//*
void GestaoR::drawDiameterCountry(const string &country) {
    int count = 0;
    int max = 0;
    for (int i = 1; i < flightNetwork_.airports.size(); i++) {
        if (flightNetwork_.airports[i].country == country) {
            count = bfsDiameterCountry(i, country);
            if (count > max) { max = count; }
        }
    }
    string helpme;
    if (max < 10) { helpme = "0" + to_string(max); }
    else { helpme = to_string(max); }

    cout << "\n+---------------------------------+----------+\n"
            "|             COUNTRY             | DIAMETER |\n"
            "+---------------------------------+----------+\n";
    cout << "|";
    pair<int, int> pad = auxCenterDraw((int) (getMaxCountryLength() - country.length()), country.length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        cout << " ";
        ++f;
    }
    cout << country;
    for (int e = 0; e < pad.second; e++) {
        cout << " ";
        ++e;
    }
    cout << "|    " << helpme << "    |\n";
    cout << "+---------------------------------+----------+\n";
}
*/
/**
 * Desenha todos os aeroportos numa cidade.
 * Complexidade Temporal O(n).
 * @param airports
 *//*
void GestaoR::drawAirportsByCityCountry(const vector<string> &airports) const {

    cout << "\n+-----+--------------------------------------------------------+\n"
            "| COD |                          NAME                          |\n"
            "+-----+--------------------------------------------------------+\n";

    for (const auto &i: airports) {
        cout << "| " << i << " |";
        pair<int, int> pad = auxCenterDraw(getMaxAirportNameLength() - (int) flightNetwork_.getAirports().at(
                flightNetwork_.getAirportInfo(i)).name.length(), (int) flightNetwork_.getAirports().at(
                flightNetwork_.getAirportInfo(i)).name.length() % 2 == 1);
        for (int f = 0; f < pad.first; f++) {
            cout << " ";
            ++f;
        }
        cout << flightNetwork_.getAirports().at(flightNetwork_.getAirportInfo(i)).name;
        for (int e = 0; e < pad.second; e++) {
            cout << " ";
            ++e;
        }
        cout << "|\n";
    }
    cout << "+-----+--------------------------------------------------------+\n";
}
*/
/**
 * Desenha as companhias aéreas por aeroporto.
 * Complexidade Temporal O(n^2).
 * @param code
 *//*
void GestaoR::drawAirlinesByAirport(const string &code) const {
    set<string> aux = getAirlinesFromAirport(code);
    set<Airline> temp;

    for (auto k: aux) {
        for (auto j: airlines) {
            if (j.getCode() == k) {
                temp.insert(j);
            }
        }
    }

    cout << "\n+---------+--------------------------------------------------------+\n"
            "| AIRLINE |                          NAME                          |\n"
            "+---------+--------------------------------------------------------+\n";

    for (const auto &i: temp) {
        cout << "|";
        pair<int, int> pad = auxCenterDraw(8 - (int) i.getCode().length(), i.getCode().length() % 2 == 1);
        for (int f = 0; f < pad.first; f++) {
            cout << " ";
            ++f;
        }
        cout << i.getCode();
        for (int e = 0; e < pad.second; e++) {
            cout << " ";
            ++e;
        }
        cout << "|";
        pad = auxCenterDraw(getMaxAirportNameLength() - (int) i.getName().length(), i.getName().length() % 2 == 1);
        for (int f = 0; f < pad.first; f++) {
            cout << " ";
            ++f;
        }
        cout << i.getName();
        for (int e = 0; e < pad.second; e++) {
            cout << " ";
            ++e;
        }
        cout << "|\n";
    }

    cout << "+---------+--------------------------------------------------------+\n";
}*/

/**
 * Pergunta ao utilizador se quer organizar a tabela de forma ascendente ou descendente.
 * A Complexidade Temporal desta função depende do tempo que o utilizador demora a dar o input.
 * @return True para input Ascendente (A) e false para input Descendente (D).
 *//*
bool GestaoR::ordenar() {
    string op;
    while (true) {
        cout << "\nDeseja ordenacao ascendente ou descendente? [A/D]:";
        cin >> op;
        if (op == "A" || op == "a") return true;
        else if (op == "D" || op == "d") return false;
        else Menu::teclaErro();
    }
}*/

/**
 * Verifica se o País existe.
 * Complexidade Temporal O(n).
 * @param country
 * @return true caso encontre o país, falso caso não encontre.
 *//*
bool GestaoR::findCountry(const string &country) {
    for (const auto &i: cities) {
        if (i.getCountry() == country) {
            return true;
        }
    }
    return false;
}*/

/**
 * Vai buscar todas as airlines com voos a sair do aeroporto (parâmetro code).
 * Complexidade Temporal O(|E|).
 * @param code
 * @return set com todas as airlines de um aeroporto.
 *//*
set<string> GestaoR::getAirlinesFromAirport(const string &code) const {
    set<string> aux;
    for (auto &airport: flightNetwork_.airports[flightNetwork_.getAirportInfo(code)].flights)
        aux.insert(airport.airline);
    return aux;
}
*/
/*!
 * Desenha o número de pontos de articulação do grafo geral.
 * Complexidade Temporal O(|V|+|V|*(|V|+|E|)).
 *//*
void GestaoR::drawArticulationPoints() {
    for (int i = 1; i < flightNetwork_.airports.size(); i++) {
        flightNetwork_.airports[i].visited = false;
        flightNetwork_.airports[i].in_stack = false;
        flightNetwork_.airports[i].is_articulation = false;
    }
    list<int> art;
    stack<int> aux;
    int nArt = 0;
    for (int i = 1; i < flightNetwork_.airports.size(); i++) {
        if (!flightNetwork_.airports[i].visited)
            dfsArticulationPoints(i, aux, art, 1, nArt);
    }

    string helpme = "0" + to_string(nArt-95);

    cout << "\n+---------------------------------+\n"
            "| NUMERO DE PONTOS DE ARTICULACAO |\n"
            "+---------------------------------+\n";
    cout << "|               " << helpme << "              |\n";
    cout << "+---------------------------------+\n";
}
*/
/*!
 * Desenha os pontos de articulação do grafo geral.
 * Complexidade Temporal O(|V|+|V|*(|V|+|E|)).
 *//*
void GestaoR::drawListArticulationPoints() {
    for (int i = 1; i < flightNetwork_.airports.size(); i++) {
        flightNetwork_.airports[i].visited = false;
        flightNetwork_.airports[i].in_stack = false;
        flightNetwork_.airports[i].is_articulation = false;
    }
    list<int> art;
    stack<int> aux;
    int nArt = 0;
    for (int i = 1; i < flightNetwork_.airports.size(); i++) {
        if (!flightNetwork_.airports[i].visited)
            dfsArticulationPoints(i, aux, art, 1, nArt);
    }

    cout << "\n+-----+--------------------------------------------------------+\n"
            "| COD |                          NAME                          |\n"
            "+-----+--------------------------------------------------------+\n";

    for (const auto &i: art) {
        cout << "| " << flightNetwork_.airports[i].code << " |";
        pair<int, int> pad = auxCenterDraw(getMaxAirportNameLength() - (int) flightNetwork_.airports[i].name.length(),
                                           (int) flightNetwork_.airports[i].name.length() % 2 == 1);
        for (int f = 0; f < pad.first; f++) {
            cout << " ";
            ++f;
        }
        cout << flightNetwork_.airports[i].name;
        for (int e = 0; e < pad.second; e++) {
            cout << " ";
            ++e;
        }
        cout << "|\n";
    }
    cout << "+-----+--------------------------------------------------------+\n";
}*/

/*!
 * Depth-First Search para calcular os pontos de articulação.
 * Complexidade Temporal O(|V|+|E|).
 * @param v
 * @param aux
 * @param art
 * @param index
 * @param nArt
 *//*
void GestaoR::dfsArticulationPoints(int v, stack<int> &aux, list<int> &art, int index, int &nArt) {
    flightNetwork_.airports[v].visited = true;
    flightNetwork_.airports[v].low = flightNetwork_.airports[v].num = index;
    index++;
    aux.push(v);
    flightNetwork_.airports[v].in_stack = true;
    int count = 0;

    for (const auto &f: flightNetwork_.airports[v].flights) {
        int w = f.dest;
        if (!flightNetwork_.airports[w].visited) {
            count++;
            dfsArticulationPoints(w, aux, art, index, nArt);
            flightNetwork_.airports[v].low = min(flightNetwork_.airports[v].low, flightNetwork_.airports[w].low);
        } else if (flightNetwork_.airports[w].in_stack)
            flightNetwork_.airports[v].low = min(flightNetwork_.airports[v].low, flightNetwork_.airports[w].num);

        if (flightNetwork_.airports[v].num != 1 && !flightNetwork_.airports[v].is_articulation &&
            flightNetwork_.airports[w].low >= flightNetwork_.airports[v].num) {
            art.push_back(v);
            flightNetwork_.airports[v].is_articulation = true;
            nArt++;
        } else if (flightNetwork_.airports[v].num == 1 && !flightNetwork_.airports[v].is_articulation && count > 1) {
            art.push_back(v);
            flightNetwork_.airports[v].is_articulation = true;
            nArt++;
        }
    }
}
*/