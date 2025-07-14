#include <iostream>
using namespace std;

/*
 * bellman ford is a shortest path algorithm
 * can handle negative edges
 * cannot handle negative cycles
 *
 */
struct edge
{
    int from, to, w;
    edge(int from, int to, int w) : from(from), to(to), w(w)
    {
    }
};

//! basic bellman to find shortest pathes
void Bellman(vector<edge> &edgelist, int n, int from)
{
    vector<int> dist(n + 1, OO);
    dist[from] = 0;
    for (int max_edges = 0; max_edges < n - 1; max_edges++)
    {
        for (auto &[from, to, w] : edgelist)
        {
            if (dist[to] > dist[from] + w)
                dist[to] = dist[from] + w;
        }
    }
}

//! improvement early termination if all edges calculated before n - 1
void Bellman(vector<edge> &edgelist, int n, int from)
{
    vector<int> dist(n + 1, OO);
    dist[from] = 0;
    for (int max_edges = 0, r = 0; max_edges < n - 1; max_edges++, r = 0)
    {
        for (auto &[from, to, w] : edgelist)
        {
            if (dist[to] > dist[from] + w)
                dist[to] = dist[from] + w, r = 1;
        }
        if (!r)
            break;
    }
}

//? how to detect negative cycles
//* if after running n - 1 times run one more time if any edges relaxed then there is a negative cycle
//! improve it to run n times to detect negative cycles in nth iteration
bool Bellman(vector<edge> &edgelist, int n, int from)
{
    vector<int> dist(n + 1, OO);
    dist[from] = 0;
    for (int max_edges = 0, r = 0; max_edges < n; max_edges++, r = 0)
    {
        for (auto &[from, to, w] : edgelist)
        {
            if (dist[to] > dist[from] + w)
            {
                dist[to] = dist[from] + w, r = 1;
                if(max_edges == n - 1)//* then there is a negative cycle
                    return 1;
            }
        }
        if (!r)
            break;
    }
    return 0;
}

//! how to return shortest path nodes
bool Bellman(vector<edge> &edgelist, int n, int from)
{
    vector<int> dist(n + 1, OO);
    vector<int> prev(n + 1, -1);
    dist[from] = 0;
    for (int max_edges = 0, r = 0; max_edges < n; max_edges++, r = 0)
    {
        for (auto &[from, to, w] : edgelist)
        {
            if (dist[to] > dist[from] + w)
            {
                dist[to] = dist[from] + w, r = 1;
                prev[to] = from;
                if(max_edges == n - 1)//* then there is a negative cycle
                    return 1;
            }
        }
        if (!r)
            break;
    }
    // know if a node is in negative cycle
    vector<bool> inCycle(n + 1, 0);
    for (int i = 0; i < n; i++)
    {
        for (auto &[from, to, w] : edgelist)
        {
            if (dist[to] > dist[from] + w)
            {
                dist[to] = dist[from] + w;
                inCycle[to] = 1;
            }
        }
    }
    // return nodes in negative cycle
    vector<int> cycleNodes;
    for (int i = 0; i < n; i++)
    {
        if (inCycle[i])
            cycleNodes.push_back(i);
    }
    // return nodes in shortest path
    vector<int> path;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            path.push_back(i);
            i = prev[i];
            if (i == -1)
                break;
        }
    }
    return 0;
}

//* how to return nodes in negative cycles
//TODO implement

//* how to detect positive cycle
// multiply the graph with -1