// Driver only. The judge's edges are DIRECTED, so bidirectional is passed as false. 0-based there.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    Dijkstra<ll> dij(n);
    for (int i = 0, a, b; i < m; i++) {
        ll c;
        cin >> a >> b >> c;
        dij.add_edge(a + 1, b + 1, c, false);
    }
    vector<int> p = dij.path(s + 1, t + 1);
    if (p.empty()) {
        cout << "-1\n";
        return 0;
    }
    ll total = dij.shortest_path(s + 1, t + 1);
    string out = to_string(total) + " " + to_string(p.size() - 1) + "\n";
    for (size_t i = 0; i + 1 < p.size(); i++) out += to_string(p[i] - 1) + " " + to_string(p[i + 1] - 1) + "\n";
    cout << out;
}
