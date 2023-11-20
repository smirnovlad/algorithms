/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/368711/problem/G
 */

#include <iostream>
#include <vector>

int main() {
    uint16_t N = 0;
    std::cin >> N;
    std::vector<int16_t> a(N);
    for (auto& item: a) {
        std::cin >> item;
    }

    uint16_t M = 0;
    std::cin >> M;
    std::vector<int16_t> b(M);
    for (auto& item: b) {
        std::cin >> item;
    }

    std::vector<uint16_t> previous(M + 1, 0), current(M + 1, 0);

    for (uint16_t i = 1; i <= N; ++i) {
        for (uint16_t j = 0; j <= M; ++j) {
            current[j] = (j > 0) ? std::max(previous[j], current[j - 1]) : 0;
            if (j > 0 && a[i - 1] == b[j - 1]) {
                current[j] = previous[j - 1] + 1;
            }
        }
        previous = current;
    }

    std::cout << current[M] << '\n';

    return 0;
}