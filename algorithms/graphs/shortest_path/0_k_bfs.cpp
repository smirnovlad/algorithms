/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/377095/problem/B
 */

#include <iostream>
#include <vector>
#include <queue>

const uint32_t k = 10, INF = 1e9;

struct Edge {
    uint32_t first_end;
    uint32_t second_end;
    uint32_t time;
};

struct DirectedEdge {
    uint32_t to;
    uint32_t id;
};

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<DirectedEdge>> gr(n);
    std::vector<Edge> edges;
    for (uint32_t id = 0; id < m; ++id) {
        uint32_t u = 0, v = 0, t = 0;
        std::cin >> u >> v >> t;
        --u; --v;
        Edge edge = {u, v, t};
        edges.push_back(edge);
        gr[u].push_back({v, id});
        gr[v].push_back({u, id});
    }
    uint32_t ans = INF;
    std::vector<std::queue<uint32_t>> q(k + 1);
    std::vector<uint32_t> dist(n, INF);
    std::vector<bool> used(n, false);
    for (uint32_t id = 0; id < m; ++id) {
        auto& [a, b, t] = edges[id];
        dist.assign(n, INF);
        used.assign(n, false);
        dist[a] = 0;
        q[0].push(a);
        for (uint32_t x = 0; x < n * k; ++x) {
            if (x >= ans) {
                break;
            }
            while (!q[x % (k + 1)].empty()) {
                uint32_t v = q[x % (k + 1)].front(); // dist[v] <= x because v can be in previous queues
                q[x % (k + 1)].pop();
                if (dist[v] > ans && used[v]) {
                    break;
                }
                if (used[v]) {
                    continue;
                }
                used[v] = true;
                for (auto& [to, i]: gr[v]) {
                    if (i == id) {
                        continue;
                    }
                    if (dist[to] > dist[v] + edges[i].time) {
                        dist[to] = dist[v] + edges[i].time;
                        q[dist[to] % (k + 1)].push(to); // dist[to] % (k + 1) < x % (k + 1) because edges[i].time <= k
                    }
                }
            }
        }
        ans = std::min(ans, t + dist[b]);
    }
    std::cout << ans << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    solve();

    return 0;
}