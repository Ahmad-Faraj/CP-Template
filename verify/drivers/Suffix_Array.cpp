// Driver only. The template appends a '$' sentinel, so p[0] is that sentinel and the array is p[1..n-1].
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    SuffixArray sa(s);
    string out;
    for (int i = 1; i < sa.n; i++) out += to_string(sa.p[i]) + (i + 1 < sa.n ? " " : "\n");
    cout << out;
}
