#include "Graph.hpp"

Graph::Graph(const char* filename)
{
    std::ifstream input(filename);
    if (!input.is_open()) 
        throw std::invalid_argument("File cannot be opened!");

    int verticesCount;
    input >> verticesCount;
    this->adjList.resize(verticesCount);

    unsigned start, end, value;
    char buffer;
    while (input >> buffer >> start >> end >> value >> buffer)
    {
        this->addEdge(start, end, value);
    }
    if (!input.eof())
        throw std::invalid_argument("Ended reading file early!");

    input.close();
}

void Graph::addEdge(unsigned start, unsigned end, unsigned value)
{
    if (start >= this->adjList.size() || end >= this->adjList.size())
        throw std::out_of_range("Invalid node! .addEdge failed!");

    // Checking if there is already a node with that cordinates
    for (int i = 0; i < this->adjList[start].size(); i++)
    {
        if (this->adjList[start][i].first == end)
            throw std::invalid_argument("There is already a node with that cordinates!");
    }
    this->adjList[start].push_back(std::make_pair(end, value));
}

void Graph::removeEdge(unsigned start, unsigned end, std::vector<Edge>& safe)
{
    if (start >= this->adjList.size() || end >= this->adjList.size()) return;

    for (int i = 0; i < this->adjList[start].size(); i++)
    {
        if (this->adjList[start][i].first == end)
        {
            safe.push_back(Edge(start, end, this->adjList[start][i].second));
            this->adjList[start].erase(this->adjList[start].begin() + i);
        }
    }
}

void Graph::reconstructPath(unsigned end, Path& paths, Path& smallest) const
{
    while (paths[end] != -1)
    {
        smallest.push_back(end);
        end = paths[end];
    }
    smallest.push_back(end);
}

void Graph::dijkstra(unsigned start, unsigned end, Path& smallest) const
{
    std::vector<unsigned> distances(this->adjList.size(), UINT_MAX);
    Path paths(this->adjList.size(), -1);

    distances[start] = 0;

    auto comparator = [&distances](const unsigned& lhs, const unsigned& rhs) 
    {
        return distances[lhs] > distances[rhs];
    };
    std::priority_queue<unsigned, std::vector<unsigned>, decltype(comparator)> q(comparator);

    q.push(start);

    unsigned jump;
    while (!q.empty())
    {
	    unsigned currVertex = q.top();
	    q.pop();

        for (const auto& neighbor : this->adjList[currVertex])
        {
            jump = distances[currVertex] + neighbor.second;

            if (distances[neighbor.first] > jump)
            {
                distances[neighbor.first] = jump;
                paths[neighbor.first] = currVertex;
                q.push(neighbor.first);
            }
        }
    }

    if (distances[end] == UINT_MAX) return;

    if (!smallest.empty()) smallest.clear();
    this->reconstructPath(end, paths, smallest);
    std::reverse(smallest.begin(), smallest.end());
}

std::vector<Path> Graph::yenKSP(unsigned start, unsigned end, unsigned Kst)
{

    std::vector<Path> A;
    Path smallestPath;

    // Determine the shortest path from the source to the sink.
    this->dijkstra(start, end, smallestPath);
    A.push_back(smallestPath);

    // Initialize the set to store the potential kth shortest path.
    std::vector<Path> B;

    for (int k = 1; k < Kst; k++)
    {
        Path& previousPath = A[k-1];

        // The spur node ranges from the first node to the next to last node in the previous k-shortest path.
        for (int i = 0; i < A[k - 1].size() - 1; i++)
        {
            // Spur node is retrieved from the previous k-shortest path, k − 1.
            unsigned spurNode = previousPath[i];

            // The sequence of nodes from the source to the spur node of the previous k-shortest path.
            Path rootPath(previousPath.begin(), previousPath.begin() + i + 1);

            std::vector<Edge> removedEdges;

            for (const auto& path : A)
            {
                if (path.size() > i && std::equal(rootPath.begin(), rootPath.end() - 1, path.begin()))
                {
                    // Remove the links that are part of the previous shortest paths which share the same root path.
                    this->removeEdge(path[i], path[i + 1], removedEdges);
                }
            }

            // Remove root path nodes (except spur node) to prevent revisiting
            int idx;
            for (int i = 0; i < rootPath.size() - 1; i++) 
            {
                idx = rootPath[i];
                while (!this->adjList[idx].empty())
                {
                    this->removeEdge(idx, this->adjList[idx][0].first, removedEdges);
                }
            }

            // Calculate the spur path from the spur node to the sink.
            // Consider also checking if any spurPath found
            Path spurPath;
            this->dijkstra(spurNode, end, spurPath);

            // Add back the edges and nodes that were removed from the graph.
            for (const auto& edge : removedEdges)
            {
                this->addEdge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
            }

            if (!spurPath.empty())
            {
                // Entire path is made up of the root path and spur path.
                Path totalPath = rootPath;
                if (totalPath.back() == spurPath.front()) totalPath.pop_back();
                totalPath.insert(totalPath.end(), spurPath.begin(), spurPath.end());

                // Add the potential k-shortest path to the heap.
                bool dublicate = false;
                for (const auto& path : A)
                {
                    if (path == totalPath)
                    {
                        dublicate = true;
                        break;
                    }
                }
                if (!dublicate)
                {
                    for (const auto& path : B)
                    {
                        if (path == totalPath)
                        {
                            dublicate = true;
                            break;
                        }
                    }
                }
                if (!dublicate) B.push_back(totalPath);
            }
        }

        if (B.empty())
        {
            // This handles the case of there being no spur paths, or no spur paths left.
            // This could happen if the spur paths have already been exhausted (added to A), 
            // or there are no spur paths at all - such as when both the source and sink vertices 
            // lie along a "dead end".
            break;
        }
        // Sort the potential k-shortest paths by cost.
        auto sorter = [this](const Path &first, const Path &second)
        {
            return this->getValuePath(first) < this->getValuePath(second);
        };
        std::sort(B.begin(), B.end(), sorter);
        // Add the lowest cost path becomes the k-shortest path.
        A.push_back(B.front());
        // Removing the first element.
        B.erase(B.begin());
    }

    return A;
}

void Graph::print() const
{
    for (int i = 0; i < this->adjList.size(); i++)
    {
        for (int j = 0; j < this->adjList[i].size(); j++)
        {
            std::cout << '(' << i << ' ' << adjList[i][j].first << ' ' << adjList[i][j].second << ')';
        }
        std::cout << std::endl;
    }
}

unsigned Graph::getValueOfEdge(unsigned start, unsigned end) const
{
    if (start >= this->adjList.size() || end >= this->adjList.size())
        throw std::out_of_range("Invalid node! .getValueOfEdge failed!");
    
    for (int i = 0; i < this->adjList[start].size(); i++)
    {
        if (this->adjList[start][i].first == end) return this->adjList[start][i].second;
    }
}

unsigned Graph::getValuePath(const Path& path) const
{
    unsigned result = 0;
    for (int i = 0; i < path.size() - 1; i++)
    {
        result += this->getValueOfEdge(path[i], path[i + 1]);
    }
    return result;
}