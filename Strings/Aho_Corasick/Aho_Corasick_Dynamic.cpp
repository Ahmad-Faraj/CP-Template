#include "../../core.h"
#define all(x) x.begin(), x.end()

class aho_corasick_node {
private:
  struct node {
    vector < int > occ;
    int child[26], cnt, link;
    node() : cnt(0), link(-1) { 
      memset(child, -1, sizeof child); 
    }
  };
  vector < node > tree;

public:
  aho_corasick_node() = default;

  aho_corasick_node(const vector < pair < string, int > >& patterns) {
    tree = vector < node > (1);
    for (auto [s, idx] : patterns) {
      int cur = 0;
      for (char c : s) {
        int to = c - 'a';
        if (tree[cur].child[to] == -1) {
          tree[cur].child[to] = sz(tree);
          tree.emplace_back(node());
        }
        cur = tree[cur].child[to];
      }
      tree[cur].cnt++;
      tree[cur].occ.push_back(idx);
    }
    build_links();
  }
  
  inline void build_links(){
    queue < int > q;
    q.push(0);
    
    while(sz(q)){
      int node = q.front();
      q.pop();

      for (int i = 0; i < 26; i++) {
        int v = tree[node].child[i];
        if (~v){
          int j = tree[node].link;
          while(~j and !~tree[j].child[i]) j = tree[j].link;
          
          tree[v].link = (~j ? tree[j].child[i] : 0);
          tree[v].cnt += tree[tree[v].link].cnt;
          tree[v].occ.insert(tree[v].occ.end(), all(tree[tree[v].link].occ));

          q.push(v);
        }
      }
    }
  }

  long long count_occurrences(const string &s){
    long long ans = 0, cur = 0;
    for (char c : s) {
      int to = c - 'a';
      while (~cur and !~tree[cur].child[to]) cur = tree[cur].link;
      cur = (~cur ? tree[cur].child[to] : 0);
      ans += tree[cur].cnt;
    }
    return ans;
  }
  
  inline void get_occ(const string& s, vector < vector < int > >& ret){
    int cur = 0;
    for (int i = 0; i < sz(s); i++) {
      int to = s[i] - 'a';
      while (~cur and !~tree[cur].child[to]) cur = tree[cur].link;
      cur = (~cur ? tree[cur].child[to] : 0);
      for (int idx : tree[cur].occ) ret[idx].push_back(i);
    }
  } 
};

class aho_corasick {
private:  
  int curr_size = 0;
  static const int LOG = 20;
  aho_corasick_node aho[LOG];
  vector < pair < string, int > > list[LOG];
  
public:
  inline int get_nxt(){
    for (int i = 0; i < LOG; i++) 
      if (list[i].empty()) return i;
    return 0;
  }
  
  inline void insert(const string& s){
    int idx = get_nxt();
    for (int bef = 0; bef < idx; bef++) {
      for (auto & it : list[bef]) {
        list[idx].push_back(it);
      }
      list[bef].clear();
      aho[bef] = aho_corasick_node();
    }
    list[idx].emplace_back(s, curr_size++);
    aho[idx] = aho_corasick_node(list[idx]);
  }

  inline long long count(const string &s){
    long long ans = 0;
    for (int i = 0; i < LOG; i++) 
      if (!list[i].empty()) ans += aho[i].count_occurrences(s);
    return ans;
  }

  vector < vector < int > > get_occ(const string& s){
    vector < vector < int > > ans(curr_size);
    for (int i = 0; i < LOG; i++) {
      if (!list[i].empty()) aho[i].get_occ(s, ans);
    }
    return ans;
  }

  inline void delete_word(const string& s) {
    for (int l = 0; l < LOG; l++) {
      auto it = find_if(all(list[l]), [&](const pair<string, int>& p) { return p.first == s; });
      if (it != list[l].end()) {
        list[l].erase(it);
        aho[l] = aho_corasick_node(list[l]);
        break;
      }
    }
  }
};
