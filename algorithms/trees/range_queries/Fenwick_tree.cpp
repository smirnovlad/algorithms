/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/350943/problem/F
 */

#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::vector;
using std::max;
using std::min;

struct Point{
    size_t x, y, z;

    Point() = default;
    Point(size_t x, size_t y, size_t z) : x(x), y(y), z(z) {}
};

class FenwickTree {
private:
    vector<vector<vector<int>>> sums_;

    size_t f(size_t i)const {
        return i & (i + 1);
    }

    size_t g(size_t i)const {
        return i | (i + 1);
    }

    int GetPrefixCubeSum(Point point)const {
        int result = 0;
        for (int i = point.x; i >= 0; i = f(i) - 1) {
            for (int j = point.y; j >= 0; j = f(j) - 1) {
                for (int k = point.z; k >= 0; k = f(k) - 1) {
                    result += sums_[i][j][k];
                }
            }
        }
        return result;
    }
public:
    explicit FenwickTree(const vector<vector<vector<int>>> array) : sums_(array) {}

    int Query(Point point_1, Point point_2)const {
        int result_0 = 0, result_1 = 0, result_2_1 = 0,  result_2_2 = 0,
                result_3_1 = 0, result_3_2 = 0, result_4_1 = 0, result_4_2 = 0;

        result_0 = GetPrefixCubeSum(Point(max(point_1.x, point_2.x),
                                          max(point_1.y, point_2.y),
                                          max(point_1.z, point_2.z)));
        // Эты if'ы можно не ставить. Первой итерациий цикла при подсчёте суммы не произойдёт.
        if (min(point_1.x, point_2.x) != 0) {
            result_1 = GetPrefixCubeSum(Point(min(point_1.x, point_2.x) - 1,
                                              max(point_1.y, point_2.y),
                                              max(point_1.z, point_2.z)));
            if (min(point_1.y, point_2.y) != 0) {
                result_2_1 = GetPrefixCubeSum(Point(min(point_1.x, point_2.x) - 1,
                                                    min(point_1.y, point_2.y) - 1,
                                                    max(point_1.z, point_2.z)));
            }
            if (min(point_1.z, point_2.z) != 0) {
                result_3_1 = GetPrefixCubeSum(Point(min(point_1.x, point_2.x) - 1,
                                                    max(point_1.y, point_2.y),
                                                    min(point_1.z, point_2.z) - 1));
            }
            if (min(point_1.y, point_2.y) != 0 && min(point_1.z, point_2.z) != 0) {
                result_4_1 = GetPrefixCubeSum(Point(min(point_1.x, point_2.x) - 1,
                                                    min(point_1.y, point_2.y) - 1,
                                                    min(point_1.z, point_2.z) - 1));
            }
        }

        if (min(point_1.y, point_2.y) != 0) {
            result_2_2 = GetPrefixCubeSum(Point(max(point_1.x, point_2.x),
                                                min(point_1.y, point_2.y) - 1,
                                                max(point_1.z, point_2.z)));
        }

        if (min(point_1.z, point_2.z) != 0) {
            result_3_2 = GetPrefixCubeSum(Point(max(point_1.x, point_2.x),
                                                max(point_1.y, point_2.y),
                                                min(point_1.z, point_2.z) - 1));
        }

        if (min(point_1.y, point_2.y) != 0 && min(point_1.z, point_2.z) != 0) {
            result_4_2 = GetPrefixCubeSum(Point(max(point_1.x, point_2.x),
                                                min(point_1.y, point_2.y) - 1,
                                                min(point_1.z, point_2.z) - 1));
        }
        int result = result_0 - result_1 - result_2_2 - result_3_2 + result_2_1 + result_3_1 + result_4_2 - result_4_1;
        return result;
    }

    void Update(Point point, int delta) {
        for (size_t i = point.x; i < sums_.size(); i = g(i)) {
            for (size_t j = point.y; j < sums_[i].size(); j = g(j)) {
                for (size_t k = point.z; k < sums_[i][j].size(); k = g(k)) {
                    sums_[i][j][k] += delta;
                }
            }
        }
    }
};

int main() {
    int n = 0;
    cin >> n;
    vector<vector<vector<int>>> array(n, vector<vector<int>>(n, vector<int>(n, 0)));
    FenwickTree tree(array);

    int query_type = 0;

    while (query_type != 3) {
        cin >> query_type;
        if (query_type == 1) {
            size_t x = 0, y = 0, z = 0;
            int delta = 0;
            cin >> x >> y >> z >> delta;
            array[x][y][z] += delta;
            tree.Update(Point(x, y, z), delta);
        } else if (query_type == 2) {
            Point point_1, point_2;
            cin >> point_1.x >> point_1.y >> point_1.z;
            cin >> point_2.x >> point_2.y >> point_2.z;
            cout << tree.Query(point_1, point_2) << '\n';
        }
    }

    return 0;
}
