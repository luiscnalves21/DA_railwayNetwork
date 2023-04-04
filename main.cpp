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
            // dependendo do input do utilizador, vai executar tarefas diferentes
        else if (op == "1") {
            while (true) {
                GestaoR::drawListagemMenu();
                std::string op1;
                std::cin >> op1;
                if (op1.length() != 1) {
                    Menu::teclaErro();
                    continue;
                } else if (op1 == "1") {
                    Menu::voltar();
                } else if (op1 == "2") {
                    Menu::voltar();
                } else if (op1 == "3") {
                    Menu::voltar();
                } else if (op1 == "4") {
                    Menu::voltar();
                } else if (op1 == "5") {
                    Menu::voltar();
                } else if (op1 == "6") {
                    Menu::voltar();
                }
                else if (op1 == "V" || op1 == "v") break;
            }
        }
        else if (op == "2") {
            while (true) {
                GestaoR::drawNumberMenu();
                std::string op2;
                std::cin >> op2;
                if (op2.length() != 1) {
                    Menu::teclaErro();
                    continue;
                }
                else if (op2 == "1") {
                    Menu::voltar();
                }
                else if (op2 == "2") {
                    Menu::voltar();
                }
                else if (op2 == "3") {
                    Menu::voltar();
                }
                else if (op2 == "V" || op2 == "v")  {
                    break;
                }
            }
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
