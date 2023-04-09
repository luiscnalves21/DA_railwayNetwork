#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#include "../header/GestaoR.h"
#include "../header/Menu.h"

/**
 * @brief Constructor of the GestaoR class
 * Time Complexity: O(1)
 */
GestaoR::GestaoR() {
    railwayNetwork = Graph();
}

/**
 * @brief Reads the file stations.csv and adds the stations to the graph, as well as grouping them by municipality and district.
 * Time Complexity: O(n) (n is the size of the file excluding the header)
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
 * @brief Reads the file network.csv and adds the connections to the graph.
 * Time Complexity: O(n) (n is the size of the file excluding the header)
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

/**
 * @return the vector of stations of the graph
 */
std::vector<Vertex *> GestaoR::getVertexSet() const { return railwayNetwork.getVertexSet(); }

/**
 * @brief Checks if the station exists in the graph and if it does, executes the Edmonds-Karp algorithm.
 * Time Complexity: O(V*E^2+n+E) where V is the number of stations in the network, E is the number of edges, and n is the number of stations searched for in the network
 * @param source
 * @param target
 * @return the flow of the path
 */
double GestaoR::edmondsKarp(const std::string &source, const std::string &target) {
    int sourceId = railwayNetwork.findVertexName(source);
    int targetId = railwayNetwork.findVertexName(target);
    if (sourceId == -1) {
        Menu::estacaoNaoEncontrada();
        return -1.0;
    }
    else if (targetId == -1) {
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

/**
 * @brief Executes the Edmonds-Karp algorithm for all pairs of stations in the graph
 * Time Complexity: O(V^3*E^2) where V is the number of stations in the network, and E is the number of edges
 * @param vertexs
 * @return the maximum flow of a pair of stations
 */
double GestaoR::maxEdmondsKarp(std::vector<std::pair<std::string, std::string>> &vertexs) {
    double flow = 0.0, tempFlow;
    std::pair<std::string, std::string> result;
    for (int i = 0; i < getVertexSet().size()-1; i++) {
        for (int j = i+1; j < getVertexSet().size(); j++) {
            tempFlow = edmondsKarp(getVertexSet().at(i)->getName(), getVertexSet().at(j)->getName());
            if (tempFlow > flow) {
                vertexs.clear();
                flow = tempFlow;
                result.first = getVertexSet().at(i)->getName();
                result.second = getVertexSet().at(j)->getName();
                vertexs.push_back(result);
            }
            else if (tempFlow == flow) {
                result.first = getVertexSet().at(i)->getName();
                result.second = getVertexSet().at(j)->getName();
                vertexs.push_back(result);
            }
        }
    }
    return flow;
}

/**
 * @brief Executes the Edmonds-Karp algorithm for all pairs of stations inside a municipality or district
 * Time Complexity: O(n^2*V*E^2) where V is the number of stations in the network, E is the number of edges, and n is the size of the input vector
 * @param names
 * @return the maximum flow of the municipality or district
 */
double GestaoR::maxEKtopK(const std::vector<std::string> &names) {
    double flow = 0.0;
    for (int i = 0; i < names.size()-1; i++) {
        for (int j = i+1; j < names.size(); j++) {
            flow += edmondsKarp(names.at(i), names.at(j));
        }
    }
    return flow;
}

/**
 * @brief Executes the Edmonds-Karp algorithm for all pairs of stations inside a municipality or district
 * Time Complexity: O(n^2*V*E^2+n*log(n)) where n is the size of the input vector, and V and E are the number of vertices and edges in the network
 * @param mORd
 * @param k
 */
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

/**
 * @brief Executes the Edmonds-Karp algorithm creating a temporary vertex and connecting it to all the stations that have only one edge
 * Time Complexity: O(V*E^2) where V is the number of stations in the network, and E is the number of edges
 * @param origin
 * @return
 */
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

/**
 * @brief executes the Dijkstra algorithm to find the cheapest path between two stations
 * Time Complexity: O(E+VlogV) where E is the number of edges and V is the number of stations in the network
 * @param source
 * @param target
 * @param path
 * @param minWeigth
 * @return the cost of the path
 */
double GestaoR::dijkstraShortestPathCost(const std::string &source, const std::string &target, std::vector<std::string> &path, double &minWeigth) {
    int sourceId = railwayNetwork.findVertexName(source);
    int targetId = railwayNetwork.findVertexName(target);
    if (sourceId == -1) {
        Menu::estacaoNaoEncontrada();
        return -1.0;
    }
    else if (targetId == -1) {
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
    while (temp->getPath() != nullptr) {
        path.push_back(temp->getPath()->getOrig()->getName());
        if (temp->getPath()->getWeight() < minWeigth)
            minWeigth = temp->getPath()->getWeight();
        temp = temp->getPath()->getOrig();
    }
    double cost = 0.0;
    while (targetVertex->getPath() != nullptr) {
        if (targetVertex->getPath()->getService() == "STANDARD")
            cost += (minWeigth * 2);
        else cost += (minWeigth * 4);
        targetVertex = targetVertex->getPath()->getOrig();
    }
    return cost;
}

bool GestaoR::existStation(const std::string &name) { return railwayNetwork.findVertexName(name) != -1; }

/**
 * @brief Checks if there is a connection between two stations
 * Time Complexity: O(n) that is the number of edges of the source vertex
 * @param source
 * @param target
 * @return the weight of the connection if it exists, 0 otherwise
 */
double GestaoR::existConnection(const std::string &source, const std::string &target) const {
    int sourceId = railwayNetwork.findVertexName(source);
    int targetId = railwayNetwork.findVertexName(target);

    for (Edge *edge : getVertexSet().at(sourceId)->getAdj()) {
        if (edge->getDest()->getId() == targetId) {
            return edge->getWeight();
        }
    }
    return 0.0;
}

/**
 * @brief Shows the connections of a given station
 * Time Complexity: O(n) that is the number of edges of the source vertex
 * @param source
 * @return true if there are connections, false otherwise
 */
bool GestaoR::showAdjacents(const std::string &source) {
    int sourceId = railwayNetwork.findVertexName(source);
    if (sourceId == -1) {
        Menu::estacaoNaoEncontrada();
        return false;
    }
    int counter = 0;
    for (Edge *edge : getVertexSet().at(sourceId)->getAdj())
        if (edge->getWeight() != 0.0) counter++;
    if (counter == 0) {
        std::cout << "\n--- There are no destinations/connections from " << source << " ---" << std::endl;
        return false;
    }
    drawConnections(sourceId);
    return true;
}

/**
 * @brief Decreases the capacity of an edge
 * Time Complexity: O(n+m) where n is the number of edges of the source vertex and m is the number of edges of the target vertex
 * @param source
 * @param target
 * @param capacity
 */
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
        if (edge->getDest()->getName() == source) {
            edge->setWeight(capacity);
            break;
        }
    }
    if (capacity == 0.0) std::cout << "\nEdge " << source << " - " << target << " broken." << std::endl;
    else std::cout << "\nEdge " << source << " - " << target << " decreased." << std::endl;
}

/**
 * @brief Function to help center text
 * Time Complexity: O(1)
 * @param n
 * @param v
 * @return pair with the number of spaces and if the word is even or odd.
*/
std::pair<int, int> GestaoR::auxCenterDraw(int n, bool v) {
    int pad1 = n;
    int pad2;
    if (!v) pad2 = pad1 + 1;
    else pad2 = pad1;
    return std::pair<int, int>{pad1, pad2};
}

/**
 * @brief Draws the program's main menu
 * Time Complexity: O(1)
*/
void GestaoR::drawMenu() {
    std::cout << "\n+-------------------------------------------------------------+\n"
                 "|                     RAILWAY MANAGEMENT                      |\n"
                 "+-------------------------------------------------------------+\n"
                 "| [1] - Complete Listings                                     |\n"
                 "| [2] - Basic Service Metrics                                 |\n"
                 "| [3] - Operation Cost Optimization                           |\n"
                 "| [4] - Reduced Connectivity & Report                         |\n"
                 "| [R] - Reset Graph                                           |\n"
                 "| [Q] - Exit the application                                  |\n"
                 "+-------------------------------------------------------------+\n";
    std::cout << "\nChoose the option and press ENTER:";
}

/**
 * @brief Draws a menu that allows the user to choose the basic service metrics that he wants to see
 * Time Complexity: O(1)
*/
void GestaoR::drawBasicCostMetrics() {
    std::cout << "\n+-------------------------------------------------------------+\n"
                 "|                    BASIC SERVICE METRICS                    |\n"
                 "+-------------------------------------------------------------+\n"
                 "| [1] - Max-Flow Between Two Stations                         |\n"
                 "| [2] - Pair of Stations - Most Amount of Trains              |\n"
                 "| [3] - Purchasing & Maintenance - Budget                     |\n"
                 "| [4] - Max Number of Trains - Entire Railway Grid            |\n"
                 "| [B] - Go Back                                               |\n"
                 "+-------------------------------------------------------------+\n";
    std::cout << "\nChoose the option and press ENTER:";
}

/**
 * @brief Draws a menu that allows the user to choose between listing the stations and listing the connections of a station
 * Time Complexity: O(1)
*/
void GestaoR::drawListagemMenu() {
    std::cout << "\n+-------------------------------------------------------------+\n"
                 "|                     RAILWAY MANAGEMENT                      |\n"
                 "+-------------------------------------------------------------+\n"
                 "| [1] - List Stations                                         |\n"
                 "| [2] - List Station Connections                              |\n"
                 "| [B] - Go Back                                               |\n"
                 "+-------------------------------------------------------------+\n";
    std::cout << "\nChoose the option and press ENTER:";
}

/**
 * @brief Draws a menu that allows the user to choose between the municipalities and districts
 * Time Complexity: O(1)
 */
void GestaoR::drawBudgetMenu() {
    std::cout << "\n+-------------------------------------------------------------+\n"
                 "|                     RAILWAY MANAGEMENT                      |\n"
                 "+-------------------------------------------------------------+\n"
                 "| [1] - Municipalities                                        |\n"
                 "| [2] - Districts                                             |\n"
                 "| [B] - Go Back                                               |\n"
                 "+-------------------------------------------------------------+\n";
    std::cout << "\nChoose the option and press ENTER:";
}

/**
 * @brief Draws a station
 * Time Complexity: O(1)
 * @param order
 * @param header
 * @param pos
 * @param mORd
 */
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
    pad = auxCenterDraw(maxLength - (int) order.length(),
                                                   (int) order.length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << order;
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|" << "\n";
}

/**
 * @brief Draws the top k stations grouped by municipality or district
 * Time Complexity: O(k) where k is the number of stations to be drawn
 * @param order
 * @param mORd
 * @param k
 */
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

/**
 * @brief Draws the station and respective line
 * Time Complexity: O(1)
 * @param vertex
 * @param header
 */
void GestaoR::drawStationAndLine(const Vertex *vertex, bool header) const {
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
    pad = auxCenterDraw(getMaxLineLength() - (int) vertex->getLine().length(),
                                                   (int) vertex->getLine().length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << vertex->getLine();
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|" << "\n";
}

/**
 * @brief Draws all the stations and respective lines
 * Time Complexity: O(V) where V is the number of stations
 */
void GestaoR::drawStationsAndLine() const {
    bool header = true;
    for (const Vertex *vertex : getVertexSet()) {
        drawStationAndLine(vertex, header);
        header = false;
    }
    std::cout << "+------------------------------------+--------------------------+\n";
}

/**
 * @brief Draws the connections of a station
 * Time Complexity O(1)
 * @param edge
 * @param header
 */
void GestaoR::drawConnection(const Edge *edge, bool header) const {
    if (header) {
        std::cout << "\n+------------------------------------+------------------+------+\n"
                     "|                          DESTINATIONS                        |\n"
                     "+------------------------------------+------------------+------+\n"
                     "|                NAME                |     SERVICE      | CAP  |\n"
                     "+------------------------------------+------------------+------+\n";
    }
    std::cout << "|";
    std::pair<int, int> pad = auxCenterDraw(getMaxStationLength() - (int) edge->getDest()->getName().length(),
                                            (int) edge->getDest()->getName().length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << edge->getDest()->getName();
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|";
    pad = auxCenterDraw(17 - (int) edge->getService().length(),
                                                   (int) edge->getService().length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << edge->getService();
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|";
    std::string capacity = std::to_string(static_cast<int>(std::floor(edge->getWeight())));
    pad = auxCenterDraw(5 - (int) capacity.length(),
                                                   (int) capacity.length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << capacity;
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|" << "\n";
}

/**
 * @brief Draws the connections of a station
 * Time Complexity O(n) where n is the number of connections (edges)
 * @param sourceId
 */
void GestaoR::drawConnections(const int &sourceId) const {
    bool header = true;
    for (const Edge *edge : getVertexSet().at(sourceId)->getAdj()) {
        if (edge->getWeight() != 0) {
            drawConnection(edge, header);
            header = false;
        }
    }
    std::cout << "+------------------------------------+------------------+------+\n";
}

/**
 * @brief Draws the most affected stations one by one
 * Time Complexity: O(1)
 * @param par
 * @param header
 */
void GestaoR::drawReportedStation(const std::pair<std::string, double> &par, bool header) const {
    if (header) {
        std::cout << "\n+-----------------------------------------------------+\n"
                     "|              MOST AFFECTED STATIONS                 |\n"
                     "+------------------------------------+----------------+\n"
                     "|                NAME                | DIFFERENCE (-) |\n"
                     "+------------------------------------+----------------+\n";
    }
    std::cout << "|";
    std::pair<int, int> pad = auxCenterDraw(getMaxStationLength() - (int) par.first.length(),
                                            (int) par.first.length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << par.first;
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|";
    pad= auxCenterDraw(15 - (int) std::to_string((int)par.second).length(),
                                                   (int) std::to_string((int)par.second).length() % 2 == 0);
    for (int f = 0; f < pad.first; f++) {
        std::cout << " ";
        ++f;
    }
    std::cout << std::to_string((int)par.second);
    for (int e = 0; e < pad.second; e++) {
        std::cout << " ";
        ++e;
    }
    std::cout << "|" << "\n";
}

/**
 * @brief Draws the most affected stations
 * Time Complexity: O(n) where n is the size of affectedStations
 * @param affectedStations
 */
void GestaoR::drawReportedStations(const std::vector<std::pair<std::string, double>> &affectedStations) const {
    bool header = true;
    for (const auto &par : affectedStations) {
        if (par.second != 0.0) {
            drawReportedStation(par, header);
            header = false;
        }
    }
    std::cout << "+------------------------------------+----------------+\n";

}

/**
 * @brief Sets the maximum length of a station
 * @param maxLength the maximum length of a station
*/
void GestaoR::setMaxStationLength(int maxLength) { maxStationLength = maxLength; }

/**
 * @brief Sets the maximum length of a municipality
 * @param maxLength the maximum length of a municipality
*/
void GestaoR::setMaxMunicipalityLength(int maxLength) { maxMunicipalityLength = maxLength; }

/**
 * @brief Sets the maximum length of a district
 * @param maxLength the maximum length of a district
*/
void GestaoR::setMaxDistrictLength(int maxLength) { maxDistrictLength = maxLength; }

/**
 * @brief Sets the maximum length of a line
 * @param maxLength the maximum length of a line
*/
void GestaoR::setMaxLineLength(int maxLength) { maxLineLength = maxLength; }

/**
 * @return the maximum length of a station
*/
int GestaoR::getMaxStationLength() const { return maxStationLength; }

/**
 * @return the maximum length of a municipality
*/
int GestaoR::getMaxMunicipalityLength() const { return maxMunicipalityLength; }

/**
 * @return the maximum length of a district
*/
int GestaoR::getMaxDistrictLength() const { return maxDistrictLength; }

/**
 * @return the maximum length of a line
*/
int GestaoR::getMaxLineLength() const { return maxLineLength; }

/**
 * @return the number of municipalities
*/
int GestaoR::getMSize() const { return railwayNetwork.getMSize(); }

/**
 * @return the number of districts
 */
int GestaoR::getDSize() const { return railwayNetwork.getDSize(); }
