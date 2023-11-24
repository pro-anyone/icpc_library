template<int f(char c), int char_size = 26>
struct Trie {
    struct node {
        node* ch[char_size];
        bool fin;
        node(): fin(false) {
            for (int i = 0; i < char_size; i++) {
                ch[i] = nullptr;
            }
        }
    };

    node* root;

    Trie(): root(new node()) {}

    node* _insert(node* x, const string &s, int t) {
        if (t == int(s.size())) {
            x->fin = true;
            return x;
        }
        node* nx = x->ch[f(s[t])];
        if (!nx) {
            nx = new node();
            _insert(nx, s, t + 1);
            return x->ch[f(s[t])] = nx;
        }
        else return _insert(nx, s, t + 1);
    }

    void insert(const string &s) {
        _insert(root, s, 0);
    }

    bool _find(node* x, const string &s, int t) {
        if (t == int(s.size())) return x->fin;
        node* nx = x->ch[f(s[t])];
        if (!nx) return false;
        return _find(nx, s, t + 1);
    }

    bool find(const string &s) {
        return _find(root, s, 0);
    }

    bool _prefix(node* x, const string &s, int t) {
        if (t == int(s.size())) return true;
        node* nx = x->ch[f(s[t])];
        if (!nx) return false;
        return _prefix(nx, s, t + 1);
    }

    bool prefix(const string &s) {
        return _prefix(root, s, 0);
    }
};