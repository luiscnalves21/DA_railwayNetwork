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
                else if (op1 == "V" || op1 == "v") break;
            }
        }
        else if (op == "2") {
            std::string source, target;
            bool ignoreCin = true;
            while (true) {
                std::cout << "\nInsere o nome da estacao de partida: ";
                if (ignoreCin) std::cin.ignore();
                ignoreCin = false;
                std::getline(std::cin, source);
                if (source.length() < 1) {
                    Menu::teclaErro();
                    continue;
                }
                std::cout << "\nInsere o nome da estacao de chegada: ";
                std::getline(std::cin, target);
                if (target.length() < 1) {
                    Menu::teclaErro();
                    continue;
                }
                std::transform(source.begin(), source.end(), source.begin(), ::toupper);
                std::transform(target.begin(), target.end(), target.begin(), ::toupper);
                if (!r.edmondsKarp(source, target)) break;
            }
            Menu::voltar();
        }
        else if (op == "3") {
            while (true) {
                GestaoR::drawYMenu();
                std::string op3;
                std::cin >> op3;
                if (op3.length() != 1) {
                    Menu::teclaErro();
                    continue;
                }
                else if (op3 == "1") {
                    Menu::voltar();
                }
                else if (op3 == "2") {
                    Menu::voltar();
                }
                else if (op3 == "3") {
                    Menu::voltar();
                }
                else if (op3 == "V" || op3 == "v") {
                    break;
                }
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
