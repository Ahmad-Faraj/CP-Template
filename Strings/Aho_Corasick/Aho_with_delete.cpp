int nextNodeId = 0;

#define nd Node *
struct Aho
{
    struct Node
    {
        nd fail;
        nd child[26];
        vector<int> patIdx;
        vector<char> cahrs;
        int id;
        Node()
        {
            memset(child, 0, sizeof child);
            id = nextNodeId++;
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

    nd root;
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
        nd cur = root;
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
        cur->patIdx.push_back(idx);
    }
    void build()
    {
        queue<nd> q;
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
            nd cur = q.front();
            q.pop();
            for (int i = 0; i < cur->cahrs.size(); i++)
            {
                char c = cur->cahrs[i];
                nd next = cur->child[c];
                nd fail = cur->fail;

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
                for (int j = 0; j < next->fail->patIdx.size(); j++)
                {
                    next->patIdx.push_back(next->fail->patIdx[j]);
                }
                q.push(next);
            }
        }
    }
    vector<vector<int>> search(string &s)
    {
        vector<vector<int>> res(s.size());
        nd cur = root;
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
            for (int j = 0; j < cur->patIdx.size(); j++)
            {
                res[i].push_back(cur->patIdx[j]);
            }
        }
        return res;
    }
};
