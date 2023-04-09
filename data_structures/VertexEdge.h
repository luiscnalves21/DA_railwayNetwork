#ifndef VERTEX_EDGE_H
#define VERTEX_EDGE_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "../data_structures/MutablePriorityQueue.h"

class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

class Vertex {
    public:
        Vertex(int id, const std::string &name, const std::string &district, const std::string &municipality, const std::string &township, const std::string &line);
        bool operator<(Vertex & vertex) const;

        int getId() const;
        std::string getName() const;
        std::string getLine() const;
        std::vector<Edge *> getAdj() const;
        bool isVisited() const;
        double getDist() const;
        Edge *getPath() const;
        std::vector<Edge *> getIncoming() const;

        void setVisited(bool visited);
        void setDist(double dist);
        void setPath(Edge *path);
        Edge * addEdge(Vertex *dest, double w, const std::string &service, double cost);
        bool removeEdge(int destID);
        void removeOutgoingEdges();

        friend class MutablePriorityQueue<Vertex>;
    protected:
        int id;
        std::string name;
        std::string district;
        std::string municipality;
        std::string township;
        std::string line;
        std::vector<Edge *> adj;

        bool visited = false;
        double dist = 0;
        Edge *path = nullptr;

        std::vector<Edge *> incoming;

        int queueIndex = 0;

        void deleteEdge(Edge *edge);
};

/********************** Edge  ****************************/

class Edge {
    public:
        Edge(Vertex *orig, Vertex *dest, double w, const std::string &service, double cost);

        Vertex * getDest() const;
        double getWeight() const;
        double getCost() const;
        Vertex * getOrig() const;
        double getFlow() const;
        std::string getService() const;

        void setFlow(double flow);
        void setWeight(double weight);
    protected:
        Vertex * dest;
        double weight;
        double cost;

        std::string service;


        Vertex *orig;

        double flow;
};

#endif
