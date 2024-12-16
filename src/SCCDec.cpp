#include <vector>

struct SCC {
    int n;
    vector<vector<int>> g, rev, scc_graph;
    vector<int> time, visited;
    SCC(int _n): n(_n), g(_n), rev(_n), visited(n) {}
    void add_edge(int u, int v) {
        g[u].emplace_back(v);
        rev[v].emplace_back(u);
    }

    void dfs1(int v) {
        visited[v] = 1;
        for (int u: g[v]) {
            if (!visited[u]) dfs1(u);
        }
        time.push_back(v);
    }

    void dfs2(int v, vector<int>& scc) {
        visited[v] = 1;
        scc.push_back(v);
        for (int u: rev[v]) {
            if (!visited[u]) dfs2(u, scc);
        }
    }

    vector<vector<int>> scc_decomposition() {
        visited.assign(n, 0);
        for (int i = 0; i < n; i++) {
            if (!visited[i]) dfs1(i);
        }
        visited.assign(n, 0);
        while (!time.empty()) {
            vector<int> tmp;
            int v = time.back();
            time.pop_back();
            if (!visited[v]) {
                dfs2(v, tmp);
                scc_graph.emplace_back(tmp);
            }
        }
        return scc_graph;
    }

    vector<int> scc_renumber(const vector<vector<int>>& t) {
        vector<int> tmp(n);
        for (int i = 0; i < int(t.size()); i++) {
            auto& v = t[i];
            for (int j = 0; j < int(v.size()); j++) {
                tmp[v[j]] = i;
            }
        }
        return tmp;
    }

    vector<vector<int>> make_new_graph() {
        auto scc_g = scc_decomposition();
        vector<vector<int>> new_graph(scc_g.size());
        vector<int> tmp = scc_renumber(scc_g);
        for (int i = 0; i < int(scc_g.size()); i++) {
            auto& v = scc_g[i];
            for (auto u: v) {
                tmp[u] = 1;
                for (auto c: g[u]) {
                    if (tmp[c]) continue;
                    tmp[c] = 1;
                    new_graph[i].push_back(c);
                }
            }
            for (auto u: v) {
                tmp[u] = 0;
                for (auto c: g[u]) tmp[c] = 0;
            }
        }
        return new_graph;
    }
};