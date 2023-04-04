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

        std::vector<Vertex *> getVertexSet() const;

        int edmondsKarp(const std::string &source, const std::string &target);

        static void drawMenu();
        static void drawListagemMenu();
        static void drawYMenu();

        void drawStations() const;
        void drawNetwork() const;
    private:
        Graph railwayNetwork;
};

#endif
