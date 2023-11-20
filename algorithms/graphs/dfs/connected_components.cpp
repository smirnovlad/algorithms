/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/G
 */

#include <iostream>
#include <vector>

std::vector<bool> used;
std::vector<std::vector<uint32_t>> gr;
std::vector<uint32_t> get_ks;

uint32_t cnt = 0;

void dfs(uint32_t v) {
    used[v] = true;
    get_ks[v] = cnt;
    for (uint32_t u: gr[v]) {
        if (!used[u]) {
            dfs(u);
        }
    }
}

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    used.assign(n, false);
    gr.resize(n);
    get_ks.resize(n);

    for (uint32_t i = 0; i < m; ++i) {
        uint32_t u = 0, v = 0;
        std::cin >> u >> v;
        --u, --v;
        gr[u].push_back(v);
        gr[v].push_back(u);
    }

    for (uint32_t v = 0; v < n; ++v) {
        if (!used[v]) {
            ++cnt;
            dfs(v);
        }
    }

    std::cout << cnt << '\n';
    for (uint32_t v = 0; v < n; ++v) {
        std::cout << get_ks[v] << ' ';
    }
}

int main() {
    solve();

    return 0;
}