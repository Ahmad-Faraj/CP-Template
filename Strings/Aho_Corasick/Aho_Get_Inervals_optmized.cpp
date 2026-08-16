#include "../../core.h"

int nextNodeId = 0;

struct Aho
{
    struct Node
    {
        Node* fail;
        Node* child[26];

        // FIX 1: Replace vector<int> with a single int. A trie node represents one unique string!
        // Using a vector here and copying it in build() causes massive Memory Limit Exceeded.
        int patIdx;
        vector<char> cahrs;
        int id;
        Node()
        {
            memset(child, 0, sizeof child);
            id = nextNodeId++;
            patIdx = -1; // FIX 1: Initialize with -1
        }
        ~Node()
        {
            for (auto c : cahrs)
            {
                if (child[c])
                    delete child[c];
            }
        }
    };

    Node* root;
    Aho()
    {
        root = new Node();
    }
    ~Aho()
    {
        if (root)
            delete root;
    }
    Aho(vector<string> &patterns)
    {
        root = new Node();
        for (int i = 0; i < patterns.size(); i++)
        {
            insert(patterns[i], i);
        }
        build();
    }

    void insert(string &s, int idx)
    {
        Node* cur = root;
        for (int i = 0; i < s.size(); i++)
        {
            int c = s[i] - 'a';
            if (!cur->child[c])
            {
                cur->child[c] = new Node();
                cur->cahrs.push_back(c);
            }
            cur = cur->child[c];
        }
        // FIX 1: We just store the single pattern index ending at this node.
        cur->patIdx = idx;
    }
    void build()
    {
        queue<Node*> q;
        for (int i = 0; i < 26; i++)
        {
            if (root->child[i])
            {
                root->child[i]->fail = root;
                q.push(root->child[i]);
            }
            else
                root->child[i] = root;
        }
        while (q.size())
        {
            Node* cur = q.front();
            q.pop();
            for (int i = 0; i < cur->cahrs.size(); i++)
            {
                char c = cur->cahrs[i];
                Node* next = cur->child[c];
                Node* fail = cur->fail;

                while (fail != root && !fail->child[c])
                {
                    fail = fail->fail;
                }
                if (fail->child[c])
                {
                    next->fail = fail->child[c];
                }
                else
                {
                    next->fail = root;
                }

                // FIX 2: Do NOT copy `patIdx` vectors from the fail link here.
                // Copying vectors causes an explosion in memory. We will traverse fail links in search() instead.
                q.push(next);
            }
        }
    }

    // FIX 3: Return intervals directly. Creating a vector<vector<int>> res(N) uses lots of dynamic memory allocations.
    vector<pair<long long, long long>> search(string &s, const vector<string> &pattens)
    {
        vector<pair<long long, long long>> intervals;
        Node* cur = root;
        for (int i = 0; i < s.size(); i++)
        {
            int c = s[i] - 'a';
            while (cur != root && !cur->child[c])
            {
                cur = cur->fail;
            }
            if (cur->child[c])
            {
                cur = cur->child[c];
            }

            // FIX 3: Traverse fail links to collect all matching patterns ending at position 'i'.
            // Because max pattern length is 20, this traverses at most 20 nodes, staying well within time limits.
            Node* temp = cur;
            while (temp != root)
            {
                if (temp->patIdx != -1)
                {
                    intervals.push_back({i - pattens[temp->patIdx].size() + 1, i});
                }
                temp = temp->fail;
            }
        }
        return intervals;
    }
};
