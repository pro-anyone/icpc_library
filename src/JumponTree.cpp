#include <vector>
#include <algorithm>
#include <queue>

struct LowestCommonAncestor {
    std::vector<std::vector<int>> table;
    std::vector<int> depth;

    LowestCommonAncestor(const std::vector<std::vector<int>>& e, int root = 0) {
        int n = e.size();
        int lg = 0;
        while ((1 << lg) < n) lg++;
        table.assign(lg + 1, std::vector<int>(n));
        depth.assign(n, -1);
        std::queue<int> que;
        que.emplace(root);
        depth[root] = 0;
        table[0][root] = -1;
        while (que.size()) {
            int v = que.front();
            que.pop();
            for (int c : e[v]) {
                if (depth[c] == -1) {
                    table[0][c] = v;
                    depth[c] = depth[v] + 1;
                    que.emplace(c);
                }
            }
        }
        for (int j = 0; j < lg; j++) {
            for (int i = 0; i < n; i++) {
                if (table[j][i] == -1)
                    table[j + 1][i] = -1;
                else
                    table[j + 1][i] = table[j][table[j][i]];
            }
        }
    }

    int lca(int u, int v) {
        if (depth[u] > depth[v]) std::swap(u, v);
        for (int i = 0; i < (int)table.size(); i++) {
            if (depth[v] == depth[u]) break;
            if ((depth[v] - depth[u]) >> i & 1) {
                v = table[i][v];
            }
        }
        if (u == v) return u;
        for (int i = table.size() - 1; i >= 0; i--) {
            if (table[i][u] != table[i][v]) {
                u = table[i][u];
                v = table[i][v];
            }
        }
        return table[0][u];
    }

    int lca2(int a, int b, int c) {
        return lca(a, b) ^ lca(b, c) ^ lca(c, a);
    }

    int la(int u, int k) {
        if (depth[u] < k) return -1;
        for (int i = 0; i < (int)table.size(); i++) {
            if (k >> i & 1) {
                k -= (1 << i);
                u = table[i][u];
            }
            if (k == 0) break;
        }
        return u;
    }

    int distance(int u, int v) {
        return depth[u] + depth[v] - depth[lca(u, v)] * 2;
    }

    int jumpto(int u, int v, int k) {
        int l = lca(u, v);
        if (depth[u] - depth[l] >= k)
            return la(u, k);
        else {
            k -= (depth[u] - depth[l]);
            int d = depth[v] - depth[l];
            if (d < k)
                return -1;
            else
                return la(v, d - k);
        }
    }
};