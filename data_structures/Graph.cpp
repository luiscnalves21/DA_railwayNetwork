#include "Graph.h"

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
Vertex * Graph::findVertexId(const int &id) const {
    for (Vertex *v : vertexSet)
        if (v->getId() == id)
            return v;
    return nullptr;
}

int Graph::findVertexName(const std::string &name) const {
    for (Vertex *v : vertexSet)
        if (v->getName() == name)
            return v->getId();
    return -1;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(const int &id, const std::string &name, const std::string &district,
                      const std::string &municipality, const std::string &township, const std::string &line) {
    if (findVertexName(name) != -1)
        return false;
    vertexSet.push_back(new Vertex(id, name, district, municipality, township, line));
    return true;
}

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

bool Graph::addBidirectionalEdge(const int &sourc, const int &dest, double w, const std::string &service, double cost) {
    auto v1 = findVertexId(sourc);
    auto v2 = findVertexId(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w, service, cost);
    v2->addEdge(v1, w, service, cost);
    return true;
}

void Graph::setVertexSet(const std::vector<Vertex *> &set) { this->vertexSet = set; }

void Graph::setMunicipalities(const std::vector<std::pair<std::string, std::vector<std::string>>> &graphMunicipalities) { this->municipalities = graphMunicipalities; }

void Graph::setDistricts(const std::vector<std::pair<std::string, std::vector<std::string>>> &graphDistricts) { this->districts = graphDistricts; }

std::vector<std::pair<std::string, std::vector<std::string>>> Graph::getMunicipalities() const { return municipalities; }

std::vector<std::pair<std::string, std::vector<std::string>>> Graph::getDistricts() const { return districts; }

int Graph::getMSize() const { return (int)municipalities.size(); }

int Graph::getDSize() const { return (int)districts.size(); }

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

void Graph::testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

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

Graph::~Graph() = default;
