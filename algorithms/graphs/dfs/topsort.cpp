/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/D
 */

#include <iostream>
#include <vector>

enum color {WHITE, GREY, BLACK};
std::vector<color> used;
std::vector<std::vector<uint32_t>> gr;
std::vector<uint32_t> ans;

bool checkCycle(uint32_t v) {
    used[v] = GREY;
    for (uint32_t u: gr[v]) {
        if (used[u] == GREY) {
            return true;
        } else if (used[u] == WHITE) {
            if (checkCycle(u)) {
                return true;
            }
        }
    }
    used[v] = BLACK;
    ans.push_back(v);
    return false;
}

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    used.assign(n, WHITE);
    gr.resize(n);
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t u = 0, v = 0;
        std::cin >> u >> v;
        gr[u - 1].push_back(v - 1);
    }
    for (uint32_t v = 0; v < n; ++v) {
        if (used[v] == WHITE) {
            if (checkCycle(v)) {
                std::cout << -1 << '\n';
                return;
            }
        }
    }
    for (uint32_t i = 0; i < n; ++i) {
        std::cout << ans[n - 1 - i] + 1 << ' ';
    }
}

int main() {
    solve();

    return 0;
}