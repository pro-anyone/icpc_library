#include <vector>
#include <utility>

struct HLD{
    int n;
    vector<vector<int>> g;
    vector<int> sz;
    vector<int> head;
    vector<int> in;
    vector<int> rev;
    vector<int> out;
    vector<int> par;
    HLD(vector<vector<int>> edge) : n(edge.size()), g(edge) {
        sz = vector<int>(n);
        head = vector<int>(n);
        in = vector<int>(n);
        rev = vector<int>(n);
        par = vector<int>(n);
        out = vector<int>(n);
    }
    void dfs_sz(int v, int p){
        if (g[v].size() &&  g[v][0] == p) swap(g[v][0], g[v].back());
        sz[v] = 1;
        for (auto &u: g[v]){
            if (u == p) continue;
            dfs_sz(u, v);
            sz[v] += sz[u];
            if (sz[u] > sz[g[v][0]]) swap(u, g[v][0]);
        }
    }
    void dfs_hld(int v, int p, int& t){
        in[v] = t++;
        rev[in[v]] = v;
        par[v] = p;
        for (auto u: g[v]){
            if (u == p) continue;
            head[u] = (u == g[v][0]? head[v]: u);
            dfs_hld(u, v, t);
        }
        out[v] = t;
    }

    void build(){
        dfs_sz(0, -1);
        int t = 0;
        dfs_hld(0, -1, t);
    }

    int lca(int u, int v){
        while (true){
            if (in[u] > in[v]) swap(u, v);
            if (head[u] == head[v]) return u;
            v = par[head[v]];
        }
    }

    vector<int> renum() {
        return in;
    }

    pair<vector<pair<int, int>>, vector<pair<int, int>>> query(int u, int v, bool f = false){
        // 返り値は閉区間, 辺属性未verify, 非可換対応
        vector<pair<int, int>> l, r;
        while (true){
            if (head[u] == head[v]) break;
            if (in[u] > in[v]){
                l.emplace_back(in[head[u]], in[u]);
                u = par[head[u]];
            }
            else{
                r.emplace_back(in[head[v]], in[v]);
                v = par[head[v]];
            }
        }
        if (in[u] > in[v]){
            l.emplace_back(in[v] + f, in[u]);
        }
        else{
            r.emplace_back(in[u] + f, in[v]);
        }
        reverse(all(r));
        return {l, r};
    }

    pair<int, int> subtree(int u){
        return {in[u], out[u]};
    }
};