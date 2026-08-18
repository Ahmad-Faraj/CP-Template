// Brute-force test for:
//   Range_Query/Parallel_Binary_Search.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace PB {
#include "../Range_Query/Parallel_Binary_Search.cpp"
}

int main() {
    mt19937_64 gen(515151);
    int genericFail = 0, meteorFail = 0, neverFail = 0, roundFail = 0;
    int checked = 0, meteorChecked = 0;

    // ---------- generic: thresholds over a running total ----------
    for (int it = 0; it < 5000; it++) {
        int events = 1 + (int)(gen() % 12), queries = 1 + (int)(gen() % 8);
        vector<ll> delta(events + 1);
        for (int t = 1; t <= events; t++) delta[t] = (ll)(gen() % 6);
        vector<ll> need(queries);
        for (ll &w : need) w = (ll)(gen() % 30);

        ll total = 0;
        vector<int> got = PB::parallel_binary_search(
            queries, events, [&]() { total = 0; }, [&](int t) { total += delta[t]; },
            [&](int i) { return total >= need[i]; });
        checked++;

        // brute force: replay for each query independently
        for (int i = 0; i < queries; i++) {
            ll run = 0;
            int want = -1;
            for (int t = 1; t <= events && want == -1; t++) {
                run += delta[t];
                if (run >= need[i]) want = t;
            }
            if (got[i] != want) genericFail++;
            if (want == -1 && got[i] != -1) neverFail++;
        }
    }

    // ---------- the reset/apply contract: apply must only run forwards from a reset ----------
    for (int it = 0; it < 400; it++) {
        int events = 1 + (int)(gen() % 10), queries = 1 + (int)(gen() % 5);
        int last = 0;
        bool ordered = true;
        vector<ll> v(events + 1, 1);
        ll total = 0;
        vector<ll> need(queries);
        for (ll &w : need) w = (ll)(gen() % (events + 2));
        PB::parallel_binary_search(
            queries, events,
            [&]() {
                total = 0;
                last = 0;
            },
            [&](int t) {
                if (t != last + 1) ordered = false; // must be strictly sequential after a reset
                last = t;
                total += v[t];
            },
            [&](int i) { return total >= need[i]; });
        if (!ordered) roundFail++;
    }

    // ---------- POI Meteors, against a direct simulation ----------
    for (int it = 0; it < 1500; it++) {
        int n = 1 + (int)(gen() % 4), m = 1 + (int)(gen() % 7), q = 1 + (int)(gen() % 7);
        vector<vector<int>> owned(n);
        vector<int> owner(m + 1);
        for (int i = 1; i <= m; i++) {
            owner[i] = (int)(gen() % n);
            owned[owner[i]].push_back(i);
        }
        vector<ll> want(n);
        for (ll &w : want) w = (ll)(gen() % 12);
        vector<array<ll, 3>> fall(q + 1);
        for (int i = 1; i <= q; i++) {
            ll l = 1 + (ll)(gen() % m), r = 1 + (ll)(gen() % m), v = 1 + (ll)(gen() % 4);
            fall[i] = {l, r, v};
        }

        PB::Fenwick bit(m);
        vector<int> got = PB::parallel_binary_search(
            n, q, [&]() { bit.init(m); },
            [&](int t) {
                ll l = fall[t][0], r = fall[t][1], v = fall[t][2];
                if (l <= r) bit.add_range((int)l, (int)r, v);
                else bit.add_range((int)l, m, v), bit.add_range(1, (int)r, v);
            },
            [&](int s) {
                ll total = 0;
                for (int sec : owned[s]) total += bit.point(sec);
                return total >= want[s];
            });
        meteorChecked++;

        // simulate the sectors directly
        vector<ll> sector(m + 1, 0);
        vector<int> wantAns(n, -1);
        for (int t = 1; t <= q; t++) {
            ll l = fall[t][0], r = fall[t][1], v = fall[t][2];
            if (l <= r)
                for (ll s = l; s <= r; s++) sector[s] += v;
            else {
                for (ll s = l; s <= m; s++) sector[s] += v;
                for (ll s = 1; s <= r; s++) sector[s] += v;
            }
            for (int s = 0; s < n; s++) {
                if (wantAns[s] != -1) continue;
                ll total = 0;
                for (int sec : owned[s]) total += sector[sec];
                if (total >= want[s]) wantAns[s] = t;
            }
        }
        for (int s = 0; s < n; s++)
            if (got[s] != wantAns[s]) meteorFail++;
    }

    printf("Parallel_Binary_Search\n");
    printf("  generic vs per-query replay   : %d failures (%d cases)\n", genericFail, checked);
    printf("  never-satisfied returns -1    : %d failures\n", neverFail);
    printf("  apply() only runs forwards    : %d failures\n", roundFail);
    printf("  POI Meteors vs simulation     : %d failures (%d cases)\n", meteorFail, meteorChecked);
}
