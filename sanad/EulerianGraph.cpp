#include <bits/stdc++.h>

using namespace std;

#define cout_2d(vec,n,m)for(int Ii=0;Ii<n;Ii++,cout<<"\n")for(int Jj=0;Jj<m&&cout<<vec[Ii][Jj]<<" ";Jj++);
#define cin_2d(vec,n,m)for(int iii=0;iii<n;iii++)for(int jjj=0;jjj<m&&cin>>vec[iii][jjj];jjj++);
#define cout_map(mp) for(auto& [fff, sss] : mp) cout << fff << "  " << sss << "\n";
#define cout(vec) for(auto& iii : vec) cout << iii << " "; cout << "\n";
#define cin(vec) for(auto& iii : vec) cin >> iii
#define to_decimal(bin) stoll(bin, nullptr, 2)
#define unq(VEC) sort(all(VEC)); VEC.resize(unique(all(VEC)) - VEC.begin());
#define ll long long
#define myCeil(N, M) ((N) / (M) + !!((N) % (M)))
#define dg(DG)   cout << #DG << ": " << (DG) << "\n"
#define dg1(DG) cout << #DG << ": " << (DG) << " || ";
#define dg3(DG1,DG2,DG3) {dg2(DG1, DG2) dg1(DG3)}
#define dg2(DG1, DG2) {dg1(DG1) dg1(DG2)}
#define dg4(DG1,DG2,DG3,DG4) {dg3(DG1,DG2,DG3) dg1(DG4)}
#define sz(x) (int)(x.size())
#define all(vec) vec.begin(), vec.end()
#define rall(vec) vec.rbegin(), vec.rend()
#define ff first
#define ss second
#define el "\n"
constexpr long long OO = 1 << 30, OOL = 1LL << 60, MOD = 1e9 + 7;
constexpr double EPS = 1e-9;
string get_binary(long long num){if(!num)return"";return get_binary(num/2)+(num%2?"1":"0");}
bool is_subseq(string p, string w){int szp = p.size(), szw = w.size(), ip=0, iw=0;
for (; ip < szp && iw < szw; iw++){if (p[ip] == w[iw]){ip++;}}  return ip == szp;}
vector<pair<int,int>>dxy4={{1,0},{0,1},{0,-1},{-1,0}};
vector<pair<int,int>>dxy8={{1,-1}, {0,-1}, {-1,-1}, {1,0}, {-1,0}, {1,1}, {0,1}, {-1,1}};
vector<pair<int,int>>dxyk={{-1,-2}, {-2,-1}, {1,-2}, {2,-1}, {-2,1}, {-1,2}, {1,2}, {2,1}};

/* A function that get euler path using a stack */
void Find_euler_path_stack(int start_node, vector<vector<pair<int,int>>>&theAdjList, vector<bool>&VIS_Edges, vector<int>&AnsVec) {
    stack <int> st;
    int theTop;
    pair <int, int> curChild;
    
    st.push(start_node);
    
    while (not st.empty()) {

        theTop = st.top();

        for (int i = sz(theAdjList[theTop]) - 1; i >= 0; i--){
            auto [child, idx] = theAdjList[theTop][i];
            theAdjList[theTop].pop_back();

            if (not VIS_Edges[idx]) {
                VIS_Edges[idx] = true;
                st.push(child);
                break;
            }
        }

        if (theTop == st.top()) {
            AnsVec.emplace_back(st.top());
            st.pop();
        }
    }
}
                                                                                 // choose 0 to only cycle, 1 to only path, 2 to both are good
