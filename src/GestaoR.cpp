#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>

#include "../header/Menu.h"
#include "../header/GestaoR.h"
#include "../header/Station.h"
#include "../header/Network.h"

GestaoR::GestaoR() {
    railwayNetwork = Graph();
}

/**
 * Lê o ficheiro stations.csv e guarda todas as companhias aéreas.
 * Complexidade Temporal O(n) (n é o tamanho do ficheiro retirando o cabeçalho).
 */
void GestaoR::readStations() {
    std::string ficheiro = "../resources/stations.csv";
    std::ifstream ifs1;
    ifs1.open(ficheiro, std::ios::in);
    std::string segment;
    getline(ifs1, segment);

    while (getline(ifs1, segment)) {
        std::string name, district, municipality, township, line;
        std::istringstream stream(segment);
        getline(stream, name, ',');
        getline(stream, district, ',');
        getline(stream, municipality, ',');
        getline(stream, township, ',');
        getline(stream, line, '\r');
        Station station = Station(name, district, municipality, township, line);

    }
    ifs1.close();
}

/**
 * Lê o ficheiro network.csv e vai colocando os aerportos no grafo, assim como guarda todas as cidades diferentes.
 * Complexidade Temporal O(n) (n é o tamanho do ficheiro retirando o cabeçalho).
 */
void GestaoR::readNetwork() {
    std::string ficheiro = "../resources/airports.csv";
    std::ifstream ifs2;
    ifs2.open(ficheiro, std::ios::in);
    std::string segment;
    getline(ifs2, segment);

    while (getline(ifs2, segment)) {
        std::string station_A, station_B, capacityS, service;
        int capacity;
        std::istringstream stream(segment);
        getline(stream, station_A, ',');
        getline(stream, station_B, ',');
        getline(stream, capacityS, ',');
        getline(stream, service, '\r');
        capacity = stoi(capacityS);
        Network network = Network(station_A, station_B, capacity, service);
        // TODO ADD NETWORK TO "GRAPH" OR SOMETHING

    }
    ifs2.close();
}

/**
 * Função para ajudar a centralizar textos.
 * Complexidade Temporal O(1).
 * @param n
 * @param v
 * @return par com o número de espaços e se a palavra é par ou ímpar.
 *//*
pair<int, int> GestaoR::auxCenterDraw(int n, bool v) {
    int pad1 = n;
    int pad2;
    if (v) pad2 = pad1 + 1;
    else pad2 = pad1;
    return pair<int, int>{pad1, pad2};
}*/

/**
 * Desenho do Menu principal.
 * Complexidade Temporal O(1).
 */
void GestaoR::drawMenu() {
    std::cout << "\n+---------------------------------------------+\n"
            "|               RAILWAY NETWORK               |\n"
            "+---------------------------------------------+\n"
            "| [1] - Listagens Completas                   |\n" // irá abrir novo menu
            "| [2] - N a partir de um Aeroporto            |\n" // irá abrir novo menu
            "| [3] - Destinos com maximo de Y voos         |\n" // irá abrir novo menu
            "| [4] - Diametros                             |\n" // irá abrir novo menu
            "| [5] - Viajar entre dois locais              |\n" // irá abrir novo menu
            "| [6] - Pontos de articulacao                 |\n" // irá abrir novo menu
            "| [Q] - Sair da aplicacao                     |\n"
            "+---------------------------------------------+\n";
    std::cout << "\nEscolha a opcao e pressione ENTER:";
}

/**
 * Desenha um menu secundário para mais opções.
 * Complexidade Temporal O(1).
 */
void GestaoR::drawListagemMenu() {
    std::cout << "\n+-----------------------------------------------------+\n"
            "|                GESTAO DE AEROPORTOS                 |\n"
            "+-----------------------------------------------------+\n"
            "| [1] - Listar Cidades                                |\n"
            "| [2] - Listar Aeroportos                             |\n"
            "| [3] - Listar Companhias Aereas                      |\n"
            "| [4] - Listar Voos                                   |\n"
            "| [5] - Listar Aeroportos numa Cidade                 |\n"
            "| [6] - Listar Companhias Aereas num Aeroporto        |\n"
            "| [V] - Voltar                                        |\n"
            "+-----------------------------------------------------+\n";
    std::cout << "\nEscolha a opcao e pressione ENTER:";
}

/**
 * Desenho de um menu secundário para mais opções.
 * Complexidade Temporal O(1).
 */
void GestaoR::drawNumberMenu() {
    std::cout << "\n+-----------------------------------------------------+\n"
            "|                GESTAO DE AEROPORTOS                 |\n"
            "+-----------------------------------------------------+\n"
            "| [1] - N Companhias Aereas num Aeroporto             |\n"
            "| [2] - N Destinos Diferentes a partir de um Aeroporto|\n"
            "| [3] - N Paises Diferentes a partir de um Aeroporto  |\n"
            "| [V] - Voltar                                        |\n"
            "+-----------------------------------------------------+\n";
    std::cout << "\nEscolha a opcao e pressione ENTER:";
}

/**
 * Desenho de um menu secundário para mais opções.
 * Complexidade Temporal O(1).
 */
void GestaoR::drawYMenu() {
    std::cout << "\n+---------------------------------------------------+\n"
            "|                GESTAO DE AEROPORTOS               |\n"
            "+---------------------------------------------------+\n"
            "| [1] - Aeroportos Atingiveis num maximo de Y voos  |\n"
            "| [2] - Cidades Atingiveis num maximo de Y voos     |\n"
            "| [3] - Paises Atingiveis num maximo de Y voos      |\n"
            "| [V] - Voltar                                      |\n"
            "+---------------------------------------------------+\n";
    std::cout << "\nEscolha a opcao e pressione ENTER:";
}

/**
 * Desenha uma cidade (parâmetro cc) e identifica se é a 1ª cidade a ser escrita para desenhar o cabeçalho da tabela (parâmetro header).
 * Complexidade Temporal O(n).
 * @param cc
 * @param header
 *//*
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
 *//*
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
 *//*
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
 *//*
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
 *//*
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
 *//*
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
 *//*
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
 *//*
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
 *//*
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
 *//*
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