/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/371629/problem/L
 */

#include <iostream>
#include <vector>

const uint16_t max = 17;
const uint32_t Mod = 1e9 + 7;

uint64_t getAnswer(const std::vector<std::vector<uint64_t>> &matrix) {
    uint64_t result = 0;
    for (auto item: matrix[0]) {
        result += item;
        result %= Mod;
    }
    return result;
}

std::vector<std::vector<uint64_t>> operator*(
        const std::vector<std::vector<uint64_t>> &firstMatrix,
        const std::vector<std::vector<uint64_t>> &secondMatrix) {
    uint16_t n = firstMatrix.size(),
            k = firstMatrix[0].size(),
            m = secondMatrix[0].size();
    std::vector<std::vector<uint64_t>> result(n, std::vector<uint64_t>(m));
    for (uint16_t i = 0; i < n; ++i) {
        for (uint16_t j = 0; j < k; ++j) {
            for (uint16_t l = 0; l < m; ++l) {
                result[i][j] += firstMatrix[i][l] * secondMatrix[l][j];
                result[i][j] %= Mod;
            }
        }
    }
    return result;
}

std::vector<std::vector<uint64_t>> binaryExponentiation(
        const std::vector<std::vector<uint64_t>> &matrix,
        uint64_t n) {
    if (n % 2) {
        if (n == 1) {
            return matrix;
        } else {
            return matrix * binaryExponentiation(matrix, n - 1);
        }
    } else {
        auto half = binaryExponentiation(matrix, n / 2);
        return half * half;
    }
}

bool isMatch(uint64_t firstMask, uint64_t secondMask, uint64_t n) {
    for (uint64_t i = 0; i < n - 1; ++i) {
        bool color = (firstMask >> i) & 1;
        if (((firstMask >> (i + 1)) & 1) == color && ((secondMask >> i) & 1) == color
            && ((secondMask >> (i + 1)) & 1) == color) {
            return false;
        }
    }
    return true;
}

void solve() {
    uint16_t n = 0;
    uint64_t k = 0;
    std::cin >> n >> k;
    std::vector<std::vector<uint64_t>> current(1, std::vector<uint64_t>(max, 0));
    std::vector<std::vector<uint64_t>> matrix(max, std::vector<uint64_t>(max, 0));
    current[0][0] = 1;
    while (n-- > 0) {
        uint64_t a = 0, b = 0;
        uint16_t c = 0;
        std::cin >> a >> b >> c;
        if (n == 0) {
            b = k;
        }
        for (uint16_t i = 0; i < max; ++i) {
            matrix[i].assign(max, 0);
        }

        for (uint16_t i = 0; i < max; ++i) {
            if (i <= c) {
                matrix[i][i] = 1;
                if (i != 0) {
                    matrix[i - 1][i] = 1;
                }
                if (i != c) {
                    matrix[i + 1][i] = 1;
                }
            }
        }
        if (b - a > 0) { // if k != a
            matrix = binaryExponentiation(matrix, b - a);
            current = current * matrix;
        }
    }
    std::cout << current[0][0] << '\n';
}

int main() {
    solve();

    return 0;
}