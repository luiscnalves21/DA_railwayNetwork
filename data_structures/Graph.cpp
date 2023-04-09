#include "Graph.h"

/**
 * @return the vertexSet of stations
 */
std::vector<Vertex *> Graph::getVertexSet() const { return vertexSet; }

/**
 * Finds a vertex with a given id in the graph
 * Time Complexity: O(V) where V is the number of vertices
 * @param id
 * @return the vertex with the given id or nullptr if it doesn't exist
 */
Vertex * Graph::findVertexId(const int &id) const {
    for (Vertex *v : vertexSet)
        if (v->getId() == id)
            return v;
    return nullptr;
}

/**
 * Finds a vertex with a given name in the graph
 * Time Complexity: O(V) where V is the number of vertices
 * @param name
 * @return the vertex id with the given name or -1 if it doesn't exist
 */
int Graph::findVertexName(const std::string &name) const {
    for (Vertex *v : vertexSet)
        if (v->getName() == name)
            return v->getId();
    return -1;
}

/**
 * Adds a vertex to the graph
 * Time Complexity: O(V) where V is the number of vertices
 * @param id
 * @param name
 * @param district
 * @param municipality
 * @param township
 * @param line
 * @return true if the vertex was added, false otherwise
 */
bool Graph::addVertex(const int &id, const std::string &name, const std::string &district,
                      const std::string &municipality, const std::string &township, const std::string &line) {
    if (findVertexName(name) != -1)
        return false;
    vertexSet.push_back(new Vertex(id, name, district, municipality, township, line));
    return true;
}


/**
 * Removes a vertex from the graph
 * Time Complexity: O(V + E) where V is the number of vertices and E is the number of edges
 * @param id
 * @return true if the vertex was removed, false otherwise
 */
bool Graph::removeVertex(const int &id) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getId() == id) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getId());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/**
 * Adds a bidirectional edge to the graph
 * Time Complexity: O(1)
 * @param sourc
 * @param dest
 * @param w
 * @param service
 * @param cost
 * @return true if the edge was added, false otherwise
 */
bool Graph::addBidirectionalEdge(const int &sourc, const int &dest, double w, const std::string &service, double cost) {
    auto v1 = findVertexId(sourc);
    auto v2 = findVertexId(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w, service, cost);
    v2->addEdge(v1, w, service, cost);
    return true;
}

/**
 * sets the vertexSet of stations
 * @param set
 */
void Graph::setVertexSet(const std::vector<Vertex *> &set) { this->vertexSet = set; }

/**
 * sets the graphMunicipalities
 * @param graphMunicipalities
 */
void Graph::setMunicipalities(const std::vector<std::pair<std::string, std::vector<std::string>>> &graphMunicipalities) { this->municipalities = graphMunicipalities; }

/**
 * sets the graphDistricts
 * @param graphDistricts
 */
void Graph::setDistricts(const std::vector<std::pair<std::string, std::vector<std::string>>> &graphDistricts) { this->districts = graphDistricts; }

/**
 * @return the graphMunicipalities
 */
std::vector<std::pair<std::string, std::vector<std::string>>> Graph::getMunicipalities() const { return municipalities; }

/**
 * @return the graphDistricts
 */
std::vector<std::pair<std::string, std::vector<std::string>>> Graph::getDistricts() const { return districts; }

/**
 * @return the number of municipalities
 */
int Graph::getMSize() const { return (int)municipalities.size(); }

/**
 * @return the number of districts
 */
int Graph::getDSize() const { return (int)districts.size(); }

/**
 * Finds the shortest path to all vertices from a given source using Dijkstra's algorithm
 * Time Complexity: O(E log V) where E is the number of edges and V is the number of vertices
 * @param s
 */
void Graph::dijkstraShortestPath(const int &s) {
    Vertex * src = findVertexId(s);
    if (src == nullptr) return;

    for (Vertex *vertex : vertexSet) {
        vertex->setDist(INF);
        vertex->setPath(nullptr);
        vertex->setVisited(false);
    }

    MutablePriorityQueue<Vertex> q;
    src->setDist(0);
    q.insert(src);
    for (Vertex *vertex : vertexSet)
        if (vertex->getName() != src->getName())
            q.insert(vertex);

    while(!q.empty() ) {
        Vertex *v = q.extractMin();
        v->setVisited(true);
        for (Edge *edge : v->getAdj()) {
            if (!edge->getDest()->isVisited() && edge->getDest()->getDist() > (v->getDist() + edge->getCost())) {
                edge->getDest()->setDist(v->getDist() + edge->getCost());
                edge->getDest()->setPath(edge);
                q.decreaseKey(edge->getDest());
            }
        }
    }
}

/**
 * Calculates the max-flow of a graph using the Edmonds-Karp algorithm
 * Time Complexity: O(V E^2) where V is the number of vertices and E is the number of edges
 * @param source
 * @param target
 */
void Graph::edmondsKarp(int source, int target) {
    Vertex* s = findVertexId(source);
    Vertex* t = findVertexId(target);
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");

    for (auto v : vertexSet) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
    while( findAugmentingPath(s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
}

/**
 * Checks if a vertex is visited and if the residual is greater than 0
 * Time Complexity: O(1)
 * @param q
 * @param e
 * @param w
 * @param residual
 */
void Graph::testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

/**
 * performs a breadth-first search on the graph to find a path from the source to the target that has residual capacity
 * Time Complexity: O(V + E) where V is the number of vertices and E is the number of edges
 * @param s
 * @param t
 * @return true if the vertex was visited, false otherwise
 */
bool Graph::findAugmentingPath(Vertex *s, Vertex *t) {
    for(auto v : vertexSet) {
        v->setVisited(false);
    }
    s->setVisited(true);
    std::queue<Vertex *> q;
    q.push(s);
    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
    return t->isVisited();
}

/**
 * finds the minimum residual capacity along a given path
 * Time Complexity: O(V) where V is the number of vertices
 * @param s
 * @param t
 * @return the minimum residual capacity
 */
double Graph::findMinResidualAlongPath(Vertex *s, Vertex *t) {
    double f = INF;
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
    return f;
}

/**
 * updates the flow along a given path by adding the minimum residual capacity to each edge
 * Time Complexity: O(V) where V is the number of vertices
 * @param s
 * @param t
 * @param f
 */
void Graph::augmentFlowAlongPath(Vertex *s, Vertex *t, double f) {
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

/**
 * Default destructor
 */
Graph::~Graph() = default;
