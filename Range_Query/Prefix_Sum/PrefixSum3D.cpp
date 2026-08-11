/* Topic: 3D Prefix Sum and Difference Array
 * Description: Extends the prefix sum technique into three dimensions. Allows querying the sum
 *   of elements within an arbitrary 3D cuboid in O(1) time.
 * Usage: PrefixSum3D<int> ps(n, m, k); ps.build(a); ps.query(r1, c1, d1, r2, c2, d2); Diff3D<int> ds(n, m, k);
 * ds.add(r1, c1, d1, r2, c2, d2, val); ds.build();
 */
#include "../../core.h"

template <typename T = int>
struct PrefixSum3D {
    int n, m, k_dim;
    vector<vector<vector<T>>> prf;

    PrefixSum3D(int _n, int _m, int _k) {
        n = _n;
        m = _m;
        k_dim = _k;
        prf.assign(n + 1, vector<vector<T>>(m + 1, vector<T>(k_dim + 1, 0)));
    }

    // Time Complexity: O(N * M * K)
    // Space Complexity: O(N * M * K)
    void build(const vector<vector<vector<T>>>& a) {
        // 0-indexed 3D matrix 'a'
        for (int x = 1; x <= n; ++x) {
            for (int y = 1; y <= m; ++y) {
                for (int z = 1; z <= k_dim; ++z) {
                    prf[x][y][z] = a[x - 1][y - 1][z - 1] + prf[x - 1][y][z] + prf[x][y - 1][z] + prf[x][y][z - 1] -
                                   prf[x - 1][y - 1][z] - prf[x - 1][y][z - 1] - prf[x][y - 1][z - 1] +
                                   prf[x - 1][y - 1][z - 1];
                }
            }
        }
    }

    // 1-indexed query
    // Time Complexity: O(1)
    // Space Complexity: O(1)
    T query(int from_x, int from_y, int from_z, int to_x, int to_y, int to_z) {
        return prf[to_x][to_y][to_z] - prf[from_x - 1][to_y][to_z] - prf[to_x][from_y - 1][to_z] -
               prf[to_x][to_y][from_z - 1] + prf[from_x - 1][from_y - 1][to_z] + prf[from_x - 1][to_y][from_z - 1] +
               prf[to_x][from_y - 1][from_z - 1] - prf[from_x - 1][from_y - 1][from_z - 1];
    }
};

template <typename T = int>
struct Diff3D {
    int n, m, k_dim;
    vector<vector<vector<T>>> diff;

    Diff3D(int _n, int _m, int _k) {
        n = _n;
        m = _m;
        k_dim = _k;
        diff.assign(n + 2, vector<vector<T>>(m + 2, vector<T>(k_dim + 2, 0)));
    }

    // 1-indexed update
    // Time Complexity: O(1)
    // Space Complexity: O(1)
    void add(int from_x, int from_y, int from_z, int to_x, int to_y, int to_z, T val) {
        diff[from_x][from_y][from_z] += val;
        diff[to_x + 1][from_y][from_z] -= val;
        diff[from_x][to_y + 1][from_z] -= val;
        diff[to_x][from_y][to_z + 1] -= val;
        diff[to_x + 1][to_y + 1][from_z] += val;
        diff[to_x + 1][from_y][to_z + 1] += val;
        diff[from_x][to_y + 1][to_z + 1] += val;
        diff[to_x + 1][to_y + 1][to_z + 1] -= val;
    }

    // Time Complexity: O(N * M * K)
    // Space Complexity: O(N * M * K)
    void build() {
        for (int x = 1; x <= n; ++x) {
            for (int y = 1; y <= m; ++y) {
                for (int z = 1; z <= k_dim; ++z) {
                    diff[x][y][z] += diff[x - 1][y][z] + diff[x][y - 1][z] + diff[x][y][z - 1] - diff[x - 1][y - 1][z] -
                                     diff[x - 1][y][z - 1] - diff[x][y - 1][z - 1] + diff[x - 1][y - 1][z - 1];
                }
            }
        }
    }
};
