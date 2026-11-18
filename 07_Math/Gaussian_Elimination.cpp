// Gaussian Elimination: solves n linear equations in m unknowns (doubles, or XOR over GF(2)).
// Use when: "solve this system", expected values via equations, rank / determinant, XOR systems.
// Usage: Gauss g(aug, n, m);  g.kind (NO_SOLUTION / ONE_SOLUTION / INFINITE_SOLUTIONS), g.answer
//        Gauss_XOR gx(rows, m);  // rows[i] = {mask of variables, rhs bit}; gx.solvable, gx.answer
// Handles: singular systems (none / infinitely many), rectangular n != m, determinant if square.
// Time: O(n * m * min(n, m))
// Indexing: 0-based; aug has n rows of m coefficients then the right-hand side (m + 1 columns)
// Note: |v| < EPS counts as zero; scale EPS with the input. Free variables come back as 0.
//       Gauss_XOR needs m <= 64. Source: normalize branch (old gauss_solve divided by 0 pivots).

#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

enum Solutions { NO_SOLUTION = 0, ONE_SOLUTION = 1, INFINITE_SOLUTIONS = 2 };

struct Gauss {
    int rank = 0;
    double determinant = 0; // only meaningful for a square system
    Solutions kind = NO_SOLUTION;
    vector<double> answer; // one solution; free variables come back as 0

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
                continue; // no pivot in this column: its variable is free
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
        if (n != m || rank < m) determinant = 0;

        for (int col = 0; col < m; col++)
            if (where[col] != -1) answer[col] = aug[where[col]][m] / aug[where[col]][col];

        // a row 0 = non-zero means no solution
        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < m; j++) sum += answer[j] * aug[i][j];
            if (fabs(sum - aug[i][m]) > EPS) {
                kind = NO_SOLUTION;
                return;
            }
        }
        kind = rank < m ? INFINITE_SOLUTIONS : ONE_SOLUTION;
    }
};

// XOR-linear systems over GF(2): exact
struct Gauss_XOR {
    int rank = 0;
    bool solvable = true;
    vector<int> answer; // one solution; free variables come back as 0

    Gauss_XOR(vector<pair<unsigned long long, int>> rows, int m) {
        answer.assign(m, 0);
        vector<int> where(m, -1);
        int row = 0, n = rows.size();
        for (int col = 0; col < m && row < n; col++) {
            int pivot = -1;
            for (int i = row; i < n; i++)
                if (rows[i].first >> col & 1) {
                    pivot = i;
                    break;
                }
            if (pivot == -1) continue;
            swap(rows[pivot], rows[row]);
            where[col] = row;
            for (int i = 0; i < n; i++)
                if (i != row && (rows[i].first >> col & 1)) {
                    rows[i].first ^= rows[row].first;
                    rows[i].second ^= rows[row].second;
                }
            row++;
        }
        rank = row;
        for (int col = 0; col < m; col++)
            if (where[col] != -1) answer[col] = rows[where[col]].second;
        for (int i = 0; i < n; i++)
            if (rows[i].first == 0 && rows[i].second != 0) solvable = false;
    }
};
