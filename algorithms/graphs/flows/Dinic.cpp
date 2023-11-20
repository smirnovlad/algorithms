/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/H
 */

#include <iostream>
#include <vector>
#include <queue>

const uint32_t INF = 1e9;

struct Edge {
    uint32_t from, to;
    long double cap, flow;
};

uint32_t n, m, size, s, t;
std::vector<Edge> edges;
std::vector<uint32_t> d;
std::vector<std::vector<uint32_t>> gr;
std::vector<uint32_t> ptr, level;

void dfs(uint32_t v, std::vector<bool>& used) {
    used[v] = true;
    for (uint32_t edge_id: gr[v]) {
        auto& edge = edges[edge_id];
        if (!used[edge.to] && edge.cap - edge.flow > 0) {
            dfs(edge.to, used);
        }
    }
}

long double dinitsDFS(uint32_t v, long double f) {
    if (v == t) {
        return f;
    }
    while (ptr[v] < gr[v].size()) {
        uint32_t edge_id = gr[v][ptr[v]];
        auto& edge = edges[edge_id];
        if (level[v] + 1 != level[edge.to] || edge.cap - edge.flow == 0) {
            ++ptr[v];
            continue;
        }
        long double x = dinitsDFS(edge.to, std::min(f, edge.cap - edge.flow));
        if (x == 0) {
            ++ptr[v];
            continue;
        }
        edge.flow += x;
        auto& rev_edge = edges[edge_id ^ 1];
        rev_edge.flow -= x;
        return x;
    }
    return 0;
}
bool getLayers() {
    level.assign(size, INF);
    level[s] = 0;
    std::queue<uint32_t> q;
    q.push(s);
    while (!q.empty()) {
        uint32_t v = q.front();
        q.pop();
        for (uint32_t edge_id: gr[v]) {
            auto& edge = edges[edge_id];
            if (edge.cap - edge.flow > 0 && level[edge.to] == INF) {
                level[edge.to] = level[v] + 1;
                q.push(edge.to);
            }
        }
    }
    return level[t] != INF;
}

long double Dinic() {
    long double flow = 0;
    while (getLayers()) {
        ptr.assign(size, 0);
        while (true) {
            long double to_push = dinitsDFS(s, INF);
            flow += to_push;
            if (to_push == 0) {
                break;
            }
        }
    }
    return flow;
}

void minCut(std::vector<bool>& used) {
    Dinic();
    dfs(s, used);
}

void construct(long double g) {
    for (auto& edge: edges) {
        if (edge.from == s) {
            edge.cap = m;
        }
        if (edge.to == t) {
            edge.cap = m + 2 * g - d[edge.from];
        }
        edge.flow = 0;
    }
}

std::vector<bool> goldberg() {
    long double l = 0, u = m, eps = 1 / static_cast<long double>(n * (n - 1));
    std::vector<bool> V1;
    while (u - l >= eps) {
        long double g = (u + l) / 2;
        construct(g);
        std::vector<bool> used(size, false);
        minCut(used);
        uint32_t S_cardinality = 0;
        for (auto b: used) {
            if (b) {
                ++S_cardinality;
            }
        }
        if (S_cardinality == 1) {
            u = g;
        } else {
            l = g;
            V1 = used;
        }
    }
    return V1;
}

void addEdge(uint32_t from, uint32_t to, long double cap, long double flow) {
    edges.push_back({from, to, cap, flow});
    gr[from].push_back(edges.size() - 1);

}

bool get() {
    std::cin >> n >> m;
    if (m == 0) {
        std::cout << 1 << '\n' << 1 << '\n';
    } else {
        size = n + 2;
        s = 0, t = size - 1;
        gr.resize(size);
        d.assign(size, 0);
        for (uint32_t i = 0; i < m; ++i) {
            uint32_t a = 0, b = 0;
            std::cin >> a >> b;
            addEdge(a, b, 1, 0);
            addEdge(b, a, 0, 0);
            addEdge(b, a, 1, 0);
            addEdge(a, b, 0, 0);
            ++d[a];
            ++d[b];
        }
        for (uint32_t v = 1; v <= n; ++v) {
            addEdge(s, v, 1, 0);
            addEdge(v, s, 0, 0);
            addEdge(v, t, 1, 0);
            addEdge(t, v, 0, 0);
        }
    }
    return (m != 0);
}

void solve() {
    if (get()) {
        std::vector<bool> ans = goldberg();
        uint32_t cnt = 0;
        for (uint32_t v = 1; v < size - 1; ++v) {
            if (ans[v]) {
                ++cnt;
            }
        }
        std::cout << cnt << '\n';
        for (uint32_t v = 1; v < size - 1; ++v) {
            if (ans[v]) {
                std::cout << v << '\n';
            }
        }
    }
}

int main() {
    solve();

    return 0;
}