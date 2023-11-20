/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/K
 */

#include <iostream>
#include <vector>

const int64_t INF = 1e18;

struct Edge {
    uint32_t from, to;
    int64_t cap, flow;
    int64_t cost;
};

std::vector<std::vector<uint32_t>> gr;
std::vector<Edge> edges;
std::vector<int64_t> dist, pot;
std::vector<bool> used;

std::vector<uint32_t> par;

uint32_t n, m, size, s, t;

bool Dijkstra() {
    used.assign(size, false);
    dist.assign(size, INF);
    dist[s] = 0;
    while (true) {
        uint64_t v = size;
        for (uint64_t u = 0; u < size; ++u) {
            if (!used[u] && (v == size || dist[u] < dist[v])) {
                v = u;
            }
        }
        if (v == size || dist[v] == INF) {
            break;
        }
        used[v] = true;
        for (uint32_t edge_id: gr[v]) {
            auto& edge = edges[edge_id];
            int64_t val = edge.cost + pot[v] - pot[edge.to];
            if (edge.cap - edge.flow > 0 && dist[edge.to] > dist[v] + val) {
                dist[edge.to] = dist[v] + val;
                par[edge.to] = edge_id;
            }
        }
    }
    return dist[t] < INF;
}

void FB() {
    std::vector<std::vector<int64_t>> dp(size, std::vector<int64_t>(size + 1, INF));
    dp[s][0] = 0;
    for (uint64_t k = 1; k <= size; ++k) {
        for (uint64_t v = 0; v < size; ++v) {
            dp[v][k] = dp[v][k - 1];
            for (uint32_t edge_id: gr[v]) {
                auto& edge = edges[edge_id ^ 1];
                if (edge.cap - edge.flow > 0 && dp[edge.from][k - 1] != INF) {
                    dp[v][k] = std::min(dp[v][k], dp[edge.from][k - 1] + edge.cost);
                }
            }
        }
    }
    dist.assign(size, INF);
    for (uint64_t v = 0; v < size; ++v) {
        dist[v] = dp[v][size];
    }
    pot = dist;
}

void build() {
    gr.resize(size);
    for (int i = 0; i < m; ++i) {
        uint32_t from = 0, to = 0;
        int64_t cap = 0, cost = 0;
        std::cin >> from >> to >> cap >> cost;
        --from;
        --to;
        edges.push_back({from, to, cap, 0, cost});
        gr[from].push_back(edges.size() - 1);
        edges.push_back({to, from, 0, 0, -cost});
        gr[to].push_back(edges.size() - 1);
    }
}


void minCostNFlow() {
    FB();
    int64_t res = 0;
    par.assign(size, 2 * m);
    while (Dijkstra()) {
        for (uint64_t v = 0; v < size; ++v) {
            pot[v] = std::min(INF, pot[v] + dist[v]);
        }
        int64_t delta = INF;
        for (uint32_t v = t; v != s;) {
            auto& edge = edges[par[v]];
            delta = std::min(delta, edge.cap - edge.flow);
            v = edge.from;
        }
        int64_t cnt = 0;
        for (uint32_t v = t; v != s;) {
            auto& edge = edges[par[v]];
            edge.flow += delta;
            auto& rev_edge = edges[par[v] ^ 1];
            rev_edge.flow -= delta;
            res += edge.cost * delta;
            v = edge.from;
        }
    }
    std::cout << res << '\n';
}

void solve() {
    std::cin >> n >> m;
    s = 0, t = n - 1;
    size = n;
    build();
    minCostNFlow();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    solve();

    return 0;
}