#include "../header/Menu.h"
#include <iostream>

using namespace std;

/**
 * If the user enters an invalid input, this function is called
 * Time Complexity: O(1)
*/
void Menu::teclaErro() {
    cout << "\n+---------------+\n"
            "| Invalid Input |\n"
            "+---------------+\n";
}

/**
 * Every time the user have the option to return to the previous menu, this function is called
 * Time Complexity: O(n) where n is the number of iterations required to get a valid input from the user
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

/**
 * This function is called when the user enters at least an invalid station
 * Time Complexity: O(1)
*/
void Menu::estacaoNaoExiste() {
    cout << "\n+-------------------------------------------------------+\n"
            "|  At least one of the entered stations does not exist  |\n"
            "+-------------------------------------------------------+\n";
}

/**
 * This function is called when the user enters an invalid station
 * Time Complexity: O(1)
*/
void Menu::estacaoNaoEncontrada() {
    cout << "\n+----------------------------------------+\n"
            "|   The entered station does not exist   |\n"
            "+----------------------------------------+\n";
}

/**
 * This function is called when the user enters the same station twice
 * Time Complexity: O(1)
*/
void Menu::estacoesIguais() {
    cout << "\n+-------------------------------------+\n"
            "|  The stations entered are the same  |\n"
            "+-------------------------------------+\n";
}

/**
 * This function is called when the user enters stations that are not connected
 * Time Complexity: O(1)
*/
void Menu::semCaminhoPossivel() {
    cout << "\n+-----------------------------------------------------------+\n"
            "|  There is no possible path between the selected stations  |\n"
            "+-----------------------------------------------------------+\n";
}

/**
 * This function is called when the user enters a station that is not connected
 * Time Complexity: O(1)
*/
void Menu::semCaminhoEstacao() {
    cout << "\n+-----------------------------------------------------+\n"
            "|  There is no possible path to the selected station  |\n"
            "+-----------------------------------------------------+\n";
}

/**
 * This function is called when the user enters a number that is not in the range
 * Time Complexity: O(1)
*/
void Menu::numeroMenor() {
    cout << "\n+--------------------------+\n"
            "|  Enter a smaller number  |\n"
            "+--------------------------+\n";
}

/**
 * This function is called at the beginning of the application
 * Time Complexity: O(1)
*/
void Menu::abriuAplicacao() {
    cout << "\n+-------------------------------------------------------------+\n"
            "|                       Very welcome to                       |\n"
            "|                Railway Management application               |\n"
            "|                    I hope to be useful : )                  |\n"
            "+-------------------------------------------------------------+\n";
}

/**
 * This function is called at the end of the application
 * Time Complexity: O(1)
*/
void Menu::fechouAplicacao() {
    cout << "\n+-------------------------------------------------------------+\n"
            "|                 Thank you very much for using               |\n"
            "|                        our application!                     |\n"
            "|                        Come back soon...                    |\n"
            "+-------------------------------------------------------------+\n";
}
