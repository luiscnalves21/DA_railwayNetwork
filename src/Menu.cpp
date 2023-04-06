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

void Menu::estacaoNaoExiste() {
    cout << "\n+---------------------------------------------------+\n"
            "| Pelo menos uma das estacoes indicadas nao existe  |\n"
            "+---------------------------------------------------+\n";
}

void Menu::estacaoNaoEncontrada() {
    cout << "\n+---------------------------------+\n"
            "|  A estacao indicada nao existe  |\n"
            "+---------------------------------+\n";
}

void Menu::estacoesIguais() {
    cout << "\n+------------------------------------+\n"
            "|  As estacoes inseridas sao iguais  |\n"
            "+------------------------------------+\n";
}

void Menu::semCaminhoPossivel() {
    cout << "\n+---------------------------------------------------------------------+\n"
            "|  Nao existe nenhum caminho possivel entre as estacoes selecionadas  |\n"
            "+---------------------------------------------------------------------+\n";
}

void Menu::numeroMenor() {
    cout << "\n+-------------------------+\n"
            "|  Insira um numero menor |\n"
            "+-------------------------+\n";
}

void Menu::abriuAplicacao() {
    cout << "\n+---------------------------------------------+\n"
            "|          Muito bem-vindo a aplicacao        |\n"
            "|             de Gestao Ferroviaria           |\n"
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
