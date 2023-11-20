/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/J
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <stack>

std::vector<std::set<uint32_t>> gr;
std::vector<uint32_t> ans;

void getEulerPath(uint32_t v) {
    std::stack<uint32_t> s;
    s.push(v);
    while (!s.empty()) {
        uint32_t u = s.top();
        bool foundEdge = false;
        for (uint32_t w: gr[u]) {
            s.push(w);
            gr[u].erase(w);
            foundEdge = true;
            break;
        }
        if (!foundEdge) {
            s.pop();
            ans.push_back(u);
        }
    }
}

void solve() {
    uint32_t n = 0, A = 0;
    std::cin >> n >> A;
    gr.resize(n);
    for (uint32_t i = 0; i < n; ++i) {
        for (uint32_t j = 0; j < n; ++j) {
            bool flag = false;
            std::cin >> flag;
            if (!flag && i != j) {
                gr[i].insert(j);
            }
        }
    }
    getEulerPath(A - 1);
    reverse(ans.begin(), ans.end());
    for (uint32_t i = 0; i < ans.size() - 1; ++i) {
        std::cout << ans[i] + 1 << ' ' << ans[i + 1] + 1 << '\n';
    }
}

int main() {
    solve();

    return 0;
}