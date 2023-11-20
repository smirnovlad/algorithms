/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/405815/problem/E
 */

#include <iostream>
#include <vector>
#include <complex>

// source: EMAXX

using base = std::complex<double>;
const double PI = 3.14159265358979323846;

uint32_t rev(uint32_t num, uint32_t log) {
    uint32_t res = 0;
    for (uint32_t i = 0; i < log; ++i) {
        if (num & (1 << i)) {
            res |= (1 << (log - i - 1));
        }
    }
    return res;
}

void fft(std::vector<base> &a, bool invert) {
    uint32_t n = a.size();
    uint32_t log = 0;

    while ((1 << log) != n) {
        ++log;
    }

    for (uint32_t i = 0; i < n; ++i) {
        if (i < rev(i, log)) {
            swap(a[i], a[rev(i, log)]);
        }
    }

    for (uint32_t len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        base wn(cos(ang), sin(ang));
        for (uint32_t i = 0; i < n; i += len) {
            base w = 1;
            for (uint32_t j = 0; j < len / 2; ++j) {
                base u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wn;
            }
        }
    }

    if (invert) {
        for (uint32_t i = 0; i < n; ++i) {
            a[i] /= n;
        }
    }
}

void multiply(const std::vector<int32_t> &a,
              const std::vector<int32_t> &b,
              std::vector<int32_t> &res) {
    std::vector<base> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    uint32_t n = a.size();
    fft(fa, false);
    fft(fb, false);
    std::vector<base> fres(n);
    for (uint32_t i = 0; i < n; ++i) {
        fres[i] = fa[i] * fb[i];
    }
    fft(fres, true);
    for (uint32_t i = 0; i < n; ++i) {
        res[i] = llround(fres[i].real());
    }
}

void solve() {
    uint32_t n = 0;
    std::cin >> n;
    std::vector<int32_t> a(n + 1);
    for (uint32_t i = 0; i <= n; ++i) {
        std::cin >> a[n - i];
    }

    uint32_t m = 0;
    std::cin >> m;
    std::vector<int32_t> b(m + 1);
    for (uint32_t i = 0; i <= m; ++i) {
        std::cin >> b[m - i];
    }

    uint32_t deg = 1;
    while (deg <= n + m) {
        deg *= 2;
    }

    a.resize(deg);
    for (uint32_t i = n + 1; i < deg; ++i) {
        a[i] = 0;
    }

    b.resize(deg);
    for (uint32_t i = m + 1; i < deg; ++i) {
        b[i] = 0;
    }

    std::vector<int32_t> res(deg, 0);
    multiply(a, b, res);
    std::cout << n + m << ' ';
    int32_t i = deg - 1;
    while (res[i] == 0) {
        --i;
    }
    while (i >= 0) {
        std::cout << res[i--] << ' ';
    }
    std::cout << '\n';
}

int main() {
    solve();

    return 0;
}