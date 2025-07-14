template <typename T = int>
struct Dijkstra
{

    struct Edge
    {
        T v, w;
        Edge(T V = 0, T W = 0) : v(V), w(W) {}
        bool operator<(const Edge &e) const
        {
            return w > e.w;
        }
    };

    vector<vector<Edge>> adj;

    Dijkstra(int edges, bool indirected = true)
    {
        adj.resize(edges + 10);
        for (int i = 0, u, v, w; i < edges; i++)
        {
            cin >> u >> v >> w;
            adj[u].push_back(Edge(v, w));
            if (indirected)
                adj[v].push_back(Edge(u, w));
        }
    }

    T Min_Cost(int src, int dest)
    {
        int n = adj.size();
        vector<T> dist(n, LLONG_MAX);
        dist[src] = 0;
        priority_queue<Edge> Dij;
        Dij.push(Edge(src, 0));
        while (!Dij.empty())
        {
            Edge top = Dij.top();
            T u = top.v;
            T cost = top.w;
            Dij.pop();
            for (const Edge &edge : adj[u])
            {
                T v = edge.v;
                T w = edge.w;
                if (dist[v] > dist[u] + w)
                {
                    dist[v] = dist[u] + w;
                    Dij.push(Edge(v, dist[v]));
                }
            }
        }
        return (dist[dest] == LLONG_MAX ? -1 : dist[dest]);
    }

    vector<T> get_dist(int src)
    {
        int n = adj.size();
        vector<T> dist(n, LLONG_MAX);
        dist[src] = 0;
        priority_queue<Edge> Dij;
        Dij.push(Edge(src, 0));
        while (!Dij.empty())
        {
            Edge top = Dij.top();
            T u = top.v;
            T cost = top.w;
            Dij.pop();
            for (const Edge &edge : adj[u])
            {
                T v = edge.v;
                T w = edge.w;
                if (dist[v] > dist[u] + w)
                {
                    dist[v] = dist[u] + w;
                    Dij.push(Edge(v, dist[v]));
                }
            }
        }
        return dist;
    }

    vector<T> get_path(int src, int dest)
    {
        int n = adj.size();
        vector<T> dist(n, LLONG_MAX);
        vector<T> parent(n, -1);
        dist[src] = 0;
        priority_queue<Edge> Dij;
        Dij.push(Edge(src, 0));
        while (!Dij.empty())
        {
            Edge top = Dij.top();
            T u = top.v;
            T cost = top.w;
            Dij.pop();
            for (const Edge &edge : adj[u])
            {
                T v = edge.v;
                T w = edge.w;
                if (dist[v] > dist[u] + w)
                {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    Dij.push(Edge(v, dist[v]));
                }
            }
        }
        vector<T> path;
        for (int i = dest; i != -1; i = parent[i])
            path.push_back(i);
        reverse(path.begin(), path.end());
        return path;
    }
};