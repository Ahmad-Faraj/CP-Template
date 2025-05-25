template <typename T = int, int Base = 1>
struct DSU
{

    vector<T> parent, Gsize;
    int NumberOfComponents;
    stack<array<int, 5>> stk;

    DSU() {}

    DSU(int MaxNodes)
    {
        parent = Gsize = vector<T>(MaxNodes + Base);
        for (int i = Base; i < MaxNodes + Base; i++)
        {
            parent[i] = i;
            Gsize[i] = 1;
        }
        while(sz(stk))
            stk.pop();
        NumberOfComponents = MaxNodes;
    }

    T find_leader(int node)
    {
        return (parent[node] == node ? node : find_leader(parent[node]));
    }

    bool is_same_sets(int u, int v)
    {
        return find_leader(u) == find_leader(v);
    }

    void union_sets(int u, int v)
    {
        int leader_u = find_leader(u), leader_v = find_leader(v);
        if (leader_u == leader_v)
        {
            stk.push({-1, -1, -1, -1, -1});
            return;
        }
        // make leader_u is the leader with the larger component
        if (Gsize[leader_u] < Gsize[leader_v])
            swap(leader_u, leader_v);
        stk.push({leader_u, Gsize[leader_u], leader_v, parent[leader_v], NumberOfComponents});
        Gsize[leader_u] += Gsize[leader_v];
        parent[leader_v] = leader_u;
        NumberOfComponents--;
    }

    void restore()
    {
        while (sz(stk))
            undo();
    }
    void undo(){
        auto [leader_u, size_u, leader_v, parent_v, _NumberOfComponents] = stk.top();
        stk.pop();
        if(leader_u == -1) return;
        Gsize[leader_u] = size_u;
        parent[leader_v] = parent_v;
        NumberOfComponents = _NumberOfComponents;
    }
    int get_size(int u)
    {
        return Gsize[find_leader(u)];
    }

    int get_components_number()
    {
        return NumberOfComponents;
    }
};
