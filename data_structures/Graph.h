#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "../data_structures/MutablePriorityQueue.h"

#include "VertexEdge.h"

class Graph {
    public:
        ~Graph();
        /*
        * Auxiliary function to find a vertex with a given ID.
        */
        Vertex *findVertexId(const int &id) const;
        /*
        * Auxiliary function to find the ID of a vertex with a given NAME.
        */
        int findVertexName(const std::string &name) const;
        /*
         *  Adds a vertex with a given content or info (in) to a graph (this).
         *  Returns true if successful, and false if a vertex with that content already exists.
         */
        bool addVertex(const int &id, const std::string &name, const std::string &district, const std::string &municipality, const std::string &township, const std::string &line);
        bool removeEdge(const int &source, const int &dest);
        bool removeVertex(const int &id);
        /*
         * Adds an edge to a graph (this), given the contents of the source and
         * destination vertices and the edge weight (w).
         * Returns true if successful, and false if the source or destination vertex does not exist.
         */
        bool addEdge(const int &sourc, const int &dest, double w, const std::string &service);
        bool addBidirectionalEdge(const int &sourc, const int &dest, double w, const std::string &service);

        void edmondsKarp(int source, int target);

        void setMunicipalities(const std::vector<std::pair<std::string, std::vector<std::string>>> &graphMunicipalities);
        void setDistricts(const std::vector<std::pair<std::string, std::vector<std::string>>> &graphDistricts);

        int getNumVertex() const;
        std::vector<Vertex *> getVertexSet() const;
        std::vector<std::pair<std::string, std::vector<std::string>>> getMunicipalities() const;
        std::vector<std::pair<std::string, std::vector<std::string>>> getDistricts() const;
        int getMSize() const;
        int getDSize() const;
    protected:
        std::vector<Vertex *> vertexSet;    // vertex set
        std::vector<std::pair<std::string, std::vector<std::string>>> municipalities;
        std::vector<std::pair<std::string, std::vector<std::string>>> districts;

        void testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual);
        bool findAugmentingPath(Vertex *s, Vertex *t);
        double findMinResidualAlongPath(Vertex *s, Vertex *t);
        void augmentFlowAlongPath(Vertex *s, Vertex *t, double f);

        double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
        int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

        /*
         * Finds the index of the vertex with a given content.
         */
        int findVertexIdx(const int &id) const;
};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);

#endif