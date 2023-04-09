#include "VertexEdge.h"

/**
 * @brief Vertex constructor
 * Time Complexity: O(1)
 * @param id
 * @param name
 * @param district
 * @param municipality
 * @param township
 * @param line
 */
Vertex::Vertex(int id, const std::string &name, const std::string &district, const std::string &municipality, const std::string &township, const std::string &line) {
    this->id = id;
    this->name = name;
    this->district = district;
    this->municipality = municipality;
    this->township = township;
    this->line = line;
}

/**
 * @brief Auxiliary function to add an outgoing edge to a vertex (this), with a given destination vertex (d) and edge weight (w).
 * Time Complexity: O(1)
 * @param d
 * @param w
 * @param service
 * @param cost
 * @return the new edge
 */
Edge * Vertex::addEdge(Vertex *d, double w, const std::string &service, double cost) {
    auto newEdge = new Edge(this, d, w, service, cost);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/**
 * @brief Auxiliary function to remove an outgoing edge (with a given destination (d)) from a vertex (this).
 * Time Complexity: O(E) where E is the number of edges
 * @param destID
 * @return true if successful, and false if such edge does not exist.
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

/**
 * @brief Auxiliary function to remove an outgoing edge of a vertex.
 * Time Complexity: O(E^2) where E is the number of edges
 */
void Vertex::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

/**
 * @brief operator <
 * @param vertex
 * @return true if the distance of this vertex is less than the distance of the given vertex
 */
bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

/**
 * @return the id of the vertex
 */
int Vertex::getId() const { return this->id; }

/**
 * @return the name of the vertex
 */
std::string Vertex::getName() const { return this->name; }

/**
 * @return the line of the vertex
 */
std::string Vertex::getLine() const { return this->line; }

/**
 * @return the adjacent edges of the vertex
 */
std::vector<Edge*> Vertex::getAdj() const { return this->adj; }

/**
 * @return the visited status of the vertex
 */
bool Vertex::isVisited() const { return this->visited; }

/**
 * @return the distance of the vertex
 */
double Vertex::getDist() const { return this->dist; }

/**
 * @return the path of the vertex
 */
Edge *Vertex::getPath() const { return this->path; }

/**
 * @return the incoming edges of the vertex
 */
std::vector<Edge *> Vertex::getIncoming() const { return this->incoming; }

/**
 * @brief sets the visited status of the vertex
 * @param visited
 */
void Vertex::setVisited(bool visited) { this->visited = visited; }

/**
 * @brief sets the distance of the vertex
 * @param dist
 */
void Vertex::setDist(double dist) { this->dist = dist; }

/**
 * @brief sets the path of the vertex
 * @param path
 */
void Vertex::setPath(Edge *path) { this->path = path; }


/**
 * @brief Auxiliary function to delete an edge
 * Time Complexity: O(E) where E is the number of edges in the incoming list of the destination vertex
 * @param edge
 */
void Vertex::deleteEdge(Edge *edge) {
    Vertex *dest = edge->getDest();
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

/**
 * @brief Edge constructor
 * @param orig
 * @param dest
 * @param w
 * @param service
 * @param cost
 */
Edge::Edge(Vertex *orig, Vertex *dest, double w, const std::string &service, double cost): orig(orig), dest(dest), weight(w), service(service), cost(cost) {}

/**
 * @return the destination vertex
 */
Vertex * Edge::getDest() const { return this->dest; }

/**
 * @return the weight of the edge
 */
double Edge::getWeight() const { return this->weight; }

/**
 * @return the cost of the edge
 */
double Edge::getCost() const { return this->cost; }

/**
 * @return the origin vertex
 */
Vertex * Edge::getOrig() const { return this->orig; }

/**
 * @return the flow of the edge
 */
double Edge::getFlow() const { return this->flow; }

/**
 * @return the service of the edge
 */
std::string Edge::getService() const { return this->service; }

/**
 * @brief sets the flow of the edge
 * @param flow
 */
void Edge::setFlow(double flow) { this->flow = flow; }

/**
 * @brief sets the weight of the edge
 * @param weight
 */
void Edge::setWeight(double weight) { this->weight = weight; }
