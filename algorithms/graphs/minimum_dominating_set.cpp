/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/E
 */

#include <iostream>
#include <vector>

std::vector<std::vector<bool>> gr;
std::vector<bool> used;

void dfs(uint32_t v) {
    used[v] = true;
    for (uint32_t u = 0; u < gr[v].size(); ++u) {
        if (gr[v][u] && !used[u]) {
            dfs(u);
        }
    }
}

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    gr.assign(n + m, std::vector<bool>(n + m, false));
    for (uint32_t v = 0; v < n; ++v) {
        uint32_t k = 0;
        std::cin >> k;
        for (uint32_t j = 0; j < k; ++j) {
            uint32_t u = 0;
            std::cin >> u;
            u += n - 1;
            gr[u][v] = gr[v][u] = true;
        }
    }
    std::vector<bool> is_saturated(n, false);
    for (uint32_t v = 0; v < n; ++v) {
        uint32_t u = 0;
        std::cin >> u;
        if (u != 0) {
            u += n - 1;
            gr[v][u] = false;
            is_saturated[v] = true;
        }
    }
    for (uint32_t v = 0; v < n + m; ++v) {
        for (uint32_t u = 0; u < n + m; ++u) {
            if (u != v) {
                if (gr[u][v] && gr[v][u]) {
                    gr[u][v] = false;
                }
            }
        }
    }
    used.assign(n + m, false);
    for (uint32_t v = 0; v < n; ++v) {
        if (!is_saturated[v]) {
            dfs(v);
        }
    }
    std::vector<uint32_t> l, r;
    for (uint32_t v = 0; v < n + m; ++v) {
        if (v < n && !used[v]) {
            l.push_back(v);
        } else if (v >= n && used[v]) {
            r.push_back(v);
        }
    }
    std::cout << l.size() + r.size() << '\n';
    std::cout << l.size() << ' ';
    for (auto v: l) {
        std::cout << v + 1 << ' ';
    }
    std::cout << '\n' << r.size() << ' ';
    for (auto v: r) {
        std::cout << v - (n - 1) << ' ';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    solve();

    return 0;
}