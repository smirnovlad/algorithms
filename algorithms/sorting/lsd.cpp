/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/347382/problem/H
 */

#include <iostream>
#include <vector>

using ll = long long;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<ll> a(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }
    std::vector<std::vector<ll>> bytes(256);
    std::vector<int> cnt(256);
    for (int i = 0; i < 8; ++i) {

        /*
         * эта сортировка - стабильная, п.ч. числа добавляются в
         * массив bytes по возрастанию предыдущих рассмотренных байтов и при равных i-ых байтах
         * взаимное положение чисел не изменится.
         */
        for (ll el: a) {
            ll val = (el >> i * 8) % (1 << 8);
            bytes[val].push_back(el);
        }
        a.clear();
        for (int j = 0; j < 256; ++j) {
            for (ll el: bytes[j]) a.push_back(el);
        }
        bytes.assign(256, {});
    }
    for (ll el: a) std::cout << el << ' ';

    return 0;
}