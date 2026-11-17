// Test for 06_Strings/Palindromic_Tree.cpp against enumerating every substring and testing it.
// Every public method / field is checked: constructor, addChar, build, propagateOccurrences,
// distinctCount, totalOccurrences, and per node: len, occ, firstPos, link, next.
#include "../../06_Strings/Palindromic_Tree.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

bool is_pal(const string &t) { return equal(t.begin(), t.end(), t.rbegin()); }

void check(PalindromicTree &pt, const string &s, const string &tag) {
    int n = s.size();
    map<string, pair<long long, int>> br; // palindrome -> {occurrences, first 1-based end}
    long long total = 0;
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++) {
            string t = s.substr(i, j - i + 1);
            if (!is_pal(t)) continue;
            total++;
            auto it = br.find(t);
            if (it == br.end()) br[t] = {1, j + 1};
            else it->second.first++, it->second.second = min(it->second.second, j + 1);
        }
    chk(tag + "distinctCount", pt.distinctCount() == (int)br.size());
    chk(tag + "totalOccurrences", pt.totalOccurrences() == total);
    int K = pt.tree.size();
    vector<string> str(K);
    str[0] = "", str[1] = "";
    map<string, int> node_of;
    bool ok_occ = true, ok_pos = true, ok_len = true, ok_set = true;
    for (int v = 2; v < K; v++) {
        auto &nd = pt.tree[v];
        ok_len &= nd.len >= 1 && nd.firstPos >= nd.len && nd.firstPos <= n;
        if (!ok_len) break;
        str[v] = s.substr(nd.firstPos - nd.len, nd.len);
        ok_set &= br.count(str[v]) && !node_of.count(str[v]);
        node_of[str[v]] = v;
        if (br.count(str[v])) {
            ok_occ &= nd.occ == br[str[v]].first;
            ok_pos &= nd.firstPos == br[str[v]].second;
        }
    }
    chk(tag + "node.len", ok_len);
    chk(tag + "node.set==brute", ok_set && (int)node_of.size() == (int)br.size());
    chk(tag + "node.occ", ok_occ);
    chk(tag + "node.firstPos", ok_pos);
    if (!ok_len) return;
    bool ok_link = pt.tree[0].len == -1 && pt.tree[1].len == 0, ok_next = true;
    for (int v = 2; v < K; v++) { // link = longest proper palindromic suffix
        const string &t = str[v];
        int want = 1;
        for (int L = t.size() - 1; L >= 1; L--)
            if (is_pal(t.substr(t.size() - L))) { want = node_of[t.substr(t.size() - L)]; break; }
        ok_link &= pt.tree[v].link == want;
    }
    for (int v = 0; v < K; v++)
        for (int c = 0; c < pt.SIGMA; c++) {
            int u = pt.tree[v].next[c];
            if (!u) continue;
            char ch = pt.BASE + c;
            string want = v == 0 ? string(1, ch) : ch + str[v] + ch;
            ok_next &= u >= 2 && str[u] == want;
        }
    chk(tag + "node.link", ok_link);
    chk(tag + "node.next", ok_next);
}

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 16, k = rng() % 3 + 1;
        string s(n, 'a');
        for (auto &c : s) c = 'a' + rng() % k;
        if (it % 2) {
            PalindromicTree pt(n, 26, 'a');
            pt.build(s);
            check(pt, s, "");
            chk("build", true);
        } else { // manual addChar + propagateOccurrences, small reserve to force reallocation
            PalindromicTree pt(0, 26, 'a');
            for (int i = 0; i < n; i++) pt.addChar(s[i], i + 1);
            pt.propagateOccurrences();
            check(pt, s, "");
            chk("addChar+propagateOccurrences", true);
        }
    }
    for (int it = 0; it < 5000; it++) { // digit alphabet
        int n = rng() % 14;
        string s(n, '0');
        for (auto &c : s) c = '0' + rng() % 10;
        if (rng() % 2) for (int i = 0; i < n / 2; i++) s[n - 1 - i] = s[i];
        PalindromicTree pt(n, 10, '0');
        pt.build(s);
        check(pt, s, "digits:");
    }
    {
        int n = 1000000;
        string s(n, 'a');
        for (auto &c : s) c = 'a' + rng() % 26;
        auto t0 = clock();
        PalindromicTree pt(n, 26, 'a');
        pt.build(s);
        string u(n, 'a');
        PalindromicTree pu(n, 26, 'a');
        pu.build(u);
        printf("n=1e6 random + all-'a' build: %.3fs (distinct %d, %d)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, pt.distinctCount(), pu.distinctCount());
        chk("large:all-a", pu.distinctCount() == n && pu.totalOccurrences() == (long long)n * (n + 1) / 2);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
