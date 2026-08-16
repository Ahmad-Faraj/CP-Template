#include "../../core.h"

int nextNodeId = 0;
struct AhoCorasick {
    struct Node {
        Node *fail;
        Node *child[26];
        vector<int> patIdx;
        vector<char> chars;
        int id;
        Node() {
            memset(child, 0, sizeof(child));
            fail = 0;
            id = nextNodeId++;
        }
    };
    Node *root;
    
    AhoCorasick() {
        root = new Node();
    }
    
    AhoCorasick(vector<string> &patterns) {
        root = new Node();
        for(int i = 0; i < sz(patterns); i++){
            insert(patterns[i], i);
        }
        build();
    }
    
    void insert(string &s, int idx) {
        Node *cur = root;
        for(int i = 0; i < sz(s); i++){
            int c = s[i] - 'a';
            if(!cur->child[c]){
                cur->child[c] = new Node();
                cur->chars.push_back(c);
            }
            cur = cur->child[c];
        }
        cur->patIdx.push_back(idx);
    }
    
    void build() {
        queue<Node*> q;
        for(int i = 0; i < 26; i++){
            if(root->child[i]){
                root->child[i]->fail = root;
                q.push(root->child[i]);
            } else {
                root->child[i] = root;
            }
        }
        while(!q.empty()){
            Node *cur = q.front();
            q.pop();
            for(int i = 0; i < sz(cur->chars); i++){
                int c = cur->chars[i];
                Node *next = cur->child[c];
                Node *fail = cur->fail;
                while(fail != root && !fail->child[c]){
                    fail = fail->fail;
                }
                if(fail->child[c]){
                    next->fail = fail->child[c];
                } else {
                    next->fail = root;
                }
                for(int j = 0; j < sz(next->fail->patIdx); j++){
                    next->patIdx.push_back(next->fail->patIdx[j]);
                }
                q.push(next);
            }
        }
    }
    
    vector<vector<int>> search(string &s) {
        vector<vector<int>> res(sz(s));
        Node *cur = root;
        for(int i = 0; i < sz(s); i++){
            int c = s[i] - 'a';
            while(cur != root && !cur->child[c]){
                cur = cur->fail;
            }
            if(cur->child[c]){
                cur = cur->child[c];
            }
            for(int j = 0; j < sz(cur->patIdx); j++){
                res[i].push_back(cur->patIdx[j]);
            }
        }
        return res;
    }
};
