/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/368711/problem/K
 */

#include <iostream>
#include <vector>

int main() {
    uint16_t S = 0, N = 0;
    std::cin >> S >> N;

    std::vector<uint32_t> weight(N);
    for (auto& item: weight) {
        std::cin >> item;
    }

    std::vector<bool> previous(S + 1, false), current(S + 1, false);
    previous[0] = true;

    for (uint16_t i = 1; i <= N; ++i) {
        for (uint16_t w = 0; w <= S; ++w) {
            current[w] = previous[w];
            if (!current[w] && weight[i - 1] <= w) {
                current[w] = previous[w - weight[i - 1]];
            }
        }
        previous = current;
    }

    for (uint16_t w = S; w >= 0; --w) {
        if (current[w] || w == 0) {
            std::cout << w << '\n';
            break;
        }
    }

    return 0;
}