// Driver only. The judge wants a_0 = |S|; build() leaves z[0] = 0 by convention, so the driver prints it.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    Zfunctions z;
    z.build(s);
    string out = to_string(s.size());
    for (size_t i = 1; i < s.size(); i++) out += " " + to_string(z[(int)i]);
    cout << out << "\n";
}
