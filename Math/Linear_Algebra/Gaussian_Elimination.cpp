// Gaussian Elimination: solves a system of linear equations, and reports rank, determinant and whether it is singular.
// Use when: "solve these n equations in n unknowns", expected-value systems, checking linear independence.
// Handles: singular systems (none or infinitely many solutions), rectangular systems, and GF(2) systems.
// Time: O(n^2 * m)
// Indexing: 0-based; the augmented matrix is n rows by (m + 1) columns, the last column being the right-hand side
// Note: partial pivoting is on, but doubles still drift - treat |value| < EPS as zero. kind says which case holds.

#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

enum Solutions { NO_SOLUTION = 0, ONE_SOLUTION = 1, INFINITE_SOLUTIONS = 2 };

struct Gauss {
    int rank = 0;
    double determinant = 0;   // only meaningful for a square system
    Solutions kind = NO_SOLUTION;
    vector<double> answer;    // one solution; free variables come back as 0

    // aug is n rows of (m + 1) values: the coefficients then the right-hand side
    Gauss(vector<vector<double>> aug, int n, int m) {
        answer.assign(m, 0.0);
        vector<int> where(m, -1); // which row pins each variable
        determinant = 1.0;
        int row = 0;
        for (int col = 0; col < m && row < n; col++) {
            int pivot = row;
            for (int i = row; i < n; i++)
                if (fabs(aug[i][col]) > fabs(aug[pivot][col])) pivot = i; // partial pivoting
            if (fabs(aug[pivot][col]) < EPS) {
                determinant = 0;
                continue; // this column has no pivot, so its variable is free
            }
            if (pivot != row) {
                swap(aug[pivot], aug[row]);
                determinant = -determinant;
            }
            determinant *= aug[row][col];
            where[col] = row;
            for (int i = 0; i < n; i++) {
                if (i == row) continue;
                double factor = aug[i][col] / aug[row][col];
                for (int j = col; j <= m; j++) aug[i][j] -= aug[row][j] * factor;
            }
            row++;
        }
        rank = row;
        if (n != m) determinant = 0; // a determinant only exists for a square system

        for (int col = 0; col < m; col++)
            if (where[col] != -1) answer[col] = aug[where[col]][m] / aug[where[col]][col];

        // a row of all zeros with a non-zero right-hand side means no solution at all
        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < m; j++) sum += answer[j] * aug[i][j];
            if (fabs(sum - aug[i][m]) > EPS) {
                kind = NO_SOLUTION;
                return;
            }
        }
        kind = (rank < m) ? INFINITE_SOLUTIONS : ONE_SOLUTION;
    }
};

// XOR-linear systems: the same elimination over GF(2), exact and much faster
struct Gauss_XOR {
    int rank = 0;
    bool solvable = true;
    vector<int> answer;

    // each row is (mask of variables, right-hand bit)
    Gauss_XOR(vector<pair<unsigned long long, int>> rows, int m) {
        answer.assign(m, 0);
        vector<int> where(m, -1);
        int row = 0;
        for (int col = 0; col < m && row < (int)rows.size(); col++) {
            int pivot = -1;
            for (int i = row; i < (int)rows.size(); i++)
                if ((rows[i].first >> col) & 1) { pivot = i; break; }
            if (pivot == -1) continue;
            swap(rows[pivot], rows[row]);
            where[col] = row;
            for (int i = 0; i < (int)rows.size(); i++)
                if (i != row && ((rows[i].first >> col) & 1)) {
                    rows[i].first ^= rows[row].first;
                    rows[i].second ^= rows[row].second;
                }
            row++;
        }
        rank = row;
        for (int col = 0; col < m; col++)
            if (where[col] != -1) answer[col] = rows[where[col]].second;
        for (size_t i = 0; i < rows.size(); i++)
            if (rows[i].first == 0 && rows[i].second != 0) solvable = false;
    }
};

// Standard problem: n equations in n unknowns - print the solution, or report the system is singular
void solve() {
    int n;
    cin >> n;
    vector<vector<double>> aug(n, vector<double>(n + 1));
    for (auto &row : aug)
        for (double &v : row) cin >> v;
    Gauss g(aug, n, n);
    if (g.kind == NO_SOLUTION)
        cout << "NO SOLUTION\n";
    else if (g.kind == INFINITE_SOLUTIONS)
        cout << "INFINITE SOLUTIONS\n";
    else {
        cout << fixed << setprecision(6);
        for (int i = 0; i < n; i++) cout << g.answer[i] << " \n"[i + 1 == n];
    }
}
