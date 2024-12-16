#include <algorithm>
#include <unordered_set>
#include <vector>

struct IncrementalBridgeConnectivity {
    struct UnionFind {
        vector<int> par;
        UnionFind() {}
        UnionFind(int n) : par(n, -1) {}

        int find(int x) {
            if (par[x] < 0) return x;
            return par[x] = find(par[x]);
        }

        void merge(int a, int b) {
            a = find(a);
            b = find(b);
            if (a == b) return;
            if (par[a] > par[b]) swap(a, b);
            par[a] += par[b];
            par[b] = a;
            return;
        }

        bool same(int a, int b) { return find(a) == find(b); }

        int size(int x) { return -par[find(x)]; }

        vector<vector<int>> groups() {
            vector<vector<int>> res(par.size());
            for (int i = 0; i < (int)par.size(); i++) {
                res[find(i)].emplace_back(i);
            }
            res.erase(remove_if(res.begin(), res.end(),
                                [](const vector<int> &v) { return v.empty(); }),
                      res.end());
            return res;
        }
    };

    UnionFind cc, bcc;
    vector<int> par;
    IncrementalBridgeConnectivity() {}
    IncrementalBridgeConnectivity(int n) : cc(n), bcc(n), par(n, -1) {}

    int lca(int a, int b) {
        unordered_set<int> visited;
        while (true) {
            if (a >= 0) {
                if (visited.contains(a)) return a;
                visited.emplace(a);
                a = par[a];
            }
            swap(a, b);
        }
    }

    void reverse_path(int v) {
        if (par[v] < 0) return;
        reverse_path(par[v]);
        par[par[v]] = v;
    }

    void compress_path(int v, int l) {
        while (v != l) {
            bcc.merge(v, par[v]);
            v = par[v];
        }
    }

    void add_edge(int u, int v) {
        if (bcc.same(u, v)) return;
        if (cc.same(u, v)) {
            int l = lca(u, v);
            compress_path(u, l);
            compress_path(v, l);
        } else {
            if (cc.size(u) < cc.size(v)) swap(u, v);
            reverse_path(v);
            par[v] = u;
            cc.merge(u, v);
        }
    }

    bool connected(int u, int v) { return cc.same(u, v); }

    bool bridge_connected(int u, int v) { return bcc.same(u, v); }

    vector<vector<int>> bc_groups() { return bcc.groups(); }
};