vector<int> Get_Euler(int n, vector<pair<int,int>> theEdges, bool DIRECTED = false, int cycle_path_both012 = 0) {
    // an adjacency list to store the neighbours of each node and the idx of the edge connecting them
    vector < vector < pair < int , int > > >  AdjList(n + 1);

    // the size of the edges
    int m = theEdges.size();
    
    // declaring a visited array for the edges
    vector < bool > VisEdges(m);

    // Assigning for each node its neighbours and the idx of edge connecting them;
    for (int i = 0, u, v; i < m; i++){
        u = theEdges[i].first, v = theEdges[i].second;
        AdjList[u].emplace_back(v, i);
        if (not DIRECTED)
            AdjList[v].emplace_back(u, i);
    }

    bool hasCycle = true, hasPath = true;
    vector < int > In(n + 1), Out(n + 1), with_odd_deg;
    int STARTING_NODE = -1, ENDING_NODE = -1;

    if (DIRECTED) {
        for (auto &E : theEdges)
            In[E.second]++, Out[E.first]++;
        
        for (int i = 1; i <= n; i++)
            if (In[i] == Out[i])
                continue;
            else if (In[i] == Out[i] - 1){ // starting node
                if (~STARTING_NODE)
                    hasPath = false;
                STARTING_NODE = i;
                hasCycle = false;
            }else if (In[i] == Out[i] + 1) { // Ending node
                if (~ENDING_NODE)
                    hasPath = false;
                ENDING_NODE = i;
                hasCycle = false;
            }else
                hasCycle = hasPath = false;
    }
    else {
        // to store all the nodes whose degree is odd
        for (int i = 1; i <= n; i++)
            if (int(AdjList[i].size()) & 1)
                with_odd_deg.emplace_back(i);

        if (sz(with_odd_deg) == 0)
            hasCycle = true;
        else if (sz(with_odd_deg) == 2)
            hasCycle = false, hasPath = true,
            STARTING_NODE = with_odd_deg.front(), ENDING_NODE = with_odd_deg.back();
        else
            hasCycle = hasPath = false;
    }


    if (cycle_path_both012 == 0 and not hasCycle)            return {-1};
    if (cycle_path_both012 == 1 and hasCycle)               return {-1};
    if (cycle_path_both012 == 2 and not hasPath)            return {-1};


    if (DIRECTED) {
        if (hasCycle){
            for (int i = 1; i <= n; i++)
                if (AdjList[i].size()) {
                    STARTING_NODE = i;
                    break;
                }
        }
    }else{
        if (not hasCycle) { // if there is an euler path not an euler cycle.
            theEdges.emplace_back(STARTING_NODE, ENDING_NODE);// add an edges between the start and end to complete the cycle
            AdjList[STARTING_NODE].emplace_back(ENDING_NODE, theEdges.size() - 1);
            AdjList[ENDING_NODE].emplace_back(STARTING_NODE, theEdges.size() - 1);
            VisEdges.emplace_back(false);
        }else{  // if there is a cycle, then we need to choose one node to start the cycle from
            for (int i = 1; i <= n; i++)
                if (AdjList[i].size()) {
                    STARTING_NODE = i;
                    break;
                }
        }
    }

    vector < int > AnsVec;
    Find_euler_path_stack(STARTING_NODE, AdjList, VisEdges, AnsVec);
    reverse(all(AnsVec));

    if (DIRECTED) {
        // Do nothing
    }else{
        // If it was a path, but we add an edge to make it a cycle, we have to remove the added edge
        if (not hasCycle)
            for (int i = 0; i + 1 < int(AnsVec.size()) - 1; i++)
                if ((AnsVec[i] == STARTING_NODE && AnsVec[i+1] == ENDING_NODE) || (AnsVec[i] == ENDING_NODE && AnsVec[i+1] == STARTING_NODE)) {
                    vector<int> ANS2;
                    
                    for (int j = i + 1; j < int(AnsVec.size()); j++)        ANS2.push_back(AnsVec[j]);
                    for (int j = 1; j <= i; j++)                            ANS2.push_back(AnsVec[j]);
                    
                    AnsVec = ANS2;
                    
                    break;
                }
    }

    if (int(AnsVec.size()) == m + 1)
        return AnsVec;
    else
        return {-1};
}

void SOLVE(int ts_ctr) {
    int n, m;
    cin >> n >> m;

    vector < pair < int , int > > E(m);
    for (auto &[F, S] : E)
        cin >> F >> S;
        
    vector < int > theAnswer = Get_Euler(n, E, true, 1);

    if (theAnswer.front() == -1)
        cout << "IMPOSSIBLE\n";
    else {
        cout(theAnswer);
    }
}

signed main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#if !defined(ONLINE_JUDGE)
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
    // cout << fixed << setprecision(10);
    int testcases = 1;
    // cin >> testcases;
    for (int test_ctr = 1; test_ctr <= testcases; test_ctr++)   SOLVE(test_ctr);
    /* cerr<<"Time Taken: "<<(float)clock()/CLOCKS_PER_SEC<<" Secs"<<"\n"; */
}
