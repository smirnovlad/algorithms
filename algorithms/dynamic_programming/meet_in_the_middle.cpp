/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/371629/problem/M
 */

#include <iostream>
#include <vector>

void get_klika(std::vector<uint32_t> &neighbor,
               std::vector<uint32_t> &is_klika,
               uint32_t sz, uint32_t to_add = 0) {
    is_klika[0] = 1;
    int32_t oldest = -1;
    for (uint32_t mask = 1; mask < (1 << sz); ++mask) {
        if (!(mask & (mask - 1))) {
            ++oldest;
        }
        uint32_t xor_mask = mask ^ (1 << oldest);
        uint32_t id = oldest + to_add;
        is_klika[mask] = is_klika[xor_mask] && ((xor_mask | neighbor[id]) ^ neighbor[id]) == 0;
    }
}

void solve() {
    uint64_t n = 0;
    std::cin >> n;

    uint32_t sz_A = n / 2, sz_B = n - sz_A;
    std::vector<uint32_t> is_klika_A(1 << sz_A, false);
    std::vector<uint32_t> is_klika_B(1 << sz_B, 0);
    std::vector<uint32_t> neighbor_from_A(n), neighbor_from_B(n);

    for (uint64_t i = 0; i < n; ++i) {
        for (uint64_t j = 0; j < n; ++j) {
            char is_connected = '0';
            std::cin >> is_connected;
            if (is_connected == '1') {
                if (j < sz_A) {
                    neighbor_from_A[i] |= (1 << j);
                } else {
                    neighbor_from_B[i] |= (1 << (j - sz_A));
                }
            }
        }
    }

    get_klika(neighbor_from_A, is_klika_A, sz_A);
    get_klika(neighbor_from_B, is_klika_B, sz_B, sz_A);

    for (uint32_t i = 0; i < sz_B; ++i) {
        for (uint32_t mask = 0; mask < (1 << sz_B); ++mask) {
            if (!((mask >> i) & 1)) {
                is_klika_B[mask + (1 << i)] += is_klika_B[mask];
            }
        }
    }

    std::vector<uint32_t> dp(1 << sz_A, 0);
    dp[0] = (1 << sz_B) - 1;
    int64_t oldest = -1;
    for (uint64_t mask = 1; mask < (1 << sz_A); ++mask) {
        if (!(mask & (mask - 1))) {
            ++oldest;
        }
        uint64_t xor_mask = mask ^ (1 << oldest);
        dp[mask] = dp[xor_mask] & (neighbor_from_B[oldest]);
    }

    uint64_t ans = 0;
    for (uint64_t mask = 0; mask < (1 << sz_A); ++mask) {
        if (is_klika_A[mask]) {
            ans += is_klika_B[dp[mask]];
        }
    }
    std::cout << ans << '\n';
}

int main() {
    solve();

    return 0;
}