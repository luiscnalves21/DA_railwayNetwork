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

        double edmondsKarp(const std::string &source, const std::string &target);

        double maxEdmondsKarp(std::vector<std::pair<std::string, std::string>> &vertexs);
        double maxEKtopK(const std::vector<std::string> &names);
        void topK(bool mORd, int k);

        double maxFlowOrigin(const std::string &origin);
        double dijkstraShortestPathCost(const std::string &source, const std::string &target, std::vector<std::string> &path, double &minWeigth);
        bool existStation(const std::string &source);
        double existConnection(const std::string &source, const std::string &target) const;
        bool showAdjacents(const std::string& source);

        void decreaseEdge(const std::string &source, const std::string &target, double capacity);

        static std::pair<int, int> auxCenterDraw(int n, bool v);
        static void drawMenu();
        static void drawBasicCostMetrics();
        static void drawListagemMenu();
        static void drawBudgetMenu();

        void drawTopK(const std::string &order, bool header, int &pos, bool mORd) const;
        void drawTopKs(const std::vector<std::pair<std::string, double>> &order, bool mORd, int k) const;
        void drawStationAndLine(const Vertex *vertex, bool header) const;
        void drawStationsAndLine() const;
        void drawConnection(const Edge *edge, bool header) const;
        void drawConnections(const int &sourceId) const;
        void drawReportedStation(const std::pair<std::string, double> &par, bool header) const;
        void drawReportedStations(const std::vector<std::pair<std::string, double>> &affectedStations) const;

        void setMaxStationLength(int maxLength);
        void setMaxMunicipalityLength(int maxLength);
        void setMaxDistrictLength(int maxLength);
        void setMaxLineLength(int maxLength);

        int getMaxStationLength() const;
        int getMaxMunicipalityLength() const;
        int getMaxDistrictLength() const;
        int getMaxLineLength() const;
        int getMSize() const;
        int getDSize() const;
    private:
        Graph railwayNetwork;
        int maxStationLength = 0;
        int maxMunicipalityLength = 0;
        int maxDistrictLength = 0;
        int maxLineLength = 0;
};

#endif
