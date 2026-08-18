// Brute-force test for:
//   Math/Linear_Algebra/XOR_Basis.cpp
//   Math/Linear_Algebra/XOR_Basis_Range.cpp
//   Math/Linear_Algebra/Gaussian_Elimination.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace XB {
#include "../Math/Linear_Algebra/XOR_Basis.cpp"
}
namespace XR {
#include "../Math/Linear_Algebra/XOR_Basis_Range.cpp"
}
namespace GA {
#include "../Math/Linear_Algebra/Gaussian_Elimination.cpp"
}

int main() {
    mt19937_64 gen(20260817);
    int rankFail = 0, canFail = 0, maxFail = 0, minFail = 0, distFail = 0, subFail = 0, kthFail = 0, mergeFail = 0;
    int rRankFail = 0, rMaxFail = 0, rCanFail = 0;
    int gSolFail = 0, gKindFail = 0, gDetFail = 0, gxFail = 0;
    int xbChecked = 0, gChecked = 0;

    // ---------- XOR basis against exhaustive subset enumeration ----------
    for (int it = 0; it < 4000; it++) {
        int n = (int)(gen() % 11);
        int bits = 6;
        vector<ll> a(n);
        for (ll &v : a) v = (ll)(gen() % (1 << bits));

        XB::XOR_Basis<ll> b;
        for (ll v : a) b.add(v);
        xbChecked++;

        // every reachable XOR, and how many subsets reach each
        map<ll, ll> subsets;
        for (int mask = 0; mask < (1 << n); mask++) {
            ll x = 0;
            for (int i = 0; i < n; i++)
                if (mask >> i & 1) x ^= a[i];
            subsets[x]++;
        }
        set<ll> reachable;
        for (auto &kv : subsets) reachable.insert(kv.first);

        if ((ll)reachable.size() != b.distinct_count()) distFail++;
        if ((1LL << b.rank) != (ll)reachable.size()) rankFail++;

        ll wantMax = *reachable.rbegin(), wantMin = *reachable.begin();
        if (b.max_xor() != wantMax) maxFail++;
        if (b.min_xor() != wantMin) minFail++;

        for (ll x = 0; x < (1 << bits); x++) {
            bool want = reachable.count(x) > 0;
            if (b.can_make(x) != want) canFail++;
            ll wantSubsets = subsets.count(x) ? subsets[x] : 0;
            if (b.subsets_making(x) != wantSubsets) subFail++;
        }

        // kth_smallest must walk the reachable set in order
        vector<ll> sorted(reachable.begin(), reachable.end());
        for (size_t k = 0; k < sorted.size(); k++)
            if (b.kth_smallest((ll)k) != sorted[k]) kthFail++;
        if (b.kth_smallest((ll)sorted.size()) != -1) kthFail++;
        if (b.kth_smallest(-1) != -1) kthFail++;

        // merge: the basis of the union of two lists
        if (n >= 2) {
            int half = n / 2;
            XB::XOR_Basis<ll> p, q;
            for (int i = 0; i < half; i++) p.add(a[i]);
            for (int i = half; i < n; i++) q.add(a[i]);
            p.merge(q);
            if (p.rank != b.rank) mergeFail++;
            if (p.max_xor() != wantMax) mergeFail++;
        }
    }

    // ---------- range (suffix) basis against rebuilding from scratch ----------
    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 10);
        vector<ll> a(n);
        for (ll &v : a) v = (ll)(gen() % 64);
        XR::XOR_Basis_Range<ll> r;
        for (int i = 0; i < n; i++) r.add(a[i], i);

        for (int l = 0; l < n; l++) {
            XB::XOR_Basis<ll> fresh;
            for (int i = l; i < n; i++) fresh.add(a[i]);
            if (r.rank_from(l) != fresh.rank) rRankFail++;
            if (r.max_xor_from(l) != fresh.max_xor()) rMaxFail++;
            for (ll x = 0; x < 64; x++)
                if (r.can_make_from(x, l) != fresh.can_make(x)) rCanFail++;
        }
    }

    // ---------- Gaussian elimination ----------
    for (int it = 0; it < 4000; it++) {
        int n = 1 + (int)(gen() % 5);
        vector<vector<double>> aug(n, vector<double>(n + 1));
        // build from a known solution most of the time, so a unique answer exists
        bool fromKnown = (it % 3 != 0);
        vector<double> truth(n);
        for (double &v : truth) v = (double)((ll)(gen() % 21) - 10);
        for (int i = 0; i < n; i++) {
            double rhs = 0;
            for (int j = 0; j < n; j++) {
                aug[i][j] = (double)((ll)(gen() % 11) - 5);
                rhs += aug[i][j] * truth[j];
            }
            aug[i][n] = fromKnown ? rhs : (double)((ll)(gen() % 21) - 10);
        }
        GA::Gauss g(aug, n, n);
        gChecked++;

        if (g.kind == GA::ONE_SOLUTION) {
            // substitute back: every equation must hold
            for (int i = 0; i < n; i++) {
                double sum = 0;
                for (int j = 0; j < n; j++) sum += aug[i][j] * g.answer[j];
                if (fabs(sum - aug[i][n]) > 1e-6) gSolFail++;
            }
            if (g.rank != n) gKindFail++;
        } else if (g.kind == GA::INFINITE_SOLUTIONS) {
            // the reported answer must still satisfy every equation
            for (int i = 0; i < n; i++) {
                double sum = 0;
                for (int j = 0; j < n; j++) sum += aug[i][j] * g.answer[j];
                if (fabs(sum - aug[i][n]) > 1e-6) gSolFail++;
            }
            if (g.rank >= n) gKindFail++;
        }
        // a system built from a known solution can never be reported unsolvable
        if (fromKnown && g.kind == GA::NO_SOLUTION) gKindFail++;
    }

    // determinant against the 2x2 and 3x3 closed forms
    for (int it = 0; it < 3000; it++) {
        vector<vector<double>> m2{{(double)(ll)(gen() % 9), (double)(ll)(gen() % 9), 0},
                                  {(double)(ll)(gen() % 9), (double)(ll)(gen() % 9), 0}};
        double want2 = m2[0][0] * m2[1][1] - m2[0][1] * m2[1][0];
        GA::Gauss g2(m2, 2, 2);
        if (fabs(g2.determinant - want2) > 1e-6) gDetFail++;

        vector<vector<double>> m3(3, vector<double>(4, 0));
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) m3[i][j] = (double)(ll)(gen() % 7);
        double want3 = m3[0][0] * (m3[1][1] * m3[2][2] - m3[1][2] * m3[2][1]) -
                       m3[0][1] * (m3[1][0] * m3[2][2] - m3[1][2] * m3[2][0]) +
                       m3[0][2] * (m3[1][0] * m3[2][1] - m3[1][1] * m3[2][0]);
        GA::Gauss g3(m3, 3, 3);
        if (fabs(g3.determinant - want3) > 1e-6) gDetFail++;
    }

    // ---------- GF(2) elimination against brute force ----------
    for (int it = 0; it < 4000; it++) {
        int m = 1 + (int)(gen() % 6), rowsN = 1 + (int)(gen() % 6);
        vector<pair<unsigned long long, int>> rows(rowsN);
        for (auto &r : rows) r = {(unsigned long long)(gen() % (1ULL << m)), (int)(gen() % 2)};
        GA::Gauss_XOR gx(rows, m);
        // brute force: is there an assignment satisfying every row
        bool want = false;
        for (int mask = 0; mask < (1 << m) && !want; mask++) {
            bool ok = true;
            for (auto &r : rows) {
                int parity = __builtin_popcountll(r.first & (unsigned long long)mask) & 1;
                if (parity != r.second) { ok = false; break; }
            }
            if (ok) want = true;
        }
        if (gx.solvable != want) gxFail++;
        if (want) { // the reported assignment must satisfy every row
            unsigned long long mask = 0;
            for (int j = 0; j < m; j++)
                if (gx.answer[j]) mask |= 1ULL << j;
            for (auto &r : rows) {
                int parity = __builtin_popcountll(r.first & mask) & 1;
                if (parity != r.second) gxFail++;
            }
        }
    }

    cout << "Linear_Algebra\n";
    cout << "  XOR basis rank == log2(reachable): " << rankFail << " failures (" << xbChecked << " sets)\n";
    cout << "  can_make vs subset enumeration    : " << canFail << " failures\n";
    cout << "  max_xor / min_xor                 : " << maxFail + minFail << " failures\n";
    cout << "  distinct_count                    : " << distFail << " failures\n";
    cout << "  subsets_making counts             : " << subFail << " failures\n";
    cout << "  kth_smallest walks in order       : " << kthFail << " failures\n";
    cout << "  merge of two bases                : " << mergeFail << " failures\n";
    cout << "  suffix rank vs rebuilt basis      : " << rRankFail << " failures\n";
    cout << "  suffix max_xor                    : " << rMaxFail << " failures\n";
    cout << "  suffix can_make                   : " << rCanFail << " failures\n";
    cout << "  Gauss solution substitutes back   : " << gSolFail << " failures (" << gChecked << " systems)\n";
    cout << "  Gauss reports the right case      : " << gKindFail << " failures\n";
    cout << "  determinant vs 2x2 and 3x3 forms  : " << gDetFail << " failures\n";
    cout << "  GF(2) elimination vs brute force  : " << gxFail << " failures\n";
}
