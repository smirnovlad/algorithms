/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/377095/problem/C
 */

#include <iostream>
#include <vector>

void solve() {
    uint32_t n = 0;
    std::cin >> n;
    std::vector<std::vector<int32_t>> gr(n, std::vector<int32_t>(n, 1e9));
    for (auto& v: gr) {
        for (auto& el: v) {
            std::cin >> el;
        }
    }

    auto dist = gr;
    for (uint32_t w = 0; w < n; ++w) {
        for (uint32_t u = 0; u < n; ++u) {
            for (uint32_t v = 0; v < n; ++v) {
                dist[u][v] = std::min(dist[u][v], dist[u][w] + dist[w][v]);
            }
        }
    }

    for (auto& vec: dist) {
        for (auto& el: vec) {
            std::cout << el << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    solve();

    return 0;
}