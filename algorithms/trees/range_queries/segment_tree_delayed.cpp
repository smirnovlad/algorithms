/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/350943/problem/I
 */

#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::min;

const size_t INF = 1e9;

class SegmentTree {
private:
    struct Node {
        size_t left;
        size_t right;
        size_t value;

        Node() = default;
        Node (size_t left, size_t right, size_t value) : left(left), right(right), value(value) {}
    };

    vector<Node> tree_;
    const size_t root_index_ = 1;
    vector<size_t> promise;

    void Build(size_t v, size_t left, size_t right, const vector<size_t>& array) {
        if (left == right - 1) {
            tree_[v] = Node(left, right, array[left]);
            return;
        } else {
            size_t middle = (left + right) >> 1;
            Build (2 * v, left, middle, array);
            Build (2 * v + 1, middle, right, array);
            tree_[v] = Node(left, right, min(tree_[2 * v].value, tree_[2 * v + 1].value));
        }
    }

    size_t Query(size_t v, size_t query_left, size_t query_right) {
        if (tree_[v].left >= query_left && tree_[v].right <= query_right) {
            return tree_[v].value;
        } else if (tree_[v].left >= query_right || tree_[v].right <= query_left) {
            return INF;
        } else {
            push(v);
            size_t left_result = Query(2 * v, query_left, query_right);
            size_t right_result = Query(2 * v + 1, query_left, query_right);
            return min(left_result, right_result);
        }
    }

    void push(size_t v) {
        if (promise[v] == -1) {
            return;
        } else {
            promise[2 * v] = promise[v];
            promise[2 * v + 1] = promise[v];
            promise[v] = -1;
            tree_[2 * v].value = promise[2 * v];
            tree_[2 * v + 1].value = promise[2 * v + 1];
        }
    }

    void assign(size_t v, size_t query_left, size_t query_right, size_t new_value) {
        if (tree_[v].left >= query_left && tree_[v].right <= query_right) {
            promise[v] = new_value;
            tree_[v].value = new_value;
            return;
        } else if (tree_[v].left >= query_right || tree_[v].right <= query_left) {
            return;
        } else {
            push(v);
            assign(2 * v, query_left, query_right, new_value);
            assign(2 * v + 1, query_left, query_right, new_value);
            tree_[v].value = min(tree_[2 * v].value, tree_[2 * v + 1].value);
        }
    }

public:
    explicit SegmentTree(const vector<size_t>& array) : tree_(4 * array.size()), promise(4 * array.size(), -1) {
        Build(root_index_, 0, array.size(), array);
    }

    size_t Query(size_t left, size_t right) {
        return Query(root_index_, left, right);
    }

    void assign(size_t left, size_t right, size_t new_value) {
        assign(root_index_, left, right, new_value);
    }
};

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    size_t N = 0;
    cin >> N;
    vector<size_t> segment(N);
    for (size_t& element: segment) {
        size_t R = 0, G = 0, B = 0;
        cin >> R >> G >> B;
        element = R + G + B;
    }

    SegmentTree tree(segment);

    size_t K = 0;
    cin >> K;
    while (K-- > 0) {
        size_t C = 0, D = 0, R = 0, G = 0, B = 0, E = 0, F = 0;
        cin >> C >> D >> R >> G >> B >> E >> F;
        tree.assign(C, D + 1, R + G + B);
        cout << tree.Query(E, F + 1) << ' ';
    }

    return 0;
}