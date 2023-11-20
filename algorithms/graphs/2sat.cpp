/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/L
 */

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<uint32_t> top_sort;
enum color {
    WHITE, GREY, BLACK
};
std::vector<color> used;
std::vector<std::vector<uint32_t>> gr, rev_gr;
std::vector<std::vector<uint32_t>> kss;
std::vector<uint32_t> get_kss;
uint32_t cnt_kss = 0;

void dfs(uint32_t v) {
    used[v] = GREY;
    for (uint32_t u: rev_gr[v]) {
        if (used[u] == WHITE) {
            dfs(u);
        }
    }
    kss[cnt_kss].push_back(v);
    get_kss[v] = cnt_kss;
    used[v] = BLACK;
}

void getTopSort(uint32_t v) {
    used[v] = GREY;
    for (uint32_t u: gr[v]) {
        if (used[u] == WHITE) {
            getTopSort(u);
        }
    }
    top_sort.push_back(v);
    used[v] = BLACK;
}

struct Bulb {
    uint32_t firstToggle = 1e9;
    uint32_t secondToggle = 1e9;
    bool state = false;
};

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<Bulb> bulbs(n);
    for (uint32_t i = 0; i < n; ++i) {
        bool state = false;
        std::cin >> bulbs[i].state;
    }
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t x = 0;
        std::cin >> x;
        for (uint32_t j = 0; j < x; ++j) {
            uint32_t num = 0;
            std::cin >> num;
            if (bulbs[num - 1].firstToggle == 1e9) {
                bulbs[num - 1].firstToggle = i;
            } else {
                bulbs[num - 1].secondToggle = i;
            }
        }
    }

    // r_i = x_i xor y_i = (x or y) and (not(x) or not(y))
    // Let x_i = 0 and y_i = 0 at the beginning.
    // r_i = 0: not(x_i) -> y_i, not(y_i) -> x_i, x_i -> not(y_i), y_i -> not(x_i)
    // r_i = 1: x_i -> y_i, y_i -> x_i, not(x_i) -> not(y_i), not(y_i) -> not(x_i)
    gr.resize(2 * m);
    rev_gr.resize(2 * m);
    used.assign(2 * m, WHITE);
    for (uint32_t i = 0; i < n; ++i) {
        uint32_t u = bulbs[i].firstToggle,
                v = bulbs[i].secondToggle;
        if (bulbs[i].state) {
            gr[u].push_back(v); rev_gr[v].push_back(u);
            gr[v].push_back(u); rev_gr[u].push_back(v);
            gr[u + m].push_back(v + m); rev_gr[v + m].push_back(u + m);
            gr[v + m].push_back(u + m); rev_gr[u + m].push_back(v + m);
        } else {
            gr[u + m].push_back(v); rev_gr[v].push_back(u + m);
            gr[u].push_back(v + m); rev_gr[v + m].push_back(u);
            gr[v + m].push_back(u); rev_gr[u].push_back(v + m);
            gr[v].push_back(u + m); rev_gr[u + m].push_back(v);
        }
    }
    for (uint32_t v = 0; v < 2 * m; ++v) {
        if (used[v] == WHITE) {
            getTopSort(v);
        }
    }
    reverse(top_sort.begin(), top_sort.end());

    used.assign(2 * m, WHITE);
    get_kss.resize(2 * m);
    for (uint32_t i = 0; i < 2 * m; ++i) {
        uint32_t v = top_sort[i];
        if (used[v] == WHITE) {
            kss.push_back({});
            dfs(v);
            ++cnt_kss;
        }
    }

    bool ans = true;
    for (uint32_t i = 0; i < m; ++i) {
        if (get_kss[i] == get_kss[i + m]) {
            ans = false;
            break;
        }
    }

    if (ans) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
}

int main() {
    solve();

    return 0;
}