#inlcude <iostream>
using namespace std;

vector<vector<int>> adj;
vector<int> dfsn, low, comp; // initialize dfsn and low to -1
vector<bool> instack; // initialize to false
stack<int> st;
vector<vector<int>> comps;
int n, m, idx, cnt;

void tarjan(int u, int p){
    dfsn[u] = low[u] = idx++;
    st.push(u);
    instack[u] = true;
    for(int v : adj[u]){
        if(v == p) continue;
        if(dfsn[v] == -1){
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
        }else if(instack[v]){
            low[u] = min(low[u], dfsn[v]);
        }
    }
    if(dfsn[u] == low[u]){
        comps.push_back(vector<int>());
        int v;
        do{
            v = st.top();
            st.pop();
            instack[v] = false;
            comp[v] = cnt;
            comps[cnt].push_back(v);
        }while(u != v);
        cnt++;
    }
}

vector<vector<int>> newadj;
// for directed
void computeCompGraph(){
    //TODO initialize newadj 
    //! newadj.assign(cnt, vector<int>());
    for (int i = 0; i < sz(adj); i++){
        for(int j = 0; j < sz(adj[i]); j++){
            int k = adj[i][j];
            if(comp[k] != comp[i]){
                newadj[comp[i]].push_back(comp[k]);
            }
        }
    }
}

//* bridges
vector<pair<int, int>> bridges;
void tarjan(int u, int p){
    dfsn[u] = low[u] = idx++;
    for(int v : adj[u]){
        if(v == p) continue;
        if(dfsn[v] == -1){
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if(low[v] > dfsn[u]){
                bridges.push_back({u, v});
            }
        }else{
            low[u] = min(low[u], dfsn[v]);
        }
    }
}

//* articulation points
vector<int> artPoints;
void tarjan(int u, int p){
    dfsn[u] = low[u] = idx++;
    int children = 0;
    for(int v : adj[u]){
        if(v == p) continue;
        if(dfsn[v] == -1){
            children++;
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if(low[v] >= dfsn[u] && p != -1){
                artPoints.push_back(u);
            }
        }else{
            low[u] = min(low[u], dfsn[v]);
        }
    }
    if(p == -1 && children > 1){
        artPoints.push_back(u);
    }
}








int main(){
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while(tc--){
        comps.clear();
        cin >> n >> m;
        adj.assign(n + 1, vector<int>());
        for(int i = 0; i < m; i++){
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        dfsn.assign(n + 1, -1);
        low.assign(n + 1, -1);
        comp.assign(n + 1, -1);
        instack.assign(n + 1, false);
        idx = cnt = 0;
        for(int i = 1; i <= n; i++){
            if(dfsn[i] == -1){
                tarjan(i, -1);
            }
        }
        computeCompGraph();
    }
    return 0;
}