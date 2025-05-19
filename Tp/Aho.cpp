#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}
int nextNodeId = 0;
struct AhoCorasick
{
    struct Node{
        Node *fail;
        Node *child[26];
        vector<int> patIdx;
        vector<char> chars;
        int id;
        Node(){
            memset(child, 0, sizeof(child));
            fail = 0;
            id = nextNodeId++;
        }
    };
    Node *root;
    AhoCorasick(){
        root = new Node();
    }
    AhoCorasick(vector<string> &patterns){
        root = new Node();
        for(int i = 0; i < sz(patterns); i++){
            insert(patterns[i], i);
        }
        build();
    }
    void insert(string &s, int idx){
        Node *cur = root;
        for(int i = 0; i < sz(s); i++){
            int c = s[i] - 'a';
            if(!cur->child[c]){
                cur->child[c] = new Node();
                cur->chars.push_back(c);
            }
            cur = cur->child[c];
        }
        cur->patIdx.push_back(idx);
    }
    void build(){
        queue<Node*> q;
        for(int i = 0; i < 26; i++){
            if(root->child[i]){
                root->child[i]->fail = root;
                q.push(root->child[i]);
            }
            else{
                root->child[i] = root;
            }
        }
        while(!q.empty()){
            Node *cur = q.front();
            q.pop();
            for(int i = 0; i < sz(cur->chars); i++){
                int c = cur->chars[i];
                Node *next = cur->child[c];
                Node *fail = cur->fail;
                while(fail != root && !fail->child[c]){
                    fail = fail->fail;
                }
                if(fail->child[c]){
                    next->fail = fail->child[c];
                }
                else{
                    next->fail = root;
                }
                for(int j = 0; j < sz(next->fail->patIdx); j++){
                    next->patIdx.push_back(next->fail->patIdx[j]);
                }
                q.push(next);
            }
        }
    }
    vector<vector<int>> search(string &s){
        vector<vector<int>> res(sz(s));
        Node *cur = root;
        for(int i = 0; i < sz(s); i++){
            int c = s[i] - 'a';
            while(cur != root && !cur->child[c]){
                cur = cur->fail;
            }
            if(cur->child[c]){
                cur = cur->child[c];
            }
            for(int j = 0; j < sz(cur->patIdx); j++){
                res[i].push_back(cur->patIdx[j]);
            }
        }
        return res;
    }
};
void solve(int tc)
{
    int n;
    cin >> n;
    vector<string> patterns(n);
    for(int i = 0; i < n; i++){
        cin >> patterns[i];
    }
    AhoCorasick ac(patterns);
    string s;
    cin >> s;
    vector<vector<int>> res = ac.search(s);
    for(int i = 0; i < sz(res); i++){
        cout << i << ": ";
        for(int j = 0; j < sz(res[i]); j++){
            cout << res[i][j] << sp;
        }
        cout << nl;
    }
}
signed main(void)
{
    fastio();
    int tc = 1;
    //cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}