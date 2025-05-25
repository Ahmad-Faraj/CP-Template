template <typename T = int, int Base = 0>
struct Floyd
{

    int n;
    vector<vector<T>> dist;

    Floyd(int _n = 0) : n(_n)
    {
        dist.assign(n + Base, vector<T>(n + Base, INF));
        for (int i = Base; i < n + Base; i++)
            dist[i][i] = 0; // self loops
    }

    Floyd(int _n, const vector<vector<T>> &D) : n(_n), dist(D) { build(); }

    T operation(T a, T b)
    {
        return min(a, b);
    }

    void add_edge(int u, int v, T w)
    {
        dist[u][v] = operation(dist[u][v], w);
    }

    void build()
    {
        for (int i = Base; i < n + Base; i++)
            for (int u = Base; u < n + Base; u++)
                for (int v = Base; v < n + Base; v++)
                    dist[u][v] = operation(dist[u][v], dist[u][i] + dist[i][v]);
    }

    T get_dist(int u, int v)
    {
        return dist[u][v];
    }

    void update_dist(int u, int v, int a, int b)
    {
        dist[u][v] = operation(dist[u][v], dist[u][a] + dist[a][b] + dist[b][v]);
        dist[u][v] = operation(dist[u][v], dist[u][b] + dist[b][a] + dist[a][v]);
    }
};