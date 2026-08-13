#include "../../core.h"
/*
 * Topic: Manhattan Minimum Spanning Tree
 * Description: Computes MST for 2D points using Manhattan distance efficiently.
 * 
 * Important Facts:
 * - 0-based indexing for nodes by default.
 * - Time Complexity: O(N log N)
 * - Space Complexity: O(N)
 */
#include <vector>
#include <algorithm>
#include <numeric>

struct ManhattanMST {
    struct Point {
        long long x, y;
        int id;
    };
    
    struct Edge {
        int u, v;
        long long w;
        bool operator<(const Edge& o) const {
            return w < o.w;
        }
    };
    
    int n;
    vector<Point> pts;
    vector<Edge> edges;
    
    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // ManhattanMST mmst(number_of_points)
    ManhattanMST(int n_) : n(n_) {}
    
    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // mmst.addPoint(x, y, id)
    void addPoint(long long x, long long y, int id) {
        pts.push_back({x, y, id});
    }
    
    // Time Complexity: O(alpha(V))
    // Space Complexity: O(1) auxiliary
    // int root = mmst.findSet(parent, u)
    int findSet(vector<int>& parent, int u) {
        return parent[u] == u ? u : (parent[u] = findSet(parent, parent[u]));
    }
    
    // Time Complexity: O(N log N)
    // Space Complexity: O(N)
    // long long mst_cost = mmst.solve()
    long long solve() {
        vector<int> id(n);
        iota(id.begin(), id.end(), 0);
        
        for (int dir = 0; dir < 4; ++dir) {
            for (auto& p : pts) {
                if (dir == 1 || dir == 3) {
                    swap(p.x, p.y);
                }
                if (dir == 2) {
                    p.x = -p.x;
                }
            }
            
            sort(id.begin(), id.end(), [&](int a, int b) {
                return (pts[a].x + pts[a].y) < 
                       (pts[b].x + pts[b].y);
            });
            
            vector<long long> ys;
            for (int i = 0; i < n; ++i) {
                ys.push_back(pts[i].y - pts[i].x);
            }
            sort(ys.begin(), ys.end());
            ys.erase(unique(ys.begin(), ys.end()), ys.end());
            
            vector<pair<long long, int>> bit(ys.size() + 1, {(long long)2e18, -1});
            
            for (int i = n - 1; i >= 0; --i) {
                int p = id[i];
                int pos = static_cast<int>(lower_bound(ys.begin(), ys.end(), pts[p].y - pts[p].x) - ys.begin()) + 1;
                
                pair<long long, int> mn = {(long long)2e18, -1};
                for (int j = pos; j <= static_cast<int>(ys.size()); j += j & -j) {
                    if (bit[j].first < mn.first) {
                        mn = bit[j];
                    }
                }
                
                if (mn.second != -1) {
                    edges.push_back({pts[p].id, pts[mn.second].id,
                        abs(pts[p].x - pts[mn.second].x) + 
                        abs(pts[p].y - pts[mn.second].y)});
                }
                
                for (int j = pos; j > 0; j -= (j & -j)) {
                    if (pts[p].x + pts[p].y < bit[j].first) {
                        bit[j] = {pts[p].x + pts[p].y, p};
                    }
                }
            }
        }
        
        sort(edges.begin(), edges.end());
        vector<int> parent(n);
        iota(parent.begin(), parent.end(), 0);
        
        long long total_weight = 0;
        int count = 0;
        for (const auto& e : edges) {
            int u = findSet(parent, e.u);
            int v = findSet(parent, e.v);
            if (u != v) {
                parent[u] = v;
                total_weight += e.w;
                count++;
            }
        }
        
        return count == n - 1 ? total_weight : -1;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (cin >> n) {
        ManhattanMST mmst(n);
        for (int i = 0; i < n; ++i) {
            long long x, y;
            cin >> x >> y;
            mmst.addPoint(x, y, i);
        }
        long long ans = mmst.solve();
        if (ans == -1) cout << "IMPOSSIBLE\n";
        else cout << ans << "\n";
    }
    return 0;
}
