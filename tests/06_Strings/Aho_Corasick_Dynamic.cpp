// Test for 06_Strings/Aho_Corasick_Dynamic.cpp against naive substring matching over the live patterns.
// Every public method is checked: aho_corasick insert, delete_word (present, missing, duplicate copies),
// count, get_occ; aho_corasick_node constructor, count_occurrences, get_occ, empty pattern list.
#include "../../06_Strings/Aho_Corasick_Dynamic.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

string rnd_str(int len, int alpha) {
    string s(len, 'a');
    for (auto &c : s) c = char('a' + rng() % alpha);
    return s;
}
vector<int> ends_of(const string &p, const string &t) {
    vector<int> r;
    for (int i = 0; i + (int)p.size() <= (int)t.size(); i++)
        if (t.compare(i, p.size(), p) == 0) r.push_back(i + (int)p.size() - 1);
    return r;
}

int main() {
    for (int it = 0; it < 1500; it++) {
        int alpha = vector<int>{1, 2, 3, 26}[rng() % 4];
        int maxlen = 1 + rng() % 5;
        aho_corasick ac;
        multiset<string> live;   // oracle: live patterns with multiplicity
        vector<string> id_word;  // id -> word (ids assigned in insertion order)
        int ops = rng() % 80;
        for (int op = 0; op < ops; op++) {
            int t = rng() % 5;
            if (t <= 1) {
                string p = rnd_str(1 + rng() % maxlen, alpha);
                ac.insert(p), live.insert(p), id_word.push_back(p);
                chk("insert", true);
            } else if (t == 2) {
                // delete: usually a live word, sometimes a missing one
                string p;
                if (!live.empty() && rng() % 4) {
                    auto itw = live.begin();
                    advance(itw, rng() % live.size());
                    p = *itw;
                } else p = rnd_str(1 + rng() % maxlen, alpha);
                ac.delete_word(p);
                auto f = live.find(p);
                if (f != live.end()) live.erase(f);
                chk("delete_word", true);
            }
            string txt = rnd_str(rng() % 25, alpha);
            long long want = 0;
            for (auto &p : live) want += ends_of(p, txt).size();
            chk("count", ac.count(txt) == want);
            // get_occ: per word, ids holding it are either the full match list or empty (deleted),
            // and exactly live.count(word) of them are alive when the word occurs in txt.
            auto occ = ac.get_occ(txt);
            bool ok = occ.size() == id_word.size();
            map<string, int> alive_hits;
            long long total = 0;
            for (size_t id = 0; ok && id < id_word.size(); id++) {
                auto e = ends_of(id_word[id], txt);
                ok &= occ[id].empty() || occ[id] == e;
                if (!occ[id].empty()) alive_hits[id_word[id]]++;
                total += occ[id].size();
            }
            for (auto &w : set<string>(id_word.begin(), id_word.end()))
                if (!ends_of(w, txt).empty()) ok &= alive_hits[w] == (int)live.count(w);
            ok &= total == want;
            chk("get_occ", ok);
        }
        chk("count(empty text)", ac.count("") == 0);
    }
    // aho_corasick_node used directly (static pattern set, arbitrary ids)
    for (int it = 0; it < 3000; it++) {
        int alpha = 1 + rng() % 4;
        int k = rng() % 8;
        vector<pair<string, int>> pats;
        for (int i = 0; i < k; i++) pats.emplace_back(rnd_str(1 + rng() % 4, alpha), i);
        aho_corasick_node nd(pats);
        string txt = rnd_str(rng() % 30, alpha);
        long long want = 0;
        vector<vector<int>> got(k);
        nd.get_occ(txt, got);
        bool ok = true;
        for (auto &[p, id] : pats) {
            auto e = ends_of(p, txt);
            want += e.size();
            ok &= got[id] == e;
        }
        chk("node:count_occurrences", nd.count_occurrences(txt) == want);
        chk("node:get_occ", ok);
    }
    {
        aho_corasick_node e(vector<pair<string, int>>{});
        chk("node:empty", e.count_occurrences("abc") == 0);
        aho_corasick ac;
        ac.delete_word("abc");
        chk("delete_word(empty)", ac.count("abcabc") == 0 && ac.get_occ("abc").empty());
    }
    {
        // 1e5 inserts (total length ~1e6) interleaved with 1e5 queries (total length ~1e6), 26 letters
        auto t0 = clock();
        aho_corasick ac;
        long long sum = 0;
        for (int i = 0; i < 100000; i++) {
            ac.insert(rnd_str(1 + rng() % 19, 26));
            sum += ac.count(rnd_str(1 + rng() % 19, 26));
        }
        printf("1e5 inserts + 1e5 counts (lengths <= 19): %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, sum % 7);
        t0 = clock();
        for (int i = 0; i < 20; i++) ac.delete_word(rnd_str(1 + rng() % 2, 26)); // all hit
        printf("then 20 delete_word hits on 1e5 words (slow by design): %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
    }
    // the header's many-deletes recipe: count = ac.count(t) - del.count(t), live words only
    for (int it = 0; it < 500; it++) {
        int alpha = 1 + rng() % 3;
        aho_corasick ac, del;
        multiset<string> live;
        for (int op = 0; op < 40; op++) {
            if (live.empty() || rng() % 3) {
                string p = rnd_str(1 + rng() % 4, alpha);
                ac.insert(p), live.insert(p);
            } else {
                auto itw = live.begin();
                advance(itw, rng() % live.size());
                del.insert(*itw), live.erase(itw);
            }
            string txt = rnd_str(rng() % 25, alpha);
            long long want = 0;
            for (auto &p : live) want += ends_of(p, txt).size();
            chk("two-instance delete recipe", ac.count(txt) - del.count(txt) == want);
        }
    }
    {
        // 1e5 ops with that recipe: 6e4 inserts, 2e4 deletes, 2e4 counts, lengths <= 19
        auto t0 = clock();
        aho_corasick ac, del;
        vector<string> words;
        long long sum = 0;
        for (int i = 0; i < 100000; i++) {
            int t = rng() % 5;
            if (t < 3 || words.empty()) words.push_back(rnd_str(1 + rng() % 19, 26)), ac.insert(words.back());
            else if (t == 3) {
                swap(words[rng() % words.size()], words.back());
                del.insert(words.back()), words.pop_back();
            } else {
                string txt = rnd_str(1 + rng() % 19, 26);
                sum += ac.count(txt) - del.count(txt);
            }
        }
        printf("1e5 ops via two-instance recipe: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, sum % 7);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
