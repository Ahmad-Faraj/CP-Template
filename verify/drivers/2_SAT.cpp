// Driver only. DIMACS: a negative literal means the variable must be false. Variables are 1-based
// in both the judge's format and the template.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string p, cnf;
    int n, m;
    cin >> p >> cnf >> n >> m;
    Two_SAT sat(n);
    for (int i = 0; i < m; i++) {
        int a, b, zero;
        cin >> a >> b >> zero;
        sat.add_or(abs(a), a > 0, abs(b), b > 0);
    }
    if (!sat.satisfiable()) {
        cout << "s UNSATISFIABLE\n";
        return 0;
    }
    string out = "s SATISFIABLE\nv";
    for (int v = 1; v <= n; v++) out += " " + to_string(sat.value[v] ? v : -v);
    cout << out << " 0\n";
}
