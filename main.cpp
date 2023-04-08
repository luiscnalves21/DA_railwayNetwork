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
                    r.drawStationsAndLine();
                    Menu::voltar();
                }
                else if (op1 == "2") {
                    // listar as ligações de uma estação
                    std::string name;
                    bool ignoreCin = true;
                    while (true) {
                        std::cout << "\nEnter station name: ";
                        if (ignoreCin) std::cin.ignore();
                        ignoreCin = false;
                        std::getline(std::cin, name);
                        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                        if (name.length() < 1) {
                            Menu::teclaErro();
                            continue;
                        }
                        else if (r.existStation(name)) {
                            //r.drawStationConnections(name);
                            r.showAdjacents(name);
                            break;
                        }
                        else {
                            Menu::estacaoNaoEncontrada();
                            continue;
                        }
                    }
                    Menu::voltar();
                }
                else if (op1 == "B" || op1 == "b") break;
                else Menu::teclaErro();
            }
        }
        else if (op == "2") {
            while (true) {
                GestaoR::drawBasicCostMetrics();
                std::string op2;
                std::cin >> op2;
                if (op2.length() != 1) {
                    Menu::teclaErro();
                    continue;
                }
                else if (op2 == "1") {
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
                        } else if (flow == -1.0) continue;
                        else {
                            std::cout << "\nThe maximum number of trains running between " << source << " and "
                                      << target
                                      << " is " << flow << "." << std::endl;
                        }
                        break;
                    }
                    Menu::voltar();
                }
                else if (op2 == "2") {
                    std::vector<std::pair<std::string, std::string>> vertexs;
                    double flow = r.maxEdmondsKarp(vertexs);
                    std::cout << "\nThe maximum number of trains running between two stations is " << flow
                              << " between the stations of:\n" << std::endl;
                    for (auto &result: vertexs) {
                        std::cout << result.first << " <-> " << result.second << std::endl;
                    }
                    Menu::voltar();
                }
                else if (op2 == "3") {
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
                                } else if (std::stoi(k) > r.getMSize()) {
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
                                } else if (std::stoi(k) > r.getDSize()) {
                                    Menu::numeroMenor();
                                    continue;
                                }
                                break;
                            }
                            r.topK(false, std::stoi(k));
                            Menu::voltar();
                        } else if (op6 == "B" || op6 == "b") break;
                    }
                }
                else if (op2 == "4") {
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
                        } else if (flow == -1.0) continue;
                        else {
                            std::cout << "\nThe maximum number of trains running for " << origin << " is " << flow
                                      << "."
                                      << std::endl;
                        }
                        break;
                    }
                    Menu::voltar();
                }
                else if (op2 == "b" || op == "B") break;
                else Menu::teclaErro();
            }
        }
        else if (op == "3") {
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
                else continue;
                Menu::voltar();
                break;
            }
        }
        else if (op == "4" ) {
            // calcula o fluxo maximo entre todas as estações antes usando o 2.4
            std::vector<std::pair<std::string, double>> maxFlowBefore, maxFlowAfter, affectedStations;
            for (auto &station : r.getVertexSet()) {
                maxFlowBefore.emplace_back(station->getName(), r.maxFlowOrigin(station->getName()));
            }
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
                std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                if (!r.showAdjacents(source)) continue;
                std::cout << "\nEnter the name of the arrival station: ";
                std::getline(std::cin, target);
                if (target.length() < 1) {
                    Menu::teclaErro();
                    continue;
                }
                std::transform(target.begin(), target.end(), target.begin(), ::toupper);

                if (source == target) {
                    Menu::estacoesIguais();
                    continue;
                }
                else if (!r.existStation(target)) {
                    Menu::estacaoNaoEncontrada();
                    continue;
                }
                //exist connection
                exist = r.existConnection(source, target);

                if (exist == 0.0) {
                    Menu::semCaminhoPossivel();
                    continue;
                }
                else {
                    while (true) {
                        std::cout << "\nCapacity: " << exist << " -- " << source << " -> " << target << "\n";
                        std::cout << "\nDo you want to break the edge or decrease the capacity? (B/D): ";
                        std::cin >> res;
                        if (res == "B" || res == "b") {
                            r.decreaseEdge(source, target, 0.0);
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
                while (true) {
                    // ask if it wants to see the repport of affect stations
                    std::cout << "\nDo you want to see the report of affected stations? (Y/N): ";
                    std::cin >> res;
                    if (res == "Y" || res == "y") {
                        // calcular o fluxo maximo de todas as estações
                        for (auto &station: r.getVertexSet()) {
                            maxFlowAfter.emplace_back(station->getName(), r.maxFlowOrigin(station->getName()));
                        }
                        // diferença entre o fluxo maximo antes e depois e guardar no vector affectedStations
                        for (int i = 0; i < maxFlowBefore.size(); i++) {
                            if (maxFlowBefore.at(i).first == maxFlowAfter.at(i).first) {
                                affectedStations.emplace_back(maxFlowBefore.at(i).first,
                                                              maxFlowBefore.at(i).second - maxFlowAfter.at(i).second);
                            }
                        }
                        // sort
                        std::sort(affectedStations.begin(), affectedStations.end(),
                                  [](const std::pair<std::string, double> &a, const std::pair<std::string, double> &b) {
                                      return a.second > b.second;
                                  });
                        // display
                        if (affectedStations.at(0).second != 0) {
                            r.drawReportedStations(affectedStations);
                        }
                        else std::cout << "\nNo stations were affected!" << std::endl;
                        break;
                    } else if (res == "N" || res == "n") break;
                    else {
                        Menu::teclaErro();
                        continue;
                    }
                }
                break;
            }
            Menu::voltar();
        }
        else if (op == "r" || op == "R") {
            r.readStations();
            r.readNetwork();
            std::cout << "\nGraph reseted!" << std::endl;
        }
        else if (op == "q" || op == "Q") {
            Menu::fechouAplicacao();
            break;
        }
        else Menu::teclaErro();
    }
    return 0;
}
