#ifndef GESTAOA_H
#define GESTAOA_H

#include <string>
#include <vector>
#include <set>
#include <stack>

#include "../data_structures/Graph.h"

class GestaoR {
    public:
        GestaoR();

        void readStations();
        void readNetwork();

        static void drawMenu();
        static void drawListagemMenu();
        static void drawNumberMenu();
        static void drawYMenu();
    private:
        Graph railwayNetwork;
};

#endif
