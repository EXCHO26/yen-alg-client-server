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
        throw std::out_of_range("Invalid node!");

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
    if (start >= this->adjList.size() || end >= this->adjList.size())
        throw std::out_of_range("Invalid node!");

    for (int i = 0; i < this->adjList[start].size(); i++)
    {
        if (this->adjList[start][i].first == end)
        {
            safe.push_back(Edge(start, end, this->adjList[start][i].second));
            this->adjList[start].erase(this->adjList[start].begin() + i);
        }
    }
}

void Graph::dijkstra(unsigned start, std::vector<unsigned>& distances, std::vector<int>& path) const
{
    distances.resize(this->adjList.size(), UINT_MAX);
    path.resize(this->adjList.size(), -1);
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
                path[neighbor.first] = currVertex;
                q.push(neighbor.first);
            }
        }
    }
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