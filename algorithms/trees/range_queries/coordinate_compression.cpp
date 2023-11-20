/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/350943/problem/E
 */

#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::pair;

class SegmentTree {
private:
    struct Node {
        size_t left;
        size_t right;
        long long value;

        Node() = default;
        Node (size_t left, size_t right, long long value) : left(left), right(right), value(value) {}
    };

    vector<Node> tree_;
    const size_t root_index_ = 1;

    void Build(size_t v, size_t left, size_t right) {
        if (left == right - 1) {
            tree_[v] = Node(left, right, 0);
            return;
        } else {
            size_t middle = (left + right) >> 1;
            Build (2 * v, left, middle);
            Build (2 * v + 1, middle, right);
            tree_[v] = Node(left, right, tree_[2 * v].value + tree_[2 * v + 1].value); // по факту - 0
        }
    }

    long long Query(size_t v, size_t index)const {
        if (tree_[v].right <= index + 1) {
            return tree_[v].value;
        } else if (tree_[v].left > index) {
            return 0;
        } else {
            size_t middle = (tree_[v].left + tree_[v].right) >> 1;
            long long left_result = Query(2 * v, index);
            long long right_result = Query(2 * v + 1, index);
            return left_result + right_result;
        }
    }

    void Update(size_t v, size_t index, size_t value) {
        if (tree_[v].left == tree_[v].right - 1) {
            tree_[v].value += value;
        } else {
            size_t middle = (tree_[v].left + tree_[v].right) >> 1;
            if (index < middle) {
                Update(2 * v, index, value);
            }
            else {
                Update(2 * v + 1, index, value);
            }
            tree_[v].value = tree_[2 * v].value + tree_[2 * v + 1].value;
        }
    }
public:
    explicit SegmentTree(size_t size) : tree_(4 * size) {
        Build(root_index_, 0, size);
    }

    long long Query(size_t index)const {
        return Query(root_index_, index);
    }

    void Update(size_t index, size_t value) {
        Update(root_index_, index, value);
    }
};

struct Query{
    size_t coord;
    char type_query;
};

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    vector<Query> query_list;
    vector<size_t> coords;

    size_t q;
    cin >> q;
    while (q-- > 0) {
        char type_query = '#';
        size_t position = 0;
        cin >> type_query;
        cin >> position;
        query_list.push_back({position, type_query});
        coords.push_back(position);
    }

    sort(coords.begin(), coords.end());
    coords.resize(unique(coords.begin(), coords.end()) - coords.begin());

    SegmentTree tree(coords.size());

    for (auto query: query_list) {
        size_t position = query.coord;
        size_t left_bound = 0, right_bound = coords.size();
        while (left_bound < right_bound - 1) {
            size_t middle = (left_bound + right_bound) >> 1;
            if (coords[middle] <= position) {
                left_bound = middle;
            } else {
                right_bound = middle;
            }
        }
        size_t query_index = left_bound;
        if (query.type_query == '+') {
            tree.Update(query_index, position);
        } else if (query.type_query == '?') {
            cout << tree.Query(query_index) << '\n';
        }
    }

    return 0;
}
