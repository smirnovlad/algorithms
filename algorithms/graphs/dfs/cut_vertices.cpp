/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/I
 */

#include <iostream>
#include <vector>
#include <set>

std::vector<std::vector<uint32_t>> gr;
std::set<uint32_t> ans;
uint32_t timer = 0;
std::vector<uint32_t> tin, ret;
std::vector<bool> used;

void dfs(uint32_t v, int32_t p = -1) {
    used[v] = true;
    tin[v] = timer++;
    ret[v] = tin[v];
    uint32_t children = 0;
    for (uint32_t u: gr[v]) {
        if (used[u]) {
            ret[v] = std::min(ret[v], tin[u]);
        } else {
            ++children;
            dfs(u, v);
            ret[v] = std::min(ret[v], ret[u]);
            if (ret[u] == tin[v] && p != -1) {
                ans.insert(v);
            }
        }
    }
    if (p == -1 && children > 1) {
        ans.insert(v);
    }
}

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    gr.resize(n);
    used.assign(n, false);
    tin.assign(n, 1e9);
    ret.assign(n, 1e9);
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t u = 0, v = 0;
        std::cin >> u >> v;
        --u, --v;
        gr[u].push_back(v);
        gr[v].push_back(u);
    }

    for (uint32_t v = 0; v < n; ++v) {
        if (!used[v]) {
            dfs(v);
        }
    }
    std::cout << ans.size() << '\n';
    for (auto v: ans) {
        std::cout << v + 1 << ' ';
    }
}

int main() {
    solve();

    return 0;
}