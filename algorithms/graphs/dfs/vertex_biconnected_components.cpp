/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/K
 */

#include <iostream>
#include <vector>
#include <set>
#include <queue>

struct Edge {
    int32_t to;
    int32_t id;
    bool is_proof = false;
    bool is_tree_edge = false;
};

std::vector<std::vector<Edge>> gr;
std::vector<std::vector<uint32_t>> ans;
uint32_t timer = 0;
std::vector<uint32_t> tin, ret;
std::vector<bool> used;
uint32_t cnt_block = 0;

void dfs2(uint32_t v, std::queue<Edge>& q, Edge e = {-1, -1}) {
    used[v] = true;
    for (auto edge: gr[v]) {
        if (edge.is_proof) {
            q.push(edge);
        } else if (edge.is_tree_edge || (used[edge.to] && edge.id != e.id && tin[edge.to] < tin[v])) {
            if (edge.id == 4) {
            }
            ans[cnt_block].push_back(edge.id);
            if (!used[edge.to]) {
                dfs2(edge.to, q, edge);
            }
        }
    }
}

void dfs1(uint32_t v, Edge e = {-1, -1}, int32_t p = -1) {
    used[v] = true;
    tin[v] = timer++;
    ret[v] = tin[v];
    for (auto &edge: gr[v]) {
        uint32_t u = edge.to;
        if (used[u]) {
            ret[v] = std::min(ret[v], tin[u]);
        } else {
            dfs1(u, edge, v);
            ret[v] = std::min(ret[v], ret[u]);
            edge.is_tree_edge = true;
            if (ret[u] == tin[v] && p != -1) {
                edge.is_proof = true;
            }
        }
    }
}

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    gr.resize(n);
    used.assign(n, false);
    tin.assign(n, 1e9);
    ret.assign(n, 1e9);
    for (int32_t i = 0; i < m; ++i) {
        int32_t u = 0, v = 0;
        std::cin >> u >> v;
        gr[u].push_back({v, i});
        gr[v].push_back({u, i});
    }

    for (uint32_t v = 0; v < n; ++v) {
        if (!used[v]) {
            dfs1(v);
        }
    }

    used.assign(n, false);
    for (int32_t v = 0; v < n; ++v) {
        if (!used[v]) {
            std::queue<Edge> q;
            for (auto& el: gr[v]) {
                if (el.is_tree_edge) {
                    q.push(el);
                }
            }
            while (!q.empty()) {
                ans.push_back({});
                auto edge = q.front();
                q.pop();
                used[v] = true;
                ans[cnt_block].push_back(edge.id);
                dfs2(edge.to, q, edge);
                ++cnt_block;
            }
        }
    }
    std::cout << cnt_block << '\n';
    for (auto &vec: ans) {
        std::cout << vec.size() << ' ';
        for (auto id: vec) {
            std::cout << id << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    solve();

    return 0;
}