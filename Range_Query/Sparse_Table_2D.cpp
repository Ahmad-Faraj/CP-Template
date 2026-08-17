// 2D Sparse Table: max over any submatrix of a static grid, in O(1).
// Use when: "max/min over the submatrix (x1,y1)..(x2,y2)" with no updates and many queries.
// Handles: idempotent merges only - max, min, gcd, and, or. Static grid, no updates.
// Time: build O(n m log n log m) | query O(1) | memory O(n m log n log m)
// Indexing: 0-based, ranges inclusive
// Note: sized N=505, LG=10 (~102 MB). Shrink N and LG to the problem's grid or it will not fit.

#include <bits/stdc++.h>
using namespace std;

const int N = 505, LG = 10;

int st[N][N][LG][LG];
int grid[N][N], lg2[N];

int merge(int a, int b) { return max(a, b); }

void build(int n, int m) { // fills from grid[0..n-1][0..m-1]
    for (int i = 2; i < N; i++) lg2[i] = lg2[i >> 1] + 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) st[i][j][0][0] = grid[i][j];
    for (int a = 0; a < LG; a++)
        for (int b = 0; b < LG; b++) {
            if (a + b == 0) continue;
            for (int i = 0; i + (1 << a) <= n; i++)
                for (int j = 0; j + (1 << b) <= m; j++) {
                    if (!a)
                        st[i][j][a][b] = merge(st[i][j][a][b - 1], st[i][j + (1 << (b - 1))][a][b - 1]);
                    else
                        st[i][j][a][b] = merge(st[i][j][a - 1][b], st[i + (1 << (a - 1))][j][a - 1][b]);
                }
        }
}

int query(int x1, int y1, int x2, int y2) { // over rows [x1,x2] x cols [y1,y2]
    x2++, y2++;
    int a = lg2[x2 - x1], b = lg2[y2 - y1];
    return merge(merge(st[x1][y1][a][b], st[x2 - (1 << a)][y1][a][b]),
                 merge(st[x1][y2 - (1 << b)][a][b], st[x2 - (1 << a)][y2 - (1 << b)][a][b]));
}

// Standard problem: q queries asking the maximum inside a submatrix of a static grid
void solve() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) cin >> grid[i][j];
    build(n, m);
    int q;
    cin >> q;
    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        cout << query(x1 - 1, y1 - 1, x2 - 1, y2 - 1) << '\n';
    }
}
