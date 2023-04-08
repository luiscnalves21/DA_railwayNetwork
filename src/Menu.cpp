#include "../header/Menu.h"
#include <iostream>

using namespace std;

/**
 * Caso o input do utilizador não esteja entre os aceitados, apresenta uma mensagem de erro.
 */
void Menu::teclaErro() {
    cout << "\n+---------------+\n"
            "| Invalid Input |\n"
            "+---------------+\n";
}

/**
 * Depois do utilizador escolher uma tarefa, aparece a opção de voltar ao menu.
 */
void Menu::voltar() {
    string op;
    cout << "\nPress [B] to return to the Menu:";
    cin >> op;
    while (op != "B" && op != "b") {
        teclaErro();
        cout << "\nPress [B] to return to the Menu:";
        cin >> op;
    }
}

void Menu::estacaoNaoExiste() {
    cout << "\n+-------------------------------------------------------+\n"
            "|  At least one of the entered stations does not exist  |\n"
            "+-------------------------------------------------------+\n";
}

void Menu::estacaoNaoEncontrada() {
    cout << "\n+----------------------------------------+\n"
            "|   The entered station does not exist   |\n"
            "+----------------------------------------+\n";
}

void Menu::estacoesIguais() {
    cout << "\n+-------------------------------------+\n"
            "|  The stations entered are the same  |\n"
            "+-------------------------------------+\n";
}

void Menu::semCaminhoPossivel() {
    cout << "\n+-----------------------------------------------------------+\n"
            "|  There is no possible path between the selected stations  |\n"
            "+-----------------------------------------------------------+\n";
}

void Menu::semCaminhoEstacao() {
    cout << "\n+-----------------------------------------------------+\n"
            "|  There is no possible path to the selected station  |\n"
            "+-----------------------------------------------------+\n";
}

void Menu::numeroMenor() {
    cout << "\n+--------------------------+\n"
            "|  Enter a smaller number  |\n"
            "+--------------------------+\n";
}

void Menu::abriuAplicacao() {
    cout << "\n+-------------------------------------------------------------+\n"
            "|                       Very welcome to                       |\n"
            "|                Railway Management application               |\n"
            "|                    I hope to be useful : )                  |\n"
            "+-------------------------------------------------------------+\n";
}

void Menu::fechouAplicacao() {
    cout << "\n+-------------------------------------------------------------+\n"
            "|                 Thank you very much for using               |\n"
            "|                        our application!                     |\n"
            "|                        Come back soon...                    |\n"
            "+-------------------------------------------------------------+\n";
}
