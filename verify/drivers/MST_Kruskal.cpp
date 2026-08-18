// Driver only. Edge ids are assignment order, which matches the judge's numbering. 0-based there.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    MST_Kruskal mst(n);
    for (int i = 0, a, b; i < m; i++) {
        ll c;
        cin >> a >> b >> c;
        mst.add_edge(a + 1, b + 1, c);
    }
    ll total = mst.build();
    string out = to_string(total) + "\n";
    vector<int> ids = mst.edge_ids();
    for (size_t i = 0; i < ids.size(); i++) out += to_string(ids[i]) + (i + 1 < ids.size() ? " " : "");
    cout << out << "\n";
}
