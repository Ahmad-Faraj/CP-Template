// Brute-force test for:
//   Math/Number_Theory/Modular_Int.cpp
//   Data_Structure/Hash_Table.cpp
//   Misc/Huffman_Coding.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
using ll = long long;
namespace MI {
#include "../Math/Number_Theory/Modular_Int.cpp"
}
namespace HT {
#include "../Data_Structure/Hash_Table.cpp"
}
namespace HU {
#include "../Misc/Huffman_Coding.cpp"
}
int main() {
    mt19937_64 gen(2468);
    const ll M = 1000000007;
    int arith=0, divFail=0, powFail=0, combFail=0, mapFail=0, hufCost=0, hufPrefix=0, hufOpt=0;

    // ---- mint against explicit modular arithmetic ----
    for (int it = 0; it < 200000; it++) {
        ll a = (ll)(gen()%4000000000ULL) - 2000000000LL, b = (ll)(gen()%4000000000ULL) - 2000000000LL;
        auto n = [&](ll v){ return ((v % M) + M) % M; };
        MI::mint x = a, y = b;
        if ((ll)(int)(x+y) != n(n(a)+n(b))) arith++;
        if ((ll)(int)(x-y) != n(n(a)-n(b))) arith++;
        if ((ll)(int)(x*y) != (ll)((__int128)n(a)*n(b)%M)) arith++;
        if ((ll)(int)(-x)  != n(-a)) arith++;
        if ((x == MI::mint(a)) == false) arith++;
        // division: x/y*y == x whenever y is non-zero
        if (n(b) != 0) { MI::mint q = x / y; if ((ll)(int)(q*y) != n(a)) divFail++; }
    }
    for (int it = 0; it < 5000; it++) {
        ll a = (ll)(gen()%1000000), e = (ll)(gen()%50);
        MI::mint x = a; ll want = 1, base = ((a%M)+M)%M, ee = e;
        while (ee) { if (ee&1) want = (ll)((__int128)want*base%M); base = (ll)((__int128)base*base%M); ee>>=1; }
        if ((ll)(int)x.power(e) != want) powFail++;
    }
    {   // nCr against Pascal
        const int N = 40;
        vector<vector<ll>> pas(N+1, vector<ll>(N+1,0));
        for (int i=0;i<=N;i++){ pas[i][0]=1; for(int j=1;j<=i;j++) pas[i][j]=(pas[i-1][j-1]+pas[i-1][j])%M; }
        MI::Mod_Combinatorics c(200);
        for (int i=0;i<=N;i++) for (int j=0;j<=N;j++){
            ll want = (j<0||j>i)?0:pas[i][j];
            if ((ll)(int)c.nCr(i,j) != want) combFail++;
        }
    }

    // ---- hash table behaves like a map ----
    for (int it = 0; it < 300; it++) {
        HT::Fast_Map<ll,int> fm; HT::Safe_Map<ll,int> sm; map<ll,int> ref;
        for (int r = 0; r < 200; r++) {
            ll k = (ll)(gen()%50) - 25; int v = (int)(gen()%100);
            if (gen()%3) { fm[k]=v; sm[k]=v; ref[k]=v; }
            else {
                auto it2 = ref.find(k);
                int want = (it2==ref.end()) ? -1 : it2->second;
                auto f1 = fm.find(k); int g1 = (f1==fm.end()) ? -1 : f1->second;
                auto f2 = sm.find(k); int g2 = (f2==sm.end()) ? -1 : f2->second;
                if (g1 != want || g2 != want) mapFail++;
            }
        }
        if (fm.size() != ref.size() || sm.size() != ref.size()) mapFail++;
    }

    // ---- Huffman: cost, prefix-freeness, and optimality vs Kraft enumeration ----
    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen()%6);
        vector<ll> w(n); for (ll &x : w) x = 1 + (ll)(gen()%20);
        HU::Huffman_Coding h(w);
        if (n > 1 && h.encoded_length() != h.total_cost) hufCost++;
        // prefix-free: no code is a prefix of another
        vector<string> cs; for (auto &[s,c] : h.codes) cs.push_back(c);
        for (size_t i=0;i<cs.size();i++) for (size_t j=0;j<cs.size();j++)
            if (i!=j && cs[j].compare(0, cs[i].size(), cs[i]) == 0) hufPrefix++;
        // optimality: minimise sum(w*len) over all length vectors satisfying Kraft
        if (n > 1) {
            ll best = LLONG_MAX; vector<int> len(n,1);
            function<void(int)> rec = [&](int idx){
                if (idx == n) {
                    double kraft = 0; for (int l : len) kraft += pow(2.0, -l);
                    if (kraft > 1 + 1e-9) return;
                    ll cost = 0; for (int i2=0;i2<n;i2++) cost += w[i2]*len[i2];
                    best = min(best, cost); return;
                }
                for (int l = 1; l <= n; l++) { len[idx] = l; rec(idx+1); }
            };
            rec(0);
            if (h.encoded_length() != best) hufOpt++;
        }
    }
    printf("Batch 2: Modular_Int / Hash_Table / Huffman_Coding\n");
    printf("  mint + - * negate vs explicit : %d failures\n", arith);
    printf("  mint division round trip      : %d failures\n", divFail);
    printf("  mint power                    : %d failures\n", powFail);
    printf("  nCr via mint vs Pascal        : %d failures\n", combFail);
    printf("  hash maps match std::map      : %d failures\n", mapFail);
    printf("  huffman cost == sum(w*len)    : %d failures\n", hufCost);
    printf("  huffman codes prefix-free     : %d failures\n", hufPrefix);
    printf("  huffman OPTIMAL vs Kraft scan : %d failures\n", hufOpt);
}
