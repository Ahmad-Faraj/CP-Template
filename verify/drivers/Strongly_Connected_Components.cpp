// Driver only. Tarjan finishes sinks first, so `components` is in reverse topological order and the
// judge (which requires topological order) needs it reversed. Vertices are 0-based there, 1-based here.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    Strongly_Connected_Components scc(n);
    for (int i = 0, a, b; i < m; i++) {
        cin >> a >> b;
        scc.add_edge(a + 1, b + 1);
    }
    scc.run();
    string out = to_string(scc.component_count()) + "\n";
    for (int i = (int)scc.components.size() - 1; i >= 0; i--) {
        out += to_string(scc.components[i].size());
        for (int v : scc.components[i]) out += " " + to_string(v - 1);
        out += "\n";
    }
    cout << out;
}
