/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/368711/problem/H
 */

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<uint16_t> LCS(bool rev, std::string& a, std::string& b,
                          uint16_t la, uint16_t ra,
                          uint16_t lb, uint16_t rb) {
    uint16_t N = ra - la + 1, M = rb - lb + 1;
    std::vector<uint16_t> previous(M + 1, 0), current(M + 1, 0);

    for (uint16_t i = 1; i <= N; ++i) {
        for (uint16_t j = 1; j <= M; ++j) {
            current[j] = std::max(previous[j], current[j - 1]);
            if (!rev) {
                if (a[la + i - 1] == b[lb + j - 1]) {
                    current[j] = previous[j - 1] + 1;
                }
            } else {
                if (a[ra + 1 - i] == b[rb + 1 - j]) {
                    current[j] = previous[j - 1] + 1;
                }
            }
        }
        previous = current;
    }
    return current;
}

void hirschberg(std::string& answer, std::string& a, std::string& b,
                uint16_t la, uint16_t ra,
                uint16_t lb, uint16_t rb) {
    if (la == ra) {
        for (uint16_t j = lb; j <= rb; ++j) {
            if (b[j] == a[la]) {
                answer += a[la];
                break;
            }
        }
        return;
    }

    uint16_t mid = (ra + la) / 2;

    std::vector<uint16_t> pref = LCS(false, a, b, la, mid, lb, rb);
    std::vector<uint16_t> suff = LCS(true, a, b, mid + 1, ra, lb, rb);

    uint16_t it_max = 0, M = rb - lb + 1;
    uint16_t max = suff[it_max];
    for (uint16_t j = 0; j <= M; ++j) {
        if (pref[j] + suff[M - j] > max) {
            max = pref[j] + suff[M - j];
            it_max = j;
        }
    }

    if (it_max != 0) {
        hirschberg(answer, a, b, la, mid, lb, lb + it_max - 1);
    }
    hirschberg(answer, a, b, mid + 1, ra, lb + it_max, rb);
}

int main() {
    std::string a, b;
    std::cin >> a >> b;

    std::string answer;
    hirschberg(answer, a, b, 0, a.size() - 1, 0, b.size() - 1);

    std::cout << answer << '\n';

    return 0;
}