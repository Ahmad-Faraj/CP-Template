// Brute-force test for:
//   Graph/Eulerian_Path.cpp
//   Range_Query/Fenwick_Tree_2D.cpp
//   Math/Polynomial/Lagrange_Interpolation.cpp
//   Strings/Suffix_Automaton.cpp
//   Math/Number_Theory/Sieve.cpp
//   Math/Polynomial/NTT.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace EU {
#include "../Graph/Eulerian_Path.cpp"
}
namespace F2 {
#include "../Range_Query/Fenwick_Tree_2D.cpp"
}
namespace LI {
#include "../DP/Longest_Increasing_Subsequence.cpp"
}
namespace SA {
#include "../Strings/Suffix_Automaton.cpp"
}
namespace SV {
#include "../Math/Number_Theory/Sieve.cpp"
}
namespace NT {
#include "../Math/Polynomial/NTT.cpp"
}

int main() {
    mt19937_64 gen(777333);
    int euFail = 0, euNoneFail = 0, f2Fail = 0, liFail = 0, liValFail = 0;
    int samDistinctFail = 0, samOccFail = 0, samLcsFail = 0, samPosFail = 0;
    int segFail = 0, rootFail = 0;
    int euChecked = 0, samChecked = 0;

    // ---------- Eulerian path: the returned walk must use every edge exactly once ----------
    for (int it = 0; it < 6000; it++) {
        int n = 1 + (int)(gen() % 6), m = (int)(gen() % 8);
        bool directed = (it % 2 == 0);
        EU::Eulerian_Path g(n, directed);
        vector<pair<int, int>> es;
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            g.add_edge(u, v);
            es.push_back({u, v});
        }
        vector<int> p = g.path();
        euChecked++;
        if (!p.empty()) {
            if ((int)p.size() != m + 1) { euFail++; continue; }
            // every consecutive pair is an edge, and each edge is consumed exactly once
            vector<char> used(m, 0);
            bool ok = true;
            for (size_t i = 0; i + 1 < p.size() && ok; i++) {
                bool found = false;
                for (int e = 0; e < m && !found; e++) {
                    if (used[e]) continue;
                    if (es[e].first == p[i] && es[e].second == p[i + 1]) found = true, used[e] = 1;
                    else if (!directed && es[e].second == p[i] && es[e].first == p[i + 1]) found = true, used[e] = 1;
                }
                if (!found) ok = false;
            }
            if (!ok) euFail++;
            for (int e = 0; e < m; e++)
                if (!used[e]) euFail++;
        } else if (m > 0) {
            // claimed impossible - verify by brute force over permutations of the edges for tiny m
            if (m <= 6) {
                vector<int> order(m);
                iota(order.begin(), order.end(), 0);
                bool exists = false;
                do {
                    for (int flip = 0; flip < (directed ? 1 : (1 << m)) && !exists; flip++) {
                        vector<pair<int, int>> seq;
                        for (int i = 0; i < m; i++) {
                            auto e = es[order[i]];
                            if (!directed && (flip >> i & 1)) swap(e.first, e.second);
                            seq.push_back(e);
                        }
                        bool chain = true;
                        for (int i = 0; i + 1 < m; i++)
                            if (seq[i].second != seq[i + 1].first) chain = false;
                        if (chain) exists = true;
                    }
                } while (!exists && next_permutation(order.begin(), order.end()));
                if (exists) euNoneFail++;
            }
        }
    }

    // ---------- Fenwick 2D against a plain grid ----------
    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 7), m = 1 + (int)(gen() % 7);
        vector<vector<ll>> grid(n + 1, vector<ll>(m + 1, 0));
        F2::Fenwick_Tree_2D<ll> bit(n, m);
        for (int r = 0; r < 25; r++) {
            int x = 1 + (int)(gen() % n), y = 1 + (int)(gen() % m);
            if (gen() % 3 == 0) {
                ll v = (ll)(gen() % 41) - 20;
                bit.set(x, y, v);
                grid[x][y] = v;
            } else {
                ll d = (ll)(gen() % 21) - 10;
                bit.add(x, y, d);
                grid[x][y] += d;
            }
            int x1 = 1 + (int)(gen() % n), x2 = 1 + (int)(gen() % n);
            int y1 = 1 + (int)(gen() % m), y2 = 1 + (int)(gen() % m);
            if (x1 > x2) swap(x1, x2);
            if (y1 > y2) swap(y1, y2);
            ll want = 0;
            for (int i = x1; i <= x2; i++)
                for (int j = y1; j <= y2; j++) want += grid[i][j];
            if (bit.sum(x1, y1, x2, y2) != want) f2Fail++;
            if (bit.get(x, y) != grid[x][y]) f2Fail++;
        }
    }

    // ---------- LIS against the O(n^2) DP, both strict and non-decreasing ----------
    for (int it = 0; it < 20000; it++) {
        int n = (int)(gen() % 13);
        vector<int> a(n);
        for (int &x : a) x = (int)(gen() % 9) - 4;
        for (int strict = 0; strict < 2; strict++) {
            vector<int> dp(n, 1);
            int want = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < i; j++)
                    if (strict ? a[j] < a[i] : a[j] <= a[i]) dp[i] = max(dp[i], dp[j] + 1);
                want = max(want, dp[i]);
            }
            if (LI::lis_length(a, strict) != want) liFail++;
            vector<int> idx = LI::lis_indices(a, strict);
            if ((int)idx.size() != want) liValFail++;
            // the reported indices must be ascending and their values genuinely increasing
            for (size_t i = 0; i + 1 < idx.size(); i++) {
                if (idx[i] >= idx[i + 1]) liValFail++;
                if (strict ? !(a[idx[i]] < a[idx[i + 1]]) : !(a[idx[i]] <= a[idx[i + 1]])) liValFail++;
            }
        }
    }

    // ---------- suffix automaton against brute-force substring sets ----------
    for (int it = 0; it < 3000; it++) {
        int n = (int)(gen() % 9);
        string s;
        for (int i = 0; i < n; i++) s += char('a' + gen() % 3);
        SA::Suffix_Automaton sam(s);
        samChecked++;

        set<string> subs;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j <= n; j++) subs.insert(s.substr(i, j - i));
        if (sam.distinct_substrings() != (ll)subs.size()) samDistinctFail++;

        // occurrences and first position for every candidate up to length 4
        for (int i = 0; i < n; i++)
            for (int len = 1; len <= 4 && i + len <= n; len++) {
                string p = s.substr(i, len);
                ll want = 0;
                int firstAt = -1;
                for (int j = 0; j + len <= n; j++)
                    if (s.compare(j, len, p) == 0) {
                        want++;
                        if (firstAt == -1) firstAt = j;
                    }
                if (sam.occurrences(p) != want) samOccFail++;
                if (sam.first_occurrence(p) != firstAt) samPosFail++;
            }
        // a pattern that is not there
        if (n && sam.occurrences("zz") != 0) samOccFail++;
        if (n && sam.contains("zz")) samOccFail++;

        // longest common substring against brute force
        string t;
        for (int i = 0; i < (int)(gen() % 8); i++) t += char('a' + gen() % 3);
        string best;
        for (size_t i = 0; i < t.size(); i++)
            for (size_t j = i + 1; j <= t.size(); j++) {
                string cand = t.substr(i, j - i);
                if (cand.size() > best.size() && s.find(cand) != string::npos) best = cand;
            }
        string got = sam.longest_common_substring(t);
        if (got.size() != best.size()) samLcsFail++;
        else if (!got.empty() && (s.find(got) == string::npos || t.find(got) == string::npos)) samLcsFail++;
    }

    // ---------- segmented sieve against the plain sieve ----------
    for (int it = 0; it < 400; it++) {
        ll lo = (ll)(gen() % 5000), hi = lo + (ll)(gen() % 400);
        vector<ll> got = SV::segmented_sieve(lo, hi);
        vector<ll> want;
        for (ll v = max(lo, 2LL); v <= hi; v++)
            if (SV::is_prime(v)) want.push_back(v);
        if (got != want) segFail++;
    }
    {
        // a window far out, where sieving from zero is not an option
        ll lo = 1000000000000LL, hi = lo + 300;
        vector<ll> got = SV::segmented_sieve(lo, hi);
        for (ll p : got)
            if (!SV::is_prime(p)) segFail++;
        ll count = 0;
        for (ll v = lo; v <= hi; v++)
            if (SV::is_prime(v)) count++;
        if ((ll)got.size() != count) segFail++;
    }

    // ---------- primitive root ----------
    for (ll p : {(ll)3, (ll)5, (ll)7, (ll)11, (ll)13, (ll)97, (ll)998244353, (ll)1000000007}) {
        ll g = NT::primitive_root(p);
        if (g < 2) { rootFail++; continue; }
        // g^((p-1)/f) must differ from 1 for every prime factor f of p-1
        ll m = p - 1;
        vector<ll> fs;
        for (ll f = 2; f * f <= m; f++)
            if (m % f == 0) {
                fs.push_back(f);
                while (m % f == 0) m /= f;
            }
        if (m > 1) fs.push_back(m);
        for (ll f : fs)
            if (NT::pow_mod(g, (p - 1) / f, p) == 1) rootFail++;
        if (NT::pow_mod(g, p - 1, p) != 1) rootFail++;
    }
    if (NT::primitive_root(998244353) != 3) rootFail++; // the one NTT.cpp hardcodes

    printf("New templates\n");
    printf("  Eulerian walk uses every edge : %d failures (%d graphs)\n", euFail, euChecked);
    printf("  Eulerian 'impossible' verdict : %d failures\n", euNoneFail);
    printf("  Fenwick 2D vs a plain grid    : %d failures\n", f2Fail);
    printf("  LIS length vs O(n^2) DP       : %d failures\n", liFail);
    printf("  LIS reconstruction valid      : %d failures\n", liValFail);
    printf("  SAM distinct substrings       : %d failures (%d strings)\n", samDistinctFail, samChecked);
    printf("  SAM occurrence counts         : %d failures\n", samOccFail);
    printf("  SAM first occurrence          : %d failures\n", samPosFail);
    printf("  SAM longest common substring  : %d failures\n", samLcsFail);
    printf("  segmented sieve               : %d failures\n", segFail);
    printf("  primitive_root                : %d failures\n", rootFail);
}
