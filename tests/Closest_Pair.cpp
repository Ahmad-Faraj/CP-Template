// Brute-force test for:
//   Geometry/Closest_Pair.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace CP {
#include "../Geometry/Closest_Pair.cpp"
}
int main() {
    mt19937_64 gen(20260818);
    int distFail = 0, idxFail = 0, dupFail = 0, edgeFail = 0, checked = 0;
    for (int it = 0; it < 6000; it++) {
        int n = (int)(gen() % 14);
        int range = (it % 3 == 0) ? 4 : 60;          // small range forces duplicates
        vector<pair<ll,ll>> pts(n);
        for (auto &p : pts) p = {(ll)(gen()%range) - range/2, (ll)(gen()%range) - range/2};
        auto got = CP::closest_pair(pts);
        checked++;
        // brute force O(n^2)
        ll want = LLONG_MAX; int wi = -1, wj = -1;
        for (int i = 0; i < n; i++) for (int j = i+1; j < n; j++) {
            ll dx = pts[i].first - pts[j].first, dy = pts[i].second - pts[j].second;
            ll d = dx*dx + dy*dy;
            if (d < want) want = d, wi = i, wj = j;
        }
        if (got.squared_distance != want) distFail++;
        if (n < 2) { if (got.first != -1 || got.second != -1) edgeFail++; continue; }
        // the reported indices must be valid and realise the distance
        if (got.first < 0 || got.second < 0 || got.first >= n || got.second >= n || got.first == got.second) { idxFail++; continue; }
        ll dx = pts[got.first].first - pts[got.second].first, dy = pts[got.first].second - pts[got.second].second;
        if (dx*dx + dy*dy != want) idxFail++;
        if (want == 0) dupFail += (got.squared_distance != 0);
    }
    // explicit duplicates and a large-coordinate case
    {
        auto r = CP::closest_pair(vector<pair<ll,ll>>{{5,5},{1,1},{5,5}});
        if (r.squared_distance != 0) dupFail++;
        auto b = CP::closest_pair(vector<pair<ll,ll>>{{-1000000000,-1000000000},{1000000000,1000000000},{999999999,1000000000}});
        if (b.squared_distance != 1) edgeFail++;
        if (CP::closest_pair(vector<pair<ll,ll>>{}).first != -1) edgeFail++;
        if (CP::closest_pair(vector<pair<ll,ll>>{{3,4}}).first != -1) edgeFail++;
    }
    printf("Closest_Pair: %d random sets\n", checked);
    printf("  squared distance vs O(n^2)   : %d failures\n", distFail);
    printf("  reported indices realise it  : %d failures\n", idxFail);
    printf("  duplicate points give 0      : %d failures\n", dupFail);
    printf("  n<2, and coords at 1e9       : %d failures\n", edgeFail);
}
