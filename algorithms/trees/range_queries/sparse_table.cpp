/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/350943/problem/H
 */

#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::pair;

const size_t INF = 1e9;

class SparseTable {
private:
    struct Indexes {
        size_t first_index;
        size_t second_index;

        Indexes() = default;

        Indexes(size_t first_index, size_t second_index) : first_index(first_index), second_index(second_index) {}

        Indexes(Indexes a, Indexes b, const vector<int>& array) {
            if (a.second_index == INF && b.second_index == INF) {
                first_index = a.first_index;
                second_index = b.first_index;
                if (array[first_index] > array[second_index]) {
                    std::swap(first_index, second_index);
                }
            } else {
                if (a.first_index != b.first_index) {
                    if (array[a.first_index] <= array[b.first_index]) {
                        first_index = a.first_index;
                        second_index = ((array[a.second_index] <= array[b.first_index]) ? a.second_index : b.first_index);
                    } else {
                        first_index = b.first_index;
                        second_index = ((array[a.first_index] <= array[b.second_index]) ? a.first_index : b.second_index);
                    }
                } else {
                    first_index = a.first_index;
                    second_index = ((array[a.second_index] <= array[b.second_index]) ? a.second_index : b.second_index);
                }
            }
        }
    };

    vector<vector<Indexes>> table_;
    vector<int> degrees_;

    void Build(const vector<int>& array) {
        degrees_[1] = 0;
        for (size_t i = 2; i < degrees_.size(); ++i) {
            degrees_[i] = degrees_[i / 2] + 1;
        }
        table_.resize(degrees_.size() - 1);

        table_[0].resize(array.size());
        for (size_t i = 0; i < array.size(); ++i) {
            table_[0][i] = Indexes(i, INF);
        }

        for (size_t level = 1; level <= degrees_[degrees_.size() - 1]; ++level) {
            table_[level].resize(array.size());
            for (size_t i = 0; i + (1 << level) <= array.size(); ++i) {
                table_[level][i] = Indexes(table_[level - 1][i], table_[level - 1][i + (1 << (level - 1))], array);
            }
        }
    }

public:
    explicit SparseTable(const vector<int>& array) : degrees_(array.size() + 1) {
        Build(array);
    }

    size_t Query(size_t query_left, size_t query_right, const vector<int>& array)const {
        size_t level = degrees_[query_right - query_left];
        Indexes ans = Indexes(table_[level][query_left], table_[level][query_right - (1 << level) + 1], array);
        return array[ans.second_index];
    }
};

int main() {
    int N = 0, M = 0;
    cin >> N >> M;
    vector<int> array(N);
    for (int& element: array) cin >> element;

    SparseTable table(array);

    while (M-- > 0) {
        size_t l = 0, r = 0;
        cin >> l >> r;
        cout << table.Query(l - 1, r - 1, array) << '\n';
    }


    return 0;
}