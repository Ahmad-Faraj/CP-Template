// Driver only. This problem's modulus is 1e9+7, which is not NTT friendly - the point of this file.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<ll> a(n), b(m);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    vector<ll> c = multiply(a, b, 1000000007);
    string out;
    for (size_t i = 0; i < c.size(); i++) out += to_string(c[i]) + (i + 1 < c.size() ? " " : "");
    cout << out << "\n";
}
