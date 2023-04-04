#include "../header/Menu.h"
#include <iostream>

using namespace std;

/**
 * Caso o input do utilizador não esteja entre os aceitados, apresenta uma mensagem de erro.
 */
void Menu::teclaErro() {
    cout << "\n+--------------+\n"
            "|Input Invalido|\n"
            "+--------------+\n";
}

/**
 * Depois do utilizador escolher uma tarefa, aparece a opção de voltar ao menu.
 */
void Menu::voltar() {
    string op;
    cout << "\nPressione [V] para voltar ao Menu:";
    cin >> op;
    while (op != "V" && op != "v") {
        teclaErro();
        cout << "\nPressione [V] para voltar ao Menu:";
        cin >> op;
    }
}

void Menu::aeroportoNaoExiste() {
    cout << "\n+---------------------------------------------------+\n"
            "| Pelo menos um dos aeroportos indicados nao existe |\n"
            "+---------------------------------------------------+\n";
}

void Menu::aeroportoNaoEncontrado() {
    cout << "\n+---------------------------------+\n"
            "| O aeroporto indicado nao existe |\n"
            "+---------------------------------+\n";
}

void Menu::countryNotFound() {
    cout << "\n+----------------------------+\n"
            "| O pais indicado nao existe |\n"
            "+----------------------------+\n";
}

void Menu::airportNotFound() {
    cout << "\n+--------------------------------------------------------+\n"
            "| Nao existe nenhum aeroporto no pais e cidade indicados |\n"
            "+--------------------------------------------------------+\n";
}

void Menu::aeroportosIguais() {
    cout << "\n+------------------------------------+\n"
            "| Os aeroportos inseridos sao iguais |\n"
            "+------------------------------------+\n";
}

void Menu::semCaminhoPossivel() {
    cout << "\n+---------------------------------------------------------------------+\n"
            "| Nao existe nenhum caminho possivel entre os aeroportos selecionados |\n"
            "+---------------------------------------------------------------------+\n";
}

void Menu::companhiaNaoExiste() {
    cout << "\n+---------------------------------+\n"
            "| A companhia indicada nao existe |\n"
            "+---------------------------------+\n";
}

void Menu::caminhoCompanhiaPossivel() {
    cout << "\n+--------------------------------------------------------------------+\n"
            "| Nao existe nenhum trajeto possivel com a/as companhia/s indicada/s |\n"
            "+--------------------------------------------------------------------+\n";
}

void Menu::abriuAplicacao() {
    cout << "\n+---------------------------------------------+\n"
            "|          Muito bem-vindo a aplicacao        |\n"
            "|            de Gestao de Aeroportos          |\n"
            "|               Espero ser util : )           |\n"
            "+---------------------------------------------+\n";
}

void Menu::fechouAplicacao() {
    cout << "\n+---------------------------------------------+\n"
            "|         Muito obrigado por ter usado        |\n"
            "|              a nossa aplicacao!             |\n"
            "|               Volte em breve...             |\n"
            "+---------------------------------------------+\n";
}
