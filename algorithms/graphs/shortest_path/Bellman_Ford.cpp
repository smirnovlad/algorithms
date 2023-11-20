/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/377095/problem/D
 */

#include <iostream>
#include <vector>

const int64_t INF = 7 * 1e18;

struct Edge {
    uint32_t from;
    int64_t weight;
};

std::vector<bool> used;
std::vector<std::vector<Edge>> gr;
std::vector<std::vector<uint32_t>> gr_out;
std::vector<std::vector<int64_t>> dp;
std::vector<int64_t> dist;

void dfs(uint32_t v) {
    used[v] = true;
    dist[v] = -INF;
    for (uint32_t u: gr_out[v]) {
        if (!used[u]) {
            dfs(u);
        }
    }
}

void solve() {
    uint32_t n = 0, m = 0, s = 0;
    std::cin >> n >> m >> s;
    --s;
    gr.resize(n);
    gr_out.resize(n);
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t from = 0, to = 0;
        int64_t weight = 0;
        std::cin >> from >> to >> weight;
        --from;
        --to;
        gr[to].push_back({from, weight});
        gr_out[from].push_back(to);
    }
    dp.assign(n, std::vector<int64_t>(n + 1, INF));
    dp[s][0] = 0;
    for (uint32_t k = 1; k <= n; ++k) {
        for (uint32_t v = 0; v < n; ++v) {
            dp[v][k] = dp[v][k - 1];
            for (auto edge: gr[v]) {
                uint32_t u = edge.from;
                int64_t weight = edge.weight;
                if (dp[u][k - 1] != INF) {
                    dp[v][k] = std::min(dp[v][k], dp[u][k - 1] + weight);
                }
            }
        }
    }
    dist.assign(n, INF);
    dist[s] = 0;
    used.assign(n, false);
    for (uint32_t v = 0; v < n; ++v) {
        if (!used[v]) {
            dist[v] = dp[v][n];
            if (dp[v][n] < dp[v][n - 1]) {
                dist[v] = -INF;
                dfs(v);
            }
        }
    }

    for (uint32_t v = 0; v < n; ++v) {
        if (dist[v] == INF) {
            std::cout << "*\n";
        } else if (dist[v] == -INF) {
            std::cout << "-\n";
        } else {
            std::cout << dist[v] << '\n';
        }
    }
}

int main() {
    solve();

    return 0;
}