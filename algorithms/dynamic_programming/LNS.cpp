/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/368711/problem/B
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

uint16_t bin_search(const std::vector<int32_t>& dp, uint16_t l, uint16_t r, int32_t val) {
    while (l + 1 < r) {
        uint16_t m = (l + r) >> 1;
        if (dp[m] < val) {
            r = m;
        } else {
            l = m;
        }
    }
    return r;
}

int main() {
    uint32_t n = 0;
    std::cin >> n;
    std::vector<int32_t> a(n);
    for (auto& item: a) {
        std::cin >> item;
    }

    std::vector<int32_t> dp(n + 1, INT_MIN);
    std::vector<int32_t> pos(n + 1, -1), prev(n + 1, -1);

    uint32_t length = 0;

    for (uint32_t i = 0; i < n; ++i) {
        uint32_t place = bin_search(dp, 0, n, a[i]);
        dp[place] = a[i];
        pos[place] = i;
        prev[i] = pos[place - 1];
        length = (length > place) ? length : place;
    }
    std::vector <uint32_t> answer;
    int32_t p = pos[length];
    while (p != -1) {
        answer.push_back(p + 1);
        p = prev[p];
    }

    std::cout << length << '\n';

    std::reverse(answer.begin(), answer.end());
    for (auto item: answer) {
        std::cout << item << ' ';
    }

    return 0;
}