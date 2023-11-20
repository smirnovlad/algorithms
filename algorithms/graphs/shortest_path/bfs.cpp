/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/377095/problem/A
 */

#include <iostream>
#include <vector>
#include <queue>

const uint32_t C = 8, INF = 1e9;

std::vector<uint32_t> dist(C * C, INF);
std::vector<int32_t> par(C * C, -1);
std::queue<uint32_t> q;

void update_node(int32_t x, int32_t y, uint32_t prev) {
    if (x >= 0 && y >= 0 && x <= C - 1 && y <= C - 1) {
        uint32_t node = y * C + x;
        if (dist[node] == INF) {
            dist[node] = dist[prev] + 1;
            par[node] = prev;
            q.push(node);
        }
    }
}

void solve() {
    char y1 =' ', y2 = ' ';
    uint32_t x1 = 0, x2 = 0;
    std::cin >> y1 >> x1;
    std::cin >> y2 >> x2;
    uint32_t start = (y1 - 'a') * C + x1 - 1;
    uint32_t finish = (y2 - 'a') * C + x2 - 1;
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        uint32_t v = q.front();
        q.pop();
        int32_t cur_x = v % C, cur_y = v / C;
        update_node(cur_x - 2, cur_y - 1, v);
        update_node(cur_x - 1, cur_y - 2, v);
        update_node(cur_x + 1, cur_y - 2, v);
        update_node(cur_x + 2, cur_y - 1, v);
        update_node(cur_x + 2, cur_y + 1, v);
        update_node(cur_x + 1, cur_y + 2, v);
        update_node(cur_x - 1, cur_y + 2, v);
        update_node(cur_x - 2, cur_y + 1, v);
    }
    std::vector<uint32_t> ans;
    uint32_t cur = finish;
    ans.push_back(cur);
    while (par[cur] != -1) {
        cur = par[cur];
        ans.push_back(cur);
    }
    for (uint32_t i = 1; i <= ans.size(); ++i) {
        uint32_t node = ans[ans.size() - i];
        std::cout << char('a' + node / C) << node % C + 1 << '\n';
    }
}

int main() {
    solve();

    return 0;
}