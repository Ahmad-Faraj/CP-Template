#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define ll long long 
#define all(x) x.begin(),x.end()
#define sz(x) (int)x.size()
#define vl vector<ll>
#define pb push_back
#define ordered_set tree<ll ,  null_type ,  less_equal<> ,  rb_tree_tag ,  tree_order_statistics_node_update>

void M7MDAN_FARAG(){
  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr); 
  #ifndef ONLINE_JUDGE
   freopen("input.txt", "r", stdin),freopen("output.txt", "w", stdout);
  #endif
}

template < typename T = int > istream& operator >> (istream &in, vector < T > &v) {
    for (auto &x : v) in >> x;
    return in;
}

template < typename T = int > ostream& operator << (ostream &out, const vector < T > &v) { 
    for (const T &x : v) out << x << ' '; 
    return out;
}

template < typename T = int, int LOG = 30 > struct Trie {

    struct Node {
 
        Node* child[2];
        int freq;
 
        Node(){
            memset(child, 0, sizeof(child));
            freq = 0;
        }
    };

    Node* root;

    Trie(){
        root = new Node;
    }

    void insert(const T& x){
        Node* curr = root; 
        for(int bit = LOG; bit >= 0; --bit){
            int bit_val = (x >> bit) & 1;
            if(!curr -> child[bit_val]) 
                curr -> child[bit_val] = new Node;
            curr = curr -> child[bit_val];
            ++curr -> freq;
        }
    }
  
    void erase(const T& x, int bit, Node* curr){
        if(bit < 0) return;
        int bit_val = (x >> bit) & 1;
        erase(x, bit - 1, curr -> child[bit_val]);
        if(--curr -> child[bit_val] -> freq == 0){
            delete curr -> child[bit_val];
            curr -> child[bit_val] = nullptr;
        }
    }

    bool search(const T& x){
        Node* curr = root; 
        for(int bit = LOG; bit >= 0; --bit){
            int bit_val = (x >> bit) & 1;
            if(!curr -> child[bit_val]) 
                return false;
            curr = curr -> child[bit_val];
        }
        return true;
    }
    
    T max_xor(const T& x){
        T ans = 0;
        Node* curr = root;
        for(int bit = LOG; bit >= 0; --bit){
            int bit_val = (x >> bit) & 1;
            if(curr -> child[!bit_val]){
                ans |= (1LL << bit);
                curr = curr -> child[!bit_val];
            }else{
                curr = curr -> child[bit_val];
            }
        }
        return ans;
    }

    void erase(const T& x){
        if(search(x)) 
            erase(x, LOG, root);
    }
};

void Try() {
ll n;
cin >> n;

Trie < ll > trie;

vl v(n);
cin >> v;

trie.insert(0);

ll pre = 0;

for(int i = 0; i < n; i++){
    pre ^= v[i];
    trie.insert(pre);
}

ll suf = 0, ans = trie.max_xor(0);

for(int i = n - 1; i >= 0; i--){
    suf ^= v[i];
    ans = max(ans, trie.max_xor(suf));
}
cout << ans << '\n';
}

signed main() {
    M7MDAN_FARAG();
    int t = 1;
    // cin >> t;
    while(t--) {
        Try();
    }
}
