/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/E
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

void solve() {
    uint32_t n = 0, m = 0;
    std::cin >> n >> m;
    used.assign(n, WHITE);
    gr.resize(n);
    rev_gr.resize(n);
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t u = 0, v = 0;
        std::cin >> u >> v;
        gr[u - 1].push_back(v - 1);
        rev_gr[v - 1].push_back(u - 1);
    }

    for (uint32_t v = 0; v < n; ++v) {
        if (used[v] == WHITE) {
            getTopSort(v);
        }
    }
    reverse(top_sort.begin(), top_sort.end());

    used.assign(n, WHITE);
    get_kss.resize(n);
    for (uint32_t i = 0; i < n; ++i) {
        uint32_t v = top_sort[i];
        if (used[v] == WHITE) {
            kss.push_back({});
            dfs(v);
            ++cnt_kss;
        }
    }
    /* for (auto &vec: kss) {
      for (auto &v: vec) {
        std::cout << v + 1 << ' ';
      }
      std::cout << '\n';
    } */
    std::vector<std::vector<bool>> kss_incidence(
            cnt_kss, std::vector<bool>(cnt_kss, false));
    uint32_t cnt = 0;
    for (uint32_t out_kss_id = 0; out_kss_id < cnt_kss; ++out_kss_id) {
        for (uint32_t v: kss[out_kss_id]) {
            for (uint32_t u: gr[v]) {
                uint32_t in_kss_id = get_kss[u];
                if (out_kss_id != in_kss_id && !kss_incidence[out_kss_id][in_kss_id]) {
                    kss_incidence[out_kss_id][in_kss_id] = true;
                    ++cnt;
                }
            }
        }
    }
    std::cout << cnt << '\n';
}

int main() {
    solve();

    return 0;
}