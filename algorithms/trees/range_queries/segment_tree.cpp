/*
 * RVQ: https://codeforces.com/group/PVbQ8eK2T4/contest/350943/problem/A
 */

#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::pair;

const int INF = 1e9;
const int SIZE = 1e5;

const int Mod1 = 12345, Mod2 = 23456;

class SegmentTree{
private:
    struct Node{
        int left, right;
        int min_value, max_value;
        
        Node() = default;
        Node(size_t left, size_t right, int min_value, int max_value) : left(left), right(right), min_value(min_value), max_value(max_value) {}
    };

    vector<Node> tree_;
    const size_t root_index_ = 1;
    
    void Build(size_t v, size_t left, size_t right, const vector<int>& array) {
        if (left == right - 1) {
            tree_[v] = Node(left, right, array[left], array[left]);
            return;
        } else {
            size_t middle = (left + right) >> 1;
            Build(2 * v, left, middle, array);
            Build(2 * v + 1, middle, right, array);
            tree_[v] = Node(left, right, std::min(tree_[2 * v].min_value, tree_[2 * v + 1].min_value), 
                    std::max(tree_[2 * v].max_value, tree_[2 * v + 1].max_value));
        }
    }
    
    pair<int, int> Query(size_t v, size_t query_left, size_t query_right)const {
       if (tree_[v].left >= query_left && tree_[v].right <= query_right) {
           return std::make_pair(tree_[v].min_value, tree_[v].max_value);
       } else if (tree_[v].left >= query_right || tree_[v].right <= query_left) {
           return std::make_pair(INF, -INF);
       } else {
           pair<int, int> left_result = Query(2 * v , query_left, query_right);
           pair<int, int> right_result = Query(2 * v + 1, query_left, query_right);
           return std::make_pair(std::min(left_result.first, right_result.first), std::max(left_result.second, right_result.second));
       }
    }
    
    void Update(size_t v, size_t index, int value) {
        if (tree_[v].left == tree_[v].right - 1) {
            tree_[v].min_value = value;
            tree_[v].max_value = value;
            return;
        } else {
            size_t middle = (tree_[v].left + tree_[v].right) >> 1;
            if (index < middle) {
                Update(2 * v, index, value);
            } else {
                Update(2 * v + 1, index, value);
            }
            tree_[v].min_value = std::min(tree_[2 * v].min_value, tree_[2 * v + 1].min_value);
            tree_[v].max_value = std::max(tree_[2 * v].max_value, tree_[2 * v + 1].max_value);
        }
    }

public:
    explicit SegmentTree(const vector<int>& array) : tree_(4 * array.size()) {
        Build (root_index_, 0, array.size(), array);
    }
    
    pair<int, int> Query(size_t left, size_t right)const {
        return Query(root_index_, left, right);
    }
    
    void Update(size_t index, int value) {
        Update(root_index_, index, value);
    }

};

int main() {
    std::vector<int> a(SIZE);

    vector <long long> squares(SIZE + 1), cubes(SIZE + 1);

    for (long long i = 1; i < SIZE + 1; ++i) {
        squares[i] = i * i;
        cubes[i] = i * i * i;
    }

    for (int i = 0; i < SIZE; ++i) {
        a[i] = squares[i + 1] % Mod1 + cubes[i + 1] % Mod2;
    }
    
    SegmentTree tree(a);

    int k = 0;
    cin >> k;
    while (k-- > 0) {    
        int x = 0, y = 0;
        cin >> x >> y;
        if (x > 0) {
            pair<int, int> current_pair = tree.Query(x - 1, y);
            std::cout << current_pair.second - current_pair.first << '\n';
        } else if (x < 0) {
            tree.Update(std::abs(x) - 1, y);
        }
    }

    return 0;
}
