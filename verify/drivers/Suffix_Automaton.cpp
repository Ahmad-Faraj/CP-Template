// Driver only.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    Suffix_Automaton sam(s);
    cout << sam.distinct_substrings() << "\n";
}
