/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/B
 */

#include <iostream>
#include <vector>
#include <set>

struct Edge {
    uint32_t id;
    uint32_t first_end;
    uint32_t second_end;
    uint32_t weight;

    bool operator<(const Edge& e2) const {
        return weight < e2.weight;
    }
};

std::vector<uint32_t> p, size;

uint32_t get(uint32_t v) {
    if (p[v] == 0) {
        return v;
    } else {
        return p[v] = get(p[v]);
    }
}

void unite(uint32_t v, uint32_t u) {
    uint32_t a = get(v);
    uint32_t b = get(u);
    if (size[a] <= size[b]) {
        std::swap(a, b);
    }
    p[b] = a;
    size[a] += size[b];
}

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    std::multiset<Edge> q;
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t a = 0, b = 0, w = 0;
        std::cin >> a >> b >> w;
        q.insert({i, a, b, w});
    }
    p.assign(n + 1, 0);
    size.assign(n + 1, 1);
    std::vector<uint32_t> MST;
    uint32_t ans = 0;
    while (MST.size() != n - 1) {
        auto edge = *q.begin();
        q.erase(q.begin());
        if (get(edge.first_end) != get(edge.second_end)) {
            unite(edge.first_end, edge.second_end);
            MST.push_back(edge.id);
            ans += edge.weight;
        }
    }
    std::cout << ans << '\n';
}

int main() {
    solve();

    return 0;
}