#ifndef GRAPH_HEADER
#define GRAPH_HEADER

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <queue>
#include <climits>

using Edge = std::tuple<unsigned, unsigned, unsigned>;

class Graph
{
    public:
        Graph(const char* filename);

        void addEdge(unsigned start, unsigned end, unsigned value);
        void removeEdge(unsigned start, unsigned end, std::vector<Edge>& safe);

        void dijkstra(unsigned start, std::vector<unsigned>& distances, std::vector<int>& path) const;

        void print() const;

    private:
        std::vector<std::vector<std::pair<unsigned, unsigned>>> adjList;
};

#endif