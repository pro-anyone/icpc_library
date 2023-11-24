#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

template<class Cap, Cap INF>
struct Dinic {
    struct edge {
        int to;
        Cap cap;
        int rev_id;
        bool exist;
        edge(int v, Cap c, int id, bool h): to(v), cap(c), rev_id(id), exist(h) {}
        edge(): edge(0, 0, -1, 0) {}
    };
    int n;
    vector<vector<edge>> g;
    vector<int> iter, level;
    Dinic(int _n): n(_n) {
        g = vector<vector<edge>>(n);
        iter = vector<int>(n);
        level = vector<int>(n);
    }

    void add_edge(int u, int v, Cap c) {
        g[u].emplace_back(v, c, g[v].size(), true);
        g[v].emplace_back(u, 0, g[u].size() - 1, false);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        que.emplace(s);
        level[s] = 0;
        while (!que.empty()) {
            auto v = que.front();
            que.pop();
            for (auto& e: g[v]) {
                if (e.cap <= 0 || level[e.to] != -1) continue;
                level[e.to] = level[v] + 1;
                if (e.to == t) return true;
                que.emplace(e.to);
            }
        }
        return level[t] != -1;
    }

    Cap dfs(int v, Cap limit, const int& target) {
        if (v == target) return limit;
        Cap res = 0;
        for (; iter[v] < int(g[v].size()); iter[v]++) {
            auto& e = g[v][iter[v]];
            if (e.cap <= 0 || level[e.to] <= level[v]) continue;
            Cap f = dfs(e.to, min(limit - res, e.cap), target);
            if (f <= 0) continue;
            e.cap -= f;
            g[e.to][e.rev_id].cap += f;
            res += f;
            if (res == limit) return res;
        }
        level[v] = -1;
        return res;
    }

    Cap maxflow(int s, int t) {
        Cap res = 0;
        while (bfs(s, t)) {
            fill(iter.begin(), iter.end(), 0);
            Cap f = dfs(s, INF, t);
            if (f == 0) break;
            res += f;
        }
        return res;
    }

    vector<vector<edge>> edges() const {
        return g;
    }
};