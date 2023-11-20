/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/H
 */

#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
    int32_t to;
    int32_t id;
};

std::vector<uint32_t> ans;
std::vector<std::vector<Edge>> gr;
std::vector<bool> used;
std::vector<uint32_t> tin, ret;
uint32_t timer = 0;

void dfs(uint32_t v, Edge e = {-1, -1}, int32_t p = -1) {
    used[v] = true;
    tin[v] = timer++;
    ret[v] = tin[v];
    for (auto edge: gr[v]) {
        uint32_t u = edge.to;
        if (p == u) {
            if (edge.id != e.id) {
                ret[v] = std::min(ret[v], tin[p]);
            }
            continue;
        }
        if (used[u]) {
            ret[v] = std::min(ret[v], tin[u]);
        } else if (!used[u]) {
            dfs(u, edge, v);
            ret[v] = std::min(ret[v], ret[u]);
            if (tin[u] == ret[u]) {
                ans.push_back(edge.id);
            }
        }
    }
}

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    used.assign(n, false);
    gr.resize(n);
    tin.assign(n, 1e9);
    ret.assign(n, 1e9);
    for (int32_t i = 0; i < m; ++i) {
        int32_t u = 0, v = 0;
        std::cin >> u >> v;
        --u, --v;
        gr[u].push_back({v, i});
        gr[v].push_back({u, i});
    }
    for (uint32_t v = 0; v < n; ++v) {
        if (!used[v]) {
            dfs(v);
        }
    }
    sort(ans.begin(), ans.end());
    std::cout << ans.size() << '\n';
    for (auto id: ans) {
        std::cout << id + 1 << ' ';
    }
}

int main() {
    solve();

    return 0;
}