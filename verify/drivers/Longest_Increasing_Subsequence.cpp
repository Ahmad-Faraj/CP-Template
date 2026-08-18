// Driver only. The judge wants the INDICES of a longest strictly increasing subsequence.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    vector<int> idx = lis_indices(a, true);
    string out = to_string(idx.size()) + "\n";
    for (size_t i = 0; i < idx.size(); i++) out += to_string(idx[i]) + (i + 1 < idx.size() ? " " : "");
    cout << out << "\n";
}
