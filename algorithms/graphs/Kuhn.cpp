/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/D
 */

#include <iostream>
#include <vector>

uint32_t n = 0, m = 0;

std::vector<std::vector<uint32_t>> gr;
std::vector<uint32_t> match;
std::vector<bool> used;

bool augment(uint32_t v) {
    if (used[v]) {
        return false;
    }
    used[v] = true;
    for (uint32_t to: gr[v]) {
        if (match[to] == 0 || augment(match[to])) {
            match[to] = v;
            return true;
        }
    }
    return false;
}

void solve() {
    int32_t a = 0, b = 0;
    std::cin >> n >> m >> a >> b;
    uint32_t size = n * m;

    uint32_t cnt = 0;
    std::vector<std::vector<char>> table(n, std::vector<char>(m, '#'));
    gr.resize(n * m + 1); // this is a bipartite graph
    for (uint32_t i = 0; i < n; ++i) {
        for (uint32_t j = 0; j < m; ++j) {
            std::cin >> table[i][j];
            if (table[i][j] == '*') {
                ++cnt;
                uint32_t v = i * m + j + 1;
                uint32_t left_from_v = 0, top_from_v = 0;
                if (i > 0 && table[i - 1][j] == '*') {
                    top_from_v = ((i - 1) * m + j) + 1;
                }
                if (j > 0 && table[i][j - 1] == '*') {
                    left_from_v = (i * m + (j - 1)) + 1;
                }
                if (top_from_v != 0) {
                    gr[v].push_back(top_from_v);
                    gr[top_from_v].push_back(v);
                }
                if (left_from_v != 0) {
                    gr[v].push_back(left_from_v);
                    gr[left_from_v].push_back(v);
                }
            }
        }
    }
    if (2 * b <= a) {
        std::cout << b * int32_t(cnt) << '\n';
    } else {
        used.assign(size + 1, false);
        match.assign(size + 1, 0);

        for (uint32_t i = 0; i < n; ++i) {
            for (uint32_t j = i % 2; j < m; j += 2) {
                uint32_t v = i * m + j + 1;
                if (augment(v)) {
                    used.assign(size + 1, false);
                }
            }
        }
        uint32_t couples = 0;
        for (uint32_t i = 0; i < n; ++i) {
            for (uint32_t j = (i + 1) % 2; j < m; j += 2) {
                uint32_t v = i * m + j + 1;
                if (match[v] != 0) {
                    ++couples;
                }
            }
        }
        std::cout << int32_t(cnt - couples * 2) * b + int32_t(couples) * a << '\n';
    }

}

int main() {
    solve();

    return 0;
}