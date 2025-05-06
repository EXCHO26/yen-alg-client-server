#ifndef GRAPH_HEADER
#define GRAPH_HEADER

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <queue>
#include <climits>
#include <algorithm>

using Edge = std::tuple<unsigned, unsigned, unsigned>;
using Path = std::vector<unsigned>;

class Graph
{
    public:
        Graph(const char* filename);

        void addEdge(unsigned start, unsigned end, unsigned value);
        void removeEdge(unsigned start, unsigned end, std::vector<Edge>& safe);

        void dijkstra(unsigned start, unsigned end, Path& smallest) const;
        Path yenKSP(unsigned start, unsigned end, unsigned Kst);

        void print() const;

        unsigned getValueOfEdge(unsigned start, unsigned end) const;
        unsigned getValuePath(const Path& path) const;

    private:
        void reconstructPath(unsigned end, Path& paths, Path& smallest) const;
        void removeNode(unsigned node);
        //unsigned getValuePath(Path& path) const;

        std::vector<std::vector<std::pair<unsigned, unsigned>>> adjList;
};

#endif