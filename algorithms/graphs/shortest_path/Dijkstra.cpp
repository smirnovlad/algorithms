/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/377095/problem/F
 */

#include <iostream>
#include <vector>
#include <set>

const int INF = 1e9;

struct Edge {
    uint32_t to;
    uint32_t weight;
};

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<Edge>> gr(n);
    uint32_t s = 0, t = 0;
    std::cin >> s >> t;
    --s; --t;
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t u = 0, v = 0, w = 0;
        std::cin >> u >> v >> w;
        --u; --v;
        gr[u].push_back({v, w});
        gr[v].push_back({u, w});
    }
    std::vector<uint32_t> dist(n, INF);
    dist[s] = 0;
    std::set<std::pair<uint32_t, uint32_t>> q;
    q.insert({0, s});
    while (!q.empty()) {
        uint32_t v = q.begin()->second;
        q.erase(q.begin());
        for (auto edge: gr[v]) {
            uint32_t u = edge.to;
            if (dist[u] > dist[v] + edge.weight) {
                auto it = q.find({dist[u], u});
                if (it != q.end()) {
                    q.erase(it);
                }
                dist[u] = dist[v] + edge.weight;
                q.insert({dist[u], u});
            }
        }
    }
    if (dist[t] != INF) {
        std::cout << dist[t] << '\n';
    } else {
        std::cout << -1 << '\n';
    }
}

int main() {
    solve();

    return 0;
}