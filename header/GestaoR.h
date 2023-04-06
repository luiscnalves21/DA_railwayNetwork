#ifndef GESTAOA_H
#define GESTAOA_H

#include <string>
#include <vector>
#include <set>
#include <stack>

#include "../data_structures/Graph.h"

class GestaoR {
    public:
        struct Municipality {
            std::string municipality;
            double numberOfTrains = 0.0;
            int numberOfStations = 0;
        };

        GestaoR();

        void readStations();
        void readNetwork();

        std::vector<Vertex *> getVertexSet() const;

        double edmondsKarp(const std::string &source, const std::string &target);

        std::pair<std::string, std::string> maxEdmondsKarp();
        double maxEKtopK(const std::vector<std::string> &names);
        void topK(bool mORd);
        void combineFA(std::vector<std::pair<std::string, std::string>> &edgesNames, std::vector<Vertex *> &vertexs);
        void dfsFA(std::vector<Edge *> &maxEdges, Vertex *vertex, double max, std::vector<Vertex *> &vertexs);
        void fullAdvantage();
        void dfsMR(Vertex *vertex, Municipality &municipality);
        std::vector<Municipality> managementRailway();

        static std::pair<int, int> auxCenterDraw(int n, bool v);
        static void drawMenu();
        static void drawListagemMenu();
        static void drawBudgetMenu();

        void drawStation(const Vertex *vertex, bool header) const;
        void drawStations() const;
        void drawNetwork() const;
        void drawStationNetwork(const std::string &name);
        void drawFullAdvantage(const std::pair<std::string, std::string> &edgeName, bool header) const;
        void drawFullAdvantages(const std::vector<std::pair<std::string, std::string>> &edgesNames) const;

        void setMaxStationLength(int maxLength);
        void setMaxLineLength(int maxLength);

        int getMaxStationLength() const;
        int getMaxLineLength() const;
private:
        Graph railwayNetwork;
        int maxStationLength = 0;
        int maxLineLength = 0;
};

#endif
