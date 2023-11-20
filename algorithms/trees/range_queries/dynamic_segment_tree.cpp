/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/350943/problem/E
 */

#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

class DynamicSegmentTree {
private:
    struct Node {
        size_t left_bound, right_bound;
        size_t left_son, right_son;
        long long value;

        Node() = default;
        Node (size_t left_bound, size_t right_bound, size_t left_son, size_t right_son, long long value) :
                left_bound(left_bound), right_bound(right_bound), left_son(left_son), right_son(right_son), value(value) {}
    };

    vector<Node> tree_;
    size_t root_index_ = 0;

    long long Query(size_t v, size_t position)const {
        if (tree_[v].right_bound <= position + 1) {
            return tree_[v].value;
        } else if (tree_[v].left_bound > position) {
            return 0;
        } else {
            size_t middle = (tree_[v].left_bound + tree_[v].right_bound) >> 1;
            long long left_result = 0, right_result = 0;
            if (tree_[v].left_son != -1) left_result = Query(tree_[v].left_son, position);
            if (tree_[v].right_son != -1) right_result = Query(tree_[v].right_son, position);
            return left_result + right_result;
        }
    }

    void Update(size_t v, size_t position) {
        tree_[v].value += position;
        if (tree_[v].left_bound == tree_[v].right_bound - 1) {
            return;
        } else {
            size_t middle = (tree_[v].left_bound + tree_[v].right_bound) >> 1;
            if (position < middle && tree_[v].left_son == -1) {
                tree_.push_back(Node(tree_[v].left_bound, middle, -1, -1, 0));
                tree_[v].left_son = tree_.size() - 1;
            }
            if (position < middle) {
                Update(tree_[v].left_son, position);
            }
            if (position >= middle && tree_[v].right_son == -1) {
                tree_.push_back(Node(middle, tree_[v].right_bound, -1, -1, 0));
                tree_[v].right_son = tree_.size() - 1;
            }
            if (position >= middle) {
                Update(tree_[v].right_son, position);
            }
        }

    }

public:
    DynamicSegmentTree() {
        tree_.push_back(Node(0, 1e9, -1, -1, 0));
    }

    long long Query(size_t position)const {
        return Query(root_index_, position);
    }

    void Update(size_t position) {
        Update(root_index_, position);
    }
};

int main(int argc, char* argv[]) {
    int query_count = 0;
    cin >> query_count;

    DynamicSegmentTree tree;
    size_t size = 0;
    while(query_count-- > 0) {
        char c = '#';
        cin >> c;
        if (c == '+') {
            size_t position = 0;
            cin >> position;
            tree.Update(position);
        } else if (c == '?') {
            size_t position = 0;
            cin >> position;
            cout << tree.Query(position) << '\n';
        }
    }

    return 0;
}
