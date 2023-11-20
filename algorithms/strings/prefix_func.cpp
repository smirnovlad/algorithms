/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/400596/problem/A
 */

#include <iostream>
#include <vector>

void get_pref_function(const std::string& s, std::vector<uint32_t>& pf) {
    uint32_t n = s.size();
    pf.assign(n, 0);
    for (uint32_t i = 1; i < n; ++i) {
        uint32_t j = pf[i - 1];
        while (j != 0 && s[i] != s[j]) {
            j = pf[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
        pf[i] = j;
    }
}

void solve() {
    std::string S, T;
    std::cin >> S >> T;
    std::string concatenated = T + "#" + S;
    std::vector<uint32_t> pref_array;
    get_pref_function(concatenated, pref_array);
    for (uint32_t i = T.size() + 1; i < pref_array.size(); ++i) {
        if (pref_array[i] == T.size()) {
            std::cout << (i - T.size() + 1) - 1 - T.size() << ' ';
        }
    }
}

int main() {
    solve();

    return 0;
}