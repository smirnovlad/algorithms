/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/371629/problem/E
 */

#include <iostream>
#include <vector>

void solve() {
    uint16_t n = 0;
    std::cin >> n;
    std::vector<std::vector<uint64_t>> a(n, std::vector<uint64_t>(n));

    for (uint16_t i = 0; i < n; ++i) {
        for (uint16_t j = 0; j < n; ++j) {
            std::cin >> a[i][j];
        }
    }

    std::vector<std::vector<uint64_t>> dp(1 << n, std::vector<uint64_t>(n, 0));
    std::vector<std::vector<uint16_t>> prev(1 << n, std::vector<uint16_t>(n, 0));
    for (uint64_t mask = 0; mask < (1 << n); ++mask) {
        for (uint16_t cur = 0; cur < n; ++cur) {
            if (mask & (1 << cur)) { // cur is in mask
                uint64_t min = 1e9;
                for (uint16_t j = 0; j < n; ++j) {
                    if (j != cur && mask & (1 << j)) { // j is in mask
                        if (dp[mask ^ (1 << cur)][j] + a[j][cur] < min) {
                            min = dp[mask ^ (1 << cur)][j] + a[j][cur];
                            prev[mask][cur] = j;
                        }
                    }
                }
                dp[mask][cur] = min == 1e9 ? 0 : min;
            }
        }
    }

    uint64_t ans = 1e9;
    uint16_t finish = 1e3;
    for (uint16_t j = 0; j < n; ++j) {
        if (dp[(1 << n) - 1][j] < ans) {
            ans = dp[(1 << n) - 1][j];
            finish = j;
        }
    }
    std::cout << ans << '\n';
    uint16_t cnt = n, cur = finish;
    uint64_t cur_mask = (1 << n) - 1;
    std::vector<uint16_t> path;
    while (cnt-- > 0) {
        path.push_back(cur);
        cur_mask ^= (1 << cur);
        cur = prev[cur_mask | (1 << cur)][cur];
    }
    for (uint16_t i = 1; i <= n; ++i) {
        std::cout << path[n - i] + 1 << ' ';
    }
}

int main() {
    solve();

    return 0;
}