#include <algorithm>
#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <functional>
#include <vector>
#include "graph.h"

using namespace std;

// Graph constructor
Graph::Graph(bool directionalEdges) {
  directional = directionalEdges;
}

// Graph destructor
Graph::~Graph() {
    for (set<vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) { //NOLINT
        for (vector<edge*>::iterator it2 = (*it)->edges.begin(); it2 != (*it)->edges.end(); ++it2) { //NOLINT
            delete (*it2);
        }
        delete (*it);
    }
}

// returns number of vertices in graph
int Graph::NumberOfVertices() const {
  return (vertices.size());
}

// returns number of edges in graph
int Graph::NumberOfEdges() const {
  int numberOfEdges = 0;
  for (set<vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) { //NOLINT
    numberOfEdges += (*it)->edges.size();
  }
  return numberOfEdges;
}

// returns number of edges connected to a specific vertex
int Graph::NumberOfEdges(const string &label) const {
  if (HasVertex(label)) {
    for (set<vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) { //NOLINT
      if ((*it)->label == label) {
        return ((*it)->edges.size());
      }
    }
    return -1;
  } else {
      return -1;
  }
}

// adds a vertex with specified label to the graph
bool Graph::AddVertex(const string &label) {
  if (HasVertex(label)) {
      return false;
  } else {
      vertex *temp = new vertex;
      temp->label = label;
      vertices.insert(temp);
      return true;
  }
}

/** return true if vertex already in graph */
bool Graph::HasVertex(const std::string &label) const {
  for (set<vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) { //NOLINT
    if ((*it)->label == label) {
      return true;
    }
  }
  return false;
}

// returns a string in format label1(weight1) label2(weight2)
string Graph::GetEdges(const string &label) const {
    string temp;
    for (set<vertex *>::iterator it = vertices.begin(); it != vertices.end(); ++it) { //NOLINT
        if ((*it)->label == label) {  // TODO(me) figure out how to sort this
            for (int i = 0; i < (*it)->edges.size(); i++) {
                temp += (*it)->edges[i]->toVertex->label;
                temp += "(";
                temp += to_string((*it)->edges[i]->weight);
                temp += ") ";
            }
        }
    }
    temp.pop_back();
    return temp;
}

// connects 2 vertices in the graph, returning false if unable
bool Graph::Connect(const string &fromLabel,
                    const string &toLabel, int edgeWeight) {
  if (!HasVertex(fromLabel))
      AddVertex(fromLabel);
  if (!HasVertex(toLabel))
      AddVertex(toLabel);
  if (fromLabel == toLabel)
      return false;
  vertex* fromVertex = findVertex(fromLabel);
  vertex* toVertex = findVertex(toLabel);
  for (vector<edge*>::iterator it = fromVertex->edges.begin(); it != fromVertex->edges.end(); ++it) { //NOLINT
      if ((*it)->toVertex == toVertex) {
          return false;
      }
  }
  edge* temp = new edge;
  temp->toVertex = toVertex;
  temp->weight = edgeWeight;
  for (set<vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) { //NOLINT
      if ((*it)->label == fromLabel) {
          (*it)->edges.push_back(temp);
      }
  }
  return true;
}

// private function returning a vertex pointer to specified label
Graph::vertex* Graph::findVertex(const string &label) const {
    for (set<vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) { //NOLINT
        if ((*it)->label == label) {
            return (*it);
        }
    }
    return nullptr;
}

//  removes an edge between two specified vertices, returning false if unable
bool Graph::Disconnect(const string &fromLabel, const string &toLabel) const {
  vertex* fromVertex = findVertex(fromLabel);
  vertex* toVertex = findVertex(toLabel);
  if (fromVertex == nullptr)
      return false;
  for (vector<edge*>::iterator it = fromVertex->edges.begin(); it != fromVertex->edges.end(); ++it) { //NOLINT
      if ((*it)->toVertex == toVertex) {
          fromVertex->edges.erase(it);
          delete (*it);
          return true;
      }
  }
  return false;
}

// depth first search
void Graph::DFS(const string &startLabel, void visit(const string &)) {
  vector<vertex*> visited;
  vertex* startNode = findVertex(startLabel);
  dfs(startNode, visited, visit);
}

