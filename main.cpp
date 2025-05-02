#include <iostream>
#include <fstream>
#include <vector>

#include "Graph.hpp"

int main()
{
    Graph myGraph("demoGraph.txt");
    myGraph.print();

    std::vector<unsigned> dist;
    std::vector<int> path;
    myGraph.dijkstra(0, dist, path);
}   