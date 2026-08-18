// Brute-force test for:
//   Graph/2_SAT.cpp
//   Graph/Strong_Orientation.cpp
//   Graph/Connectivity/Strongly_Connected_Components.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;

namespace TS {
#include "../Graph/2_SAT.cpp"
}
namespace SO {
#include "../Graph/Strong_Orientation.cpp"
}
namespace SC {
#include "../Graph/Connectivity/Strongly_Connected_Components.cpp"
}

int main() {
    mt19937 gen(777);
    int satFail = 0, assignFail = 0, unsatFail = 0;
    int soCountFail = 0, soStrongFail = 0, soEdgeFail = 0, soOrientFail = 0;
    int satChecked = 0, satTrue = 0, soChecked = 0;

    // ---------- 2-SAT against exhaustive assignment search ----------
    for (int it = 0; it < 4000; it++) {
        int n = 1 + (int)(gen() % 7);
        int m = (int)(gen() % 12);
        TS::Two_SAT sat(n);
        vector<array<int, 4>> clauses; // a, af, b, bf
        for (int i = 0; i < m; i++) {
            int a = 1 + (int)(gen() % n), b = 1 + (int)(gen() % n);
            int af = (int)(gen() % 2), bf = (int)(gen() % 2);
            sat.add_or(a, af, b, bf);
            clauses.push_back({a, af, b, bf});
        }
        // brute force: is there any assignment satisfying every clause
        bool wantSat = false;
        int witness = -1;
        for (int mask = 0; mask < (1 << n) && !wantSat; mask++) {
            bool ok = true;
            for (auto &c : clauses) {
                int va = (mask >> (c[0] - 1)) & 1, vb = (mask >> (c[2] - 1)) & 1;
                if (!(va == c[1] || vb == c[3])) { ok = false; break; }
            }
            if (ok) wantSat = true, witness = mask;
        }
        (void)witness;
        bool got = sat.satisfiable();
        if (got != wantSat) satFail++;
        satChecked++;
        if (got) {
            satTrue++;
            // the assignment it produced must actually satisfy every clause
            for (auto &c : clauses) {
                int va = sat.value[c[0]], vb = sat.value[c[2]];
                if (!(va == c[1] || vb == c[3])) assignFail++;
            }
        }
    }

    // ---------- 2-SAT: the other constraint helpers ----------
    for (int it = 0; it < 3000; it++) {
        int n = 2 + (int)(gen() % 5);
        TS::Two_SAT sat(n);
        vector<array<int, 3>> cons; // kind, a, b   kind 0 equal, 1 not equal, 2 at most one true
        vector<pair<int, int>> forced;
        int m = (int)(gen() % 6);
        for (int i = 0; i < m; i++) {
            int kind = (int)(gen() % 3);
            int a = 1 + (int)(gen() % n), b = 1 + (int)(gen() % n);
            if (kind == 0) sat.add_equal(a, b);
            else if (kind == 1) sat.add_not_equal(a, b);
            else sat.at_most_one_true(a, b);
            cons.push_back({kind, a, b});
        }
        if (gen() % 2) {
            int a = 1 + (int)(gen() % n), af = (int)(gen() % 2);
            sat.force(a, af);
            forced.push_back({a, af});
        }
        bool wantSat = false;
        for (int mask = 0; mask < (1 << n) && !wantSat; mask++) {
            bool ok = true;
            for (auto &c : cons) {
                int va = (mask >> (c[1] - 1)) & 1, vb = (mask >> (c[2] - 1)) & 1;
                if (c[0] == 0 && va != vb) ok = false;
                if (c[0] == 1 && va == vb) ok = false;
                if (c[0] == 2 && va && vb) ok = false;
            }
            for (auto &f : forced)
                if (((mask >> (f.first - 1)) & 1) != f.second) ok = false;
            if (ok) wantSat = true;
        }
        bool got = sat.satisfiable();
        if (got != wantSat) unsatFail++;
        if (got) {
            for (auto &c : cons) {
                int va = sat.value[c[1]], vb = sat.value[c[2]];
                if (c[0] == 0 && va != vb) assignFail++;
                if (c[0] == 1 && va == vb) assignFail++;
                if (c[0] == 2 && va && vb) assignFail++;
            }
            for (auto &f : forced)
                if (sat.value[f.first] != f.second) assignFail++;
        }
    }

    // ---------- Strong orientation: apply the orientation, then count SCCs with the verified SCC template ----------
    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 8);
        int m = (int)(gen() % 12);
        SO::Strong_Orientation so(n);
        vector<pair<int, int>> es;
        for (int i = 0; i < m; i++) {
            int u = 1 + (int)(gen() % n), v = 1 + (int)(gen() % n);
            so.add_edge(u, v);
            es.push_back({u, v});
        }
        so.run();
        soChecked++;

        // build the directed graph it chose and count SCCs independently
        SC::Strongly_Connected_Components scc(n);
        for (int i = 0; i < m; i++) {
            auto [a, b] = so.directed_edge(i);
            // the oriented edge must be the same undirected edge
            if (!((a == es[i].first && b == es[i].second) || (a == es[i].second && b == es[i].first))) soEdgeFail++;
            scc.add_edge(a, b);
        }
        scc.run();
        if (scc.component_count() != so.min_scc_count()) soCountFail++;

        // when connected and bridgeless the result must be one single SCC
        if (so.is_strongly_orientable() && scc.component_count() != 1) soStrongFail++;

        // the orientation string must agree with directed_edge
        string s = so.orientation();
        if ((int)s.size() != m) soOrientFail++;
        for (int i = 0; i < m; i++) {
            auto [a, b] = so.directed_edge(i);
            bool kept = (a == es[i].first && b == es[i].second);
            // for a self loop both readings are the same, so skip that case
            if (es[i].first == es[i].second) continue;
            if ((s[i] == '>') != kept) soOrientFail++;
        }
    }

    // ---------- fixed cases ----------
    {
        // (x1) and (not x1) is unsatisfiable
        TS::Two_SAT a(1);
        a.force(1, true);
        a.force(1, false);
        if (a.satisfiable()) unsatFail++;
        // a triangle is bridgeless and connected, so one SCC
        SO::Strong_Orientation t(3);
        t.add_edge(1, 2);
        t.add_edge(2, 3);
        t.add_edge(3, 1);
        t.run();
        if (!t.is_strongly_orientable() || t.min_scc_count() != 1) soStrongFail++;
        // a single edge is a bridge, so two pieces
        SO::Strong_Orientation b(2);
        b.add_edge(1, 2);
        b.run();
        if (b.is_strongly_orientable() || b.min_scc_count() != 2) soStrongFail++;
    }

    cout << "2_SAT + Strong_Orientation\n";
    cout << "  2-SAT verdict vs brute force   : " << satFail << " failures (" << satChecked << " cases, " << satTrue
         << " satisfiable)\n";
    cout << "  2-SAT assignment satisfies all : " << assignFail << " failures\n";
    cout << "  2-SAT helper constraints       : " << unsatFail << " failures\n";
    cout << "  orientation SCC count minimal  : " << soCountFail << " failures (" << soChecked << " graphs)\n";
    cout << "  bridgeless+connected -> 1 SCC  : " << soStrongFail << " failures\n";
    cout << "  oriented edges are input edges : " << soEdgeFail << " failures\n";
    cout << "  orientation string consistent  : " << soOrientFail << " failures\n";
}