// recursive depth first search helper function
void Graph::dfs(vertex* currentNode, vector<vertex*> visited, void visit(const string &)) { //NOLINT
    // TODO(me) check if each node has been visited?
    visit(currentNode->label);
    visited.push_back(currentNode);
    for (int i = 0; i < currentNode->edges.size(); i++) {
        // TODO(me) visit vertices in a weighted order
        dfs(currentNode->edges[i]->toVertex, visited, visit);
    }
}

// breadth first search
void Graph::BFS(const string &startLabel, void visit(const string &)) const {
    vector<vertex*> visited;
    vertex* currentNode = findVertex(startLabel);
    visit(startLabel);
    visited.push_back(currentNode);
    queue<vertex*> q;
    q.push(currentNode);
    while (!q.empty()) {
        currentNode = q.front();
        q.pop();
        for (int i = 0; i < currentNode->edges.size(); i++) {
            bool isVisited = false;
            for (vector<vertex*>::iterator it = visited.begin(); it != visited.end(); ++it) { //NOLINT
                if (currentNode->edges[i]->toVertex == (*it)) {
                    isVisited = true;
                }
            }
            if (!isVisited) {
                visit(currentNode->edges[i]->toVertex->label);
                visited.push_back(currentNode->edges[i]->toVertex);
                q.push(currentNode->edges[i]->toVertex);
            }
        }
    }
}

// store the weights in a map
// store the previous label in a map
void Graph::Dijkstra(const string &startLabel,
                     map<string, int> &weights,
                     map<string, string> &previous) const {
  for (set<vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) { //NOLINT
      weights[(*it)->label] = INT_MAX;  // initialize all weights to inf
  }
  vector<vertex*> vertexSet;
  vertex* currentVertex = findVertex(startLabel);
  vertexSet.push_back(currentVertex);
  vertex* closestVertex;
  while (vertexSet.size() < NumberOfVertices()) {
      for (vector<edge *>::iterator it = closestVertex->edges.begin(); it != closestVertex->edges.end(); ++it) { //NOLINT
          bool inVertexSet = false;
          for (vector<vertex *>::iterator it2 = vertexSet.begin(); it2 != vertexSet.end(); ++it2) { //NOLINT
              if ((*it2)->label == (*it)->toVertex->label) {
                  inVertexSet = true;
              }
          }
          if (!inVertexSet && (*it)->weight < weights[(*it)->toVertex->label]) {
              closestVertex = (*it)->toVertex;
              weights[(*it)->toVertex->label] = (*it)->weight;
              previous[(*it)->toVertex->label] = closestVertex->label;
          }
          vertexSet.push_back(closestVertex);
      }
  }

  /* vector<int> distances(NumberOfVertices(), INT_MAX);
    for (set<vertex*>::iterator it = vertices.begin(); it != vertices.end(); it++) {
        weights[(*it)->label] = INT_MAX;  // initialize all weights to inf
  }
  map<string, int> tempWeights;
  tempWeights.insert(pair<string, int>(startLabel, 0));
  weights[startLabel] = 0;  // starting node weight is 0
  while (!tempWeights.empty()) {
      pair<string,int> temp = *(tempWeights.begin()); // temp should be nearest vertex(!)
      tempWeights.erase(tempWeights.begin());
      // iterate through all the edges
      vertex* fromVertex = findVertex(temp.first);
      for (vector<edge*>::iterator it = fromVertex->edges.begin(); it != fromVertex->edges.end(); it++) {
          string toLabel = (*it)->toVertex->label;
          int weight = (*it)->weight;
          if (weights[toLabel] > weights[fromVertex->label] + weight) {  // is there a shorter path?
              if (weights[toLabel] != INT_MAX) {
                  tempWeights.erase(weights[toLabel])
              }
          }
      }
  } */
}

/**
 * minimum spanning tree
 * @param function to be called on each edge
 * @return length of the minimum spanning tree or -1 if start vertex not found
 */
int Graph::MST(const string &startLabel,
               void visit(const string &from,
                          const string &to, int weight)) const {
  // TODO(me)
  return 0;
}

// reads a specified file as a graph
bool Graph::ReadFile(const string &filename) {
  ifstream myfile(filename);
  if (!myfile.is_open()) {
    cerr << "Failed to open " << filename << endl;
    return false;
  }
  int edges, weight;
  string fromVertex, toVertex;
  myfile >> edges;
  for (int i = 0; i < edges; ++i) {
    myfile >> fromVertex >> toVertex >> weight;
    Connect(fromVertex, toVertex, weight);
  }
  myfile.close();
  return true;
}

