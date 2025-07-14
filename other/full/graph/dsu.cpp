#include <bits/stdc++.h>
using namespace std;

template <typename T = int, int Base = 1> struct DSU {

	vector<T> parent, Gsize, nxt, tail, pos, roots;

	DSU(int MaxNodes) {
		parent = Gsize = roots = tail = pos = nxt = vector<T>(MaxNodes + Base);
		for (int i = Base; i < MaxNodes + Base; i++) {
			parent[i] = roots[i] = pos[i] = tail[i] = i;
			nxt[i] = -1, Gsize[i] = 1;
		}
	}

	T find(int node) { return parent[node] = (parent[node] == node ? node : find(parent[node])); }

	void unite(int u, int v) {
        int leader_u = find(u), leader_v = find(v);
		if (leader_u == leader_v) return;
		if (Gsize[leader_u] < Gsize[leader_v]) swap(leader_u, leader_v);
		int p = pos[leader_v];
		Gsize[leader_u] += Gsize[leader_v];
		parent[leader_v] = leader_u;
		roots[p] = roots.back();
		pos[roots[p]] = p;
		roots.pop_back();
		nxt[tail[leader_u]] = leader_v;
		tail[leader_u] = tail[leader_v];
	}

    bool connected(int u, int v) { return find(u) == find(v); }

	void print() {
		for (int root = Base; root < sz(roots); root++) {
			for (int u = roots[root]; ~u; u = nxt[u]) cout << u << " \n"[!~nxt[u]];
		}
	}

	vector<vector<int>> get_components() {
		vector<vector<int>> components;
		for (int root = Base; root < sz(roots); root++) {
			vector<int> component;
			for (int u = roots[root]; ~u; u = nxt[u]) component.push_back(u);
			components.push_back(component);
		}
		return components;
	}

	int get_size(int u) { return Gsize[find(u)]; }

	int get_components_number() { return sz(roots) - Base; }
};