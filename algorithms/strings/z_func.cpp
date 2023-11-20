/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/400596/problem/A
 */

#include <iostream>
#include <vector>

void get_z_function(const std::string& s, std::vector<uint32_t>& zf) {
    uint32_t n = s.size();
    zf.assign(n, 0);
    uint32_t left = 0, right = 0;
    for (uint32_t i = 1; i < n; ++i) {
        if (i <= right) {
            zf[i] = std::min(right + 1 - i, zf[i - left]);
        }
        while (i + zf[i] < n && s[i + zf[i]] == s[zf[i]]) {
            ++zf[i];
        }
        if (i + zf[i] - 1 > right) {
            left = i;
            right = i + zf[i] - 1;
        }
    }
}

void solve() {
    std::string S, T;
    std::cin >> S >> T;
    std::string concatenated = T + S;
    std::vector<uint32_t> z_array;
    get_z_function(concatenated, z_array);
    for (uint32_t i = T.size(); i < z_array.size(); ++i) {
        if (z_array[i] >= T.size()) {
            std::cout << i - T.size() << ' ';
        }
    }
}

int main() {
    solve();

    return 0;
}