/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/A
 */

#include <iostream>
#include <vector>
#include <set>

struct Edge {
    uint32_t id;
    uint32_t from;
    uint32_t to;
    uint32_t weight;

    bool operator<(const Edge& e2) const {
        return weight < e2.weight;
    }
};

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<Edge>> gr(n);
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t a = 0, b = 0, w = 0;
        std::cin >> a >> b >> w;
        --a; --b;
        gr[a].push_back({i, a, b, w});
        gr[b].push_back({i, b, a, w});
    }
    std::multiset<Edge> q;
    std::vector<bool> used(n, false);
    std::vector<uint32_t> MST;
    uint32_t ans = 0;
    uint32_t s = 0;
    used[s] = true;
    for (auto edge: gr[s]) {
        q.insert(edge);
    }
    while (MST.size() != n - 1) {
        auto edge = *q.begin();
        q.erase(q.begin());
        if (!used[edge.to]) {
            used[edge.to] = true;
            MST.push_back(edge.id);
            ans += edge.weight;
            for (auto new_edge: gr[edge.to]) {
                if (!used[new_edge.to]) {
                    q.insert(new_edge);
                }
            }
        }
    }
    std::cout << ans << '\n';
}

int main() {
    solve();

    return 0;
}