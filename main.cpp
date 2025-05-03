#include <iostream>
#include <fstream>
#include <vector>

#include "Graph.hpp"

int main()
{
    Graph myGraph("demoGraph.txt");

    std::vector<Path> A = myGraph.yenKSP(0, 3, 15);

    for (const auto& path : A)
    {
        for (const auto& edge : path)
        {
            std::cout << edge << ' ';
        }
        std::cout << "Path value: " << myGraph.getValuePath(path);
        std::cout << std::endl;
    }
}   
