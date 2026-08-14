/* Topic: String Trie
 * Description: Trie (Prefix Tree) is a tree-like data structure that stores strings or
 *   sequences character by character. Used for fast prefix matching, dictionary
 *   implementations, and string searching.
 * Usage: Trie tr; tr.insert(s); tr.count_prefix(s); tr.erase(s);
 */
#include "../../core.h"

struct Trie {
    Trie* arr[26];
    int lst = 0;
    int st = 0;

    Trie() {
        for (int i = 0; i < 26; ++i) { arr[i] = nullptr; }
        lst = 0;
        st = 0;
    }

    ~Trie() {
        for (int i = 0; i < 26; ++i) {
            if (arr[i] != nullptr) {
                delete arr[i];
                arr[i] = nullptr;
            }
        }
    }

    void insert(const string& word, int idx = 0) {
        // Time Complexity: O(|word|)
        // Space Complexity: O(|word|)
        if (idx == sz(word)) {
            lst++;
            st++;
            return;
        }
        int ch = word[idx] - 'a';
        if (arr[ch] == nullptr) { arr[ch] = new Trie(); }
        if (idx != 0) st++;
        arr[ch]->insert(word, idx + 1);
    }

    int count_equal(const string& word, int idx = 0) const {
        // Time Complexity: O(|word|)
        if (idx == sz(word)) { return lst; }
        int ch = word[idx] - 'a';
        if (arr[ch] == nullptr) { return 0; }
        return arr[ch]->count_equal(word, idx + 1);
    }

    int count_prefix(const string& word, int idx = 0) const {
        // Time Complexity: O(|word|)
        if (idx == sz(word)) { return st; }
        int ch = word[idx] - 'a';
        if (arr[ch] == nullptr) { return 0; }
        return arr[ch]->count_prefix(word, idx + 1);
    }

    bool erase(const string& word, int idx = 0) {
        // Time Complexity: O(|word|)
        if (idx == sz(word)) {
            lst--;
            st--;
            return true;
        }
        int ch = word[idx] - 'a';
        if (arr[ch] == nullptr) { return false; }
        if (!arr[ch]->erase(word, idx + 1)) { return false; }
        if (idx != 0) st--;
        if (arr[ch]->st == 0) {
            delete arr[ch];
            arr[ch] = nullptr;
        }
        return true;
    }

    bool GetProperPrefixTo(const string& s, string& temp, vector<string>& ans, int idx = 0) {
        if (sz(s) > idx) {
            int ch = s[idx] - 'a';
            if (arr[ch] == nullptr) return false;
            temp += s[idx];
            return arr[ch]->GetProperPrefixTo(s, temp, ans, idx + 1);
        } else {
            for (int i = 0; idx != sz(s) && i < lst; i++) {
                ans.emplace_back(temp);
                break;
            }
            for (int i = 0; i < 26; i++) {
                if (arr[i]) {
                    temp += (char)(i + 'a');
                    bool x = arr[i]->GetProperPrefixTo(s, temp, ans, idx + 1);
                    temp.pop_back();
                }
            }
            return true;
        }
    }
};
