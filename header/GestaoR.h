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

        void combineFA(std::vector<std::pair<std::string, std::string>> &edgesNames, std::vector<Vertex *> &vertexs);
        void dfsFA(std::vector<Edge *> &maxEdges, Vertex *vertex, double max, std::vector<Vertex *> &vertexs);
        void fullAdvantage();

        static std::pair<int, int> auxCenterDraw(int n, bool v);
        static void drawMenu();
        static void drawListagemMenu();

        void drawStations() const;
        void drawNetwork() const;
        void drawStationNetwork(const std::string &name);
        void drawFullAdvantage(std::pair<std::string, std::string> edgeName, bool header) const;
        void drawFullAdvantages(std::vector<std::pair<std::string, std::string>> edgesNames) const;

        void setMaxStationLength(int maxLength);

        int getMaxStationLength() const;
    private:
        Graph railwayNetwork;
        int maxStationLenght = 0;
};

#endif
