/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/400596/problem/G
 */

#include <iostream>
#include <vector>
#include <map>

uint32_t sigma = 256;

void solve() {
    std::string s;
    std::cin >> s;
    s += "#";
    uint32_t n = s.size();
    std::vector<uint32_t> p(n, n);
    std::vector<uint32_t> cnt(sigma, 0);
    std::vector<uint32_t> c(n, 0);
    for (uint32_t i = 0; i < n; ++i) {
        ++cnt[s[i]];
    }
    for (uint32_t i = 1; i < sigma; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int32_t i = n - 1; i >= 0; --i) {
        p[--cnt[s[i]]] = i;
    }
    c[p[0]] = 0;
    for (uint32_t i = 1; i < n; ++i) {
        c[p[i]] = c[p[i - 1]];
        if (s[p[i]] != s[p[i - 1]]) {
            ++c[p[i]];
        }
    }
    std::vector<int32_t> pn(n, n);
    std::vector<uint32_t> cn(n, n);
    cnt.resize(n);
    for (uint32_t exp_two = 1; exp_two <= n; exp_two *= 2) {
        for (uint32_t i = 0; i < n; ++i) {
            pn[i] = p[i] - exp_two;
            if (pn[i] < 0) pn[i] += n;
        }
        cnt.assign(cnt.size(), 0);
        for (uint32_t i = 0; i < n; ++i) {
            ++cnt[c[pn[i]]];
        }
        for (uint32_t i = 1; i < n; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int32_t i = n - 1; i >= 0; --i) {
            p[--cnt[c[pn[i]]]] = pn[i];
        }
        cn[p[0]] = 0;
        for (uint32_t i = 1; i < n; ++i) {
            cn[p[i]] = cn[p[i - 1]];
            if (c[p[i]] != c[p[i - 1]]) {
                ++cn[p[i]];
            } else if (c[(p[i] + exp_two) % n] != c[(p[i - 1] + exp_two) % n]) {
                ++cn[p[i]];
            }
        }
        c = cn;
    }
    for (uint32_t i = 1; i < n; ++i) {
        std::cout << p[i] + 1 << ' ';
    }
}

int main() {
    solve();

    return 0;
}