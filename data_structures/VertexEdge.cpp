#include "VertexEdge.h"

Vertex::Vertex(int id, const std::string &name, const std::string &district, const std::string &municipality, const std::string &township, const std::string &line) {
    this->id = id;
    this->name = name;
    this->district = district;
    this->municipality = municipality;
    this->township = township;
    this->line = line;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
Edge * Vertex::addEdge(Vertex *d, double w, const std::string &service, double cost) {
    auto newEdge = new Edge(this, d, w, service, cost);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
bool Vertex::removeEdge(int destID) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        Vertex *dest = edge->getDest();
        if (dest->getId() == destID) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true;
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
void Vertex::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

int Vertex::getId() const { return this->id; }

std::string Vertex::getName() const { return this->name; }

std::string Vertex::getLine() const { return this->line; }

std::vector<Edge*> Vertex::getAdj() const { return this->adj; }

bool Vertex::isVisited() const { return this->visited; }

double Vertex::getDist() const { return this->dist; }

Edge *Vertex::getPath() const { return this->path; }

std::vector<Edge *> Vertex::getIncoming() const { return this->incoming; }

void Vertex::setVisited(bool visited) { this->visited = visited; }

void Vertex::setDist(double dist) { this->dist = dist; }

void Vertex::setPath(Edge *path) { this->path = path; }

void Vertex::deleteEdge(Edge *edge) {
    Vertex *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getId() == this->id) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

Edge::Edge(Vertex *orig, Vertex *dest, double w, const std::string &service, double cost): orig(orig), dest(dest), weight(w), service(service), cost(cost) {}

Vertex * Edge::getDest() const { return this->dest; }

double Edge::getWeight() const { return this->weight; }

double Edge::getCost() const { return this->cost; }

Vertex * Edge::getOrig() const { return this->orig; }

double Edge::getFlow() const { return this->flow; }

std::string Edge::getService() const { return this->service; }

void Edge::setFlow(double flow) { this->flow = flow; }

void Edge::setWeight(double weight) { this->weight = weight; }
