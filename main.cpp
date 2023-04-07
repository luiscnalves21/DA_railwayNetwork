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
            std::pair<std::string, std::string> result = r.maxEdmondsKarp();
            std::cout << "\nThe maximum number of trains running between two stations is " << r.edmondsKarp(result.first, result.second) << " between the stations of " << result.first << " and " << result.second << "." << std::endl;
            Menu::voltar();
        }
        else if (op == "4") {
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
        else if (op == "5") {
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
                    Menu::semCaminhoEstacao();
                }
                else if (flow == -1.0) continue;
                else {
                    std::cout << "\nThe maximum number of trains running for " << origin << " if " << flow << "." << std::endl;
                }
                break;
            }
            Menu::voltar();
        }
        else if (op == "6") {
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
                    std::cout << "\nPath: ";
                    for (auto &p : path) {
                        if (p != path.at(path.size()-1))
                            std::cout << p << " -> ";
                        else std::cout << p;
                    }
                    std::cout << "\n\nThe cost of the path is " << dijkstra << std::endl;
                }
                Menu::voltar();
                break;
            }
        }
        else if (op == "7" ) {
            std::string source, target, res;
            double exist;
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

                exist = r.existStations(source, target);

                if (exist == -1.0) {
                    continue;
                }
                else if (exist == 0.0) {
                    Menu::semCaminhoPossivel();
                    continue;
                }
                else {
                    while (true) {
                        std::cout << "\nCapacity: " << exist << " -- " << source << " -> " << target << "\n";
                        std::cout << "\nDo you want to break the edge or decrease the capacity? (B/D): ";
                        std::cin >> res;
                        if (res == "B" || res == "b") {
                            r.breakEdge(source, target);
                            break;
                        } else if (res == "D" || res == "d") {
                            std::string capacity;
                            while (true) {
                                std::cout << "\nEnter the new capacity: ";
                                std::cin >> capacity;
                                if (capacity.find_first_not_of("1234567890") != std::string::npos) {
                                    Menu::teclaErro();
                                    continue;
                                } else if (std::stoi(capacity) > exist || std::stoi(capacity) == exist) {
                                    Menu::numeroMenor();
                                    continue;
                                }
                                break;
                            }
                            r.decreaseEdge(source, target, std::stoi(capacity));
                            break;
                        } else {
                            Menu::teclaErro();
                            continue;
                        }
                    }
                    while (true) {
                        std::cout << "\nDo you want to add another edge? (Y/N): ";
                        std::cin >> res;
                        if (res == "Y" || res == "y") {
                            ignoreCin = true;
                            break;
                        } else if (res == "N" || res == "n") break;
                        else {
                            Menu::teclaErro();
                            continue;
                        }
                    }
                    if (ignoreCin) continue;
                }
                Menu::voltar();
                break;
            }
        }
        else if (op == "8") {

        }
        else if (op == "r" || op == "R") {
            r.readStations();
            r.readNetwork();
            std::cout << "\nGraph reseted!" << std::endl;
            Menu::voltar();
        }
        else if (op == "q" || op == "Q") {
            Menu::fechouAplicacao();
            break;
        }
        else Menu::teclaErro();
    }
    return 0;
}
