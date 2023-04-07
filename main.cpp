#include <iostream>

#include "header/GestaoR.h"
#include "header/Menu.h"

int main() {
    GestaoR r;
    r.readStations();
    r.readNetwork();

    Menu::abriuAplicacao();

    // ciclo do programa
    while (true) {
        GestaoR::drawMenu(); // desenha o menu
        std::string op;
        std::cin >> op;
        if (op.length() != 1) {
            Menu::teclaErro();
            continue;
        }
        else if (op == "1") {
            while (true) {
                GestaoR::drawListagemMenu();
                std::string op1;
                std::cin >> op1;
                if (op1.length() != 1) {
                    Menu::teclaErro();
                    continue;
                } else if (op1 == "1") {
                    // listar as estações
                    r.drawStations();
                    Menu::voltar();
                }
                else if (op1 == "2") {
                    // listar as ligações
                    r.drawNetwork();
                    Menu::voltar();
                }
                else if (op1 == "3") {
                    // listar as ligações de uma estação
                    std::string name;
                    bool ignoreCin = true;
                    std::cout << "\nEnter station name: ";
                    if (ignoreCin) std::cin.ignore();
                    ignoreCin = false;
                    std::getline(std::cin, name);
                    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                    r.drawStationNetwork(name);
                    Menu::voltar();
                }
                else if (op1 == "V" || op1 == "v") break;
                else Menu::teclaErro();
            }
        }
        else if (op == "2") {
            std::string source, target;
            double flow;
            bool ignoreCin = true;
            while (true) {
                std::cout << "\nEnter the name of the departure station: ";
                if (ignoreCin) std::cin.ignore();
                ignoreCin = false;
                std::getline(std::cin, source);
                if (source.length() < 1) {
                    Menu::teclaErro();
                    continue;
                }
                std::cout << "\nEnter the name of the arrival station: ";
                std::getline(std::cin, target);
                if (target.length() < 1) {
                    Menu::teclaErro();
                    continue;
                }
                std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                std::transform(target.begin(), target.end(), target.begin(), ::toupper);
                flow = r.edmondsKarp(source, target);
                if (flow == 0.0) {
                    Menu::semCaminhoPossivel();
                }
                else if (flow == -1.0) continue;
                else {
                    std::cout << "\nThe maximum number of trains running between " << source << " and " << target << " is " << flow << "." << std::endl;
                }
                break;
            }
            Menu::voltar();
        }
        else if (op == "3") {
            r.fullAdvantage();
            Menu::voltar();
        }
        else if (op == "4") {
            while (true) {
                GestaoR::drawBudgetMenu();
                std::string op4;
                std::cin >> op4;
                if (op4.length() != 1) {
                    Menu::teclaErro();
                    continue;
                } else if (op4 == "1") {
                    // manutenção
                    std::vector<GestaoR::Municipality> municipalities = r.managementRailway();
                    std::sort(municipalities.begin(), municipalities.end(), [](const GestaoR::Municipality &a, const GestaoR::Municipality &b) {
                        return a.numberOfStations/a.numberOfTrains < b.numberOfStations/b.numberOfTrains;
                    });
                    std::string k;
                    while (true) {
                        std::cout << "\nDefine K: ";
                        std::cin >> k;
                        if (k.find_first_not_of("1234567890") != std::string::npos || k == "0") {
                            Menu::teclaErro();
                            continue;
                        }
                        else if (std::stoi(k) > municipalities.size()) {
                            Menu::numeroMenor();
                            continue;
                        }
                        break;
                    }
                    for (int i = 0; i < std::stoi(k); i++) {
                        std::cout << "Name: " << municipalities.at(i).municipality << " ratio: " << municipalities.at(i).numberOfStations/municipalities.at(i).numberOfTrains << std::endl;
                    }
                    Menu::voltar();
                } else if (op4 == "2") {
                    // compra
                    std::vector<GestaoR::Municipality> municipalities = r.managementRailway();
                    std::sort(municipalities.begin(), municipalities.end(), [](const GestaoR::Municipality &a, const GestaoR::Municipality &b) {
                        return a.numberOfTrains/a.numberOfStations < b.numberOfTrains/b.numberOfStations;
                    });
                    std::string k;
                    while (true) {
                        std::cout << "\nDefine K: ";
                        std::cin >> k;
                        if (k.find_first_not_of("1234567890") != std::string::npos || k == "0") {
                            Menu::teclaErro();
                            continue;
                        }
                        else if (std::stoi(k) > municipalities.size()) {
                            Menu::numeroMenor();
                            continue;
                        }
                        break;
                    }
                    for (int i = 0; i < std::stoi(k); i++) {
                        double ratio = municipalities.at(i).numberOfTrains/municipalities.at(i).numberOfStations;
                        std::cout << "\nName: " << municipalities.at(i).municipality << " ratio: " << ratio << std::endl;
                    }
                    Menu::voltar();
                } else if (op4 == "V" || op4 == "v") break;
                else Menu::teclaErro();
            }
        }
        else if (op == "5") {
            std::pair<std::string, std::string> result = r.maxEdmondsKarp();
            std::cout << "\nThe maximum number of trains running between two stations is " << r.edmondsKarp(result.first, result.second) << " between the stations of " << result.first << " and " << result.second << "." << std::endl;
        }
        else if (op == "6") {
            while (true) {
                GestaoR::drawBudgetMenu();
                std::string op6;
                std::cin >> op6;
                if (op6.length() != 1) {
                    Menu::teclaErro();
                    continue;
                } else if (op6 == "1") {
                    // concelhos
                    std::string k;
                    while (true) {
                        std::cout << "\nDefine K: ";
                        std::cin >> k;
                        if (k.find_first_not_of("1234567890") != std::string::npos || k == "0") {
                            Menu::teclaErro();
                            continue;
                        }
                        else if (std::stoi(k) > r.getMSize()) {
                            Menu::numeroMenor();
                            continue;
                        }
                        break;
                    }
                    r.topK(true, std::stoi(k));
                    Menu::voltar();
                } else if (op6 == "2") {
                    // distritos
                    std::string k;
                    while (true) {
                        std::cout << "\nDefine K: ";
                        std::cin >> k;
                        if (k.find_first_not_of("1234567890") != std::string::npos || k == "0") {
                            Menu::teclaErro();
                            continue;
                        }
                        else if (std::stoi(k) > r.getDSize()) {
                            Menu::numeroMenor();
                            continue;
                        }
                        break;
                    }
                    r.topK(false, std::stoi(k));
                    Menu::voltar();
                } else if (op6 == "V" || op6 == "v") break;
            }
        }
        else if (op == "7") {
            std::string origin;
            double flow;
            bool ignoreCin = true;
            while (true) {
                std::cout << "\nEnter station name: ";
                if (ignoreCin) std::cin.ignore();
                ignoreCin = false;
                std::getline(std::cin, origin);
                if (origin.length() < 1) {
                    Menu::teclaErro();
                    continue;
                }
                std::transform(origin.begin(), origin.end(), origin.begin(), ::toupper);
                flow = r.maxFlowOrigin(origin);
                if (flow == 0.0) {
                    std::cout << "\nThe entered station does not have a possible connection." << std::endl;
                }
                else if (flow == -1.0) continue;
                else {
                    std::cout << "\nThe maximum number of trains running for " << origin << " if " << flow << "." << std::endl;
                }
                break;
            }
            Menu::voltar();
        }
        else if (op == "8") {
            std::string source, target;
            double dijkstra;
            bool ignoreCin = true;
            while (true) {
                std::cout << "\nEnter the name of the departure station: ";
                if (ignoreCin) std::cin.ignore();
                ignoreCin = false;
                std::getline(std::cin, source);
                if (source.length() < 1) {
                    Menu::teclaErro();
                    continue;
                }
                std::cout << "\nEnter the name of the arrival station: ";
                std::getline(std::cin, target);
                if (target.length() < 1) {
                    Menu::teclaErro();
                    continue;
                }
                std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                std::transform(target.begin(), target.end(), target.begin(), ::toupper);
                std::vector<std::string> path;
                double minWeigth = INF;
                dijkstra = r.dijkstraShortestPathCost(source, target, path, minWeigth);
                if (dijkstra == 0.0) Menu::semCaminhoPossivel();
                else if (dijkstra != -1) {
                    std::reverse(path.begin(), path.end());
                    std::cout << "\nThe maximum number of trains from " << source << " to " << target << " is " << minWeigth << std::endl;
                    std::cout << "Path: ";
                    for (auto &p : path) {
                        if (p != path.at(path.size()-1))
                            std::cout << p << " -> ";
                        else std::cout << p;
                    }
                    std::cout << "\nThe cost of the path is " << dijkstra << std::endl;
                }
                Menu::voltar();
                break;
            }
        }
        else if (op == "q" || op == "Q") {
            Menu::fechouAplicacao();
            break;
        }
        else Menu::teclaErro();
    }
    return 0;
}
