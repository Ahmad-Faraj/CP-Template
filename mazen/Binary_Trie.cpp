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
struct BinaryTrie {

    struct Node {
        Node* child[2];
        int freq;
        Node(){
            memset(child, 0, sizeof(child));
            freq = 0;
        }
    };

    Node* root;
    BinaryTrie(){
        root = new Node;
        insert(0);
    }
    
    void insert(const int n){
        Node* curr = root;
        for (int i = 40; i >= 0; i--){
            int bit = (n >> i) & 1;
            if(!curr -> child[bit]) curr -> child[bit] = new Node;
            curr = curr -> child[bit];
            curr -> freq++;
        }
    }
  
    void erase(const int n, int idx, Node* curr){
        if(idx == -1){
            return;
        }
        int bit = (n >> idx) & 1;
        erase(n, idx - 1, curr -> child[bit]);
        curr -> child[bit] -> freq--;
        if(curr -> child[bit] -> freq == 0){
            delete curr -> child[bit];
            curr -> child[bit] = 0;
        }
    }
    void erase(const int n){
        erase(n, 40, root);
    }
    int max_xor(const int n){
        Node *curr = root;
        int ans = 0;
        for (int i = 40; i >= 0; i--){
            int bit = (n >> i) & 1;
            if(curr -> child[!bit]){
                ans |= (1LL << i);
                curr = curr -> child[!bit];
            }else{
                curr = curr -> child[bit];
            }
        }
        return ans;
    }
    int min_xor(const int n){
        Node *curr = root;
        int ans = 0;
        for (int i = 40; i >= 0; i--){
            int bit = (n >> i) & 1;
            if(curr -> child[bit]){
                curr = curr -> child[bit];
            }else{
                ans |= (1LL << i);
                curr = curr -> child[!bit];
            }
        }
        return ans;
    }
};
void solve(int tc)
{
    
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