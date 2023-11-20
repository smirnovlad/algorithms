/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/371629/problem/J
 */

#include <iostream>
#include <vector>

bool isMatch(uint64_t firstMask, uint64_t secondMask, uint64_t n) {
    for (uint64_t i = 0; i < n - 1; ++i) {
        bool color = (firstMask >> i) & 1;
        if (((firstMask >> (i + 1)) & 1) == color && ((secondMask >> i) & 1) == color
            && ((secondMask >> (i + 1)) & 1) == color) {
            return false;
        }
    }
    return true;
}

void solve() {
    uint64_t n = 0, m = 0;
    std::cin >> n >> m;
    if (n > m) {
        std::swap(n, m);
    }
    std::vector<std::vector<uint64_t>> dp(m, std::vector<uint64_t>(1 << n, 0));
    for (uint64_t mask = 0; mask < (1 << n); ++mask) {
        dp[0][mask] = 1;
    }
    for (uint64_t j = 1; j < m; ++j) {
        for (uint64_t secondMask = 0; secondMask < (1 << n); ++secondMask) {
            for (uint64_t firstMask = 0; firstMask < (1 << n); ++firstMask) {
                if (isMatch(firstMask, secondMask, n)) {
                    dp[j][secondMask] += dp[j - 1][firstMask];
                }
            }
        }
    }
    uint64_t ans = 0;
    for (auto val: dp[m - 1]) {
        ans += val;
    }
    std::cout << ans << '\n';
}

int main() {
    solve();

    return 0;
}