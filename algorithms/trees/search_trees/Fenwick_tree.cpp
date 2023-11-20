/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/357142/problem/I
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::pair;

// 1. ДО из ДД
// 2. ДО из ДО - плохо по памяти (n^2)
// 3. ДО из фенвиков - также плохо по памяти, но константа поменьше

class FenwickTree {
private:
    vector <uint32_t> sums_;

    size_t f(uint32_t i)const {
        return i & (i + 1);
    }

    size_t g(uint32_t i)const {
        return i | (i + 1);
    }

    void Build(const vector<pair<uint32_t, bool>>& array) {
        vector<uint32_t> prefixes_of_array(array.size());
        prefixes_of_array[0] = array[0].second;
        for (uint32_t i = 1; i < array.size(); ++i) {
            prefixes_of_array[i] = prefixes_of_array[i - 1] + array[i].second;
        }

        for (uint32_t i = 0; i < sums_.size(); ++i) {
           sums_[i] = prefixes_of_array[i];
           if (f(i)) sums_[i] -= prefixes_of_array[f(i) - 1];
        }
    }

    uint32_t GetPrefixSum(uint32_t bound)const {
        uint32_t result = 0;
        for (int32_t i = bound; i >= 0; i = f(i) - 1) {
            result += sums_[i];
        }
        return result;
    }
public:
    FenwickTree() = default;
    FenwickTree(const vector<pair<uint32_t, bool>>& array) : sums_(array.size()) {
        Build(array);
    }

    uint32_t Query(uint32_t query_left, uint32_t query_right)const {
        uint32_t left_result = 0;
        if (query_left > 0) {
            left_result += GetPrefixSum(query_left - 1);
        }
        uint32_t right_result = GetPrefixSum(query_right);
        return right_result - left_result;
    }

    void Update(uint32_t prev_index, uint32_t new_index) {
        for (uint32_t i = prev_index; i < sums_.size(); i = g(i)) {
            sums_[i] -= 1;
        }
        for (uint32_t i = new_index; i < sums_.size(); i = g(i)) {
            sums_[i] += 1;
        }
    }

};

class SegmentTree {
private:
    struct Node {
        uint32_t left;
        uint32_t right;
        FenwickTree fenwick;
        vector<pair<uint32_t, bool>> array;

        Node() = default;
        Node(uint32_t left, uint32_t right, const vector<pair<uint32_t, bool>>& array) : left(left), right(right), array(array), fenwick(array) {}
        Node(uint32_t left, uint32_t right, const Node& node_1, const Node& node_2) : left(left), right(right) {
            Merge(node_1.array, node_2.array);
            fenwick = FenwickTree(array);
        }

        void Merge(const vector<pair<uint32_t, bool>>& left_array, const vector<pair<uint32_t, bool>>& right_array) {
            uint32_t left_ptr = 0, right_ptr = 0;
            bool flag = true;
            while (left_ptr < left_array.size() && right_ptr < right_array.size()) {
                if (left_array[left_ptr].first < right_array[right_ptr].first) {
                    array.push_back(left_array[left_ptr]);
                    ++left_ptr;
                    flag = true;
                } else if (left_array[left_ptr].first == right_array[right_ptr].first && flag) {
                    array.push_back(left_array[left_ptr]);
                    array.back().second = left_array[left_ptr].second || right_array[right_ptr].second;
                    ++left_ptr;
                    ++right_ptr;
                    flag = false;
                } else {
                    array.push_back(right_array[right_ptr]);
                    ++right_ptr;
                    flag = true;
                }
            }
            while (left_ptr < left_array.size()) {
                array.push_back(left_array[left_ptr]);
                ++left_ptr;
            }
            while (right_ptr < right_array.size()) {
                array.push_back(right_array[right_ptr]);
                ++right_ptr;
            }
        }
    };

    vector<Node> tree_;
    const uint32_t root_index_ = 1; 

    void Build(uint32_t v, uint32_t left, uint32_t right, const vector<vector<pair<uint32_t, bool>>>& array) {
        if (left == right - 1) {
            tree_[v] = Node(left, right, array[left]);
            return;
        } else {
            size_t middle = (left + right) >> 1;
            Build (2 * v, left, middle, array);
            Build (2 * v + 1, middle, right, array);
            tree_[v] = Node(left, right, tree_[2 * v], tree_[2 * v + 1]);
        }
    }
    
    uint32_t Query(uint32_t v, uint32_t query_left, uint32_t query_right, uint32_t lower_bound, uint32_t upper_bound) { 
        if (tree_[v].left >= query_left && tree_[v].right <= query_right) {
            lower_bound = GetNext(v, lower_bound);
            upper_bound = GetPrev(v, upper_bound);
            if (lower_bound == tree_[v].array.size() || upper_bound == tree_[v].array.size() || lower_bound > upper_bound) {
                return 0;
            } else {
                uint32_t res = tree_[v].fenwick.Query(lower_bound, upper_bound);
                return res;
            }
        }
        else if (tree_[v].left >= query_right || tree_[v].right <= query_left) {
            return 0;
        }
        else {
            uint32_t left_result = Query(2 * v, query_left, query_right, lower_bound, upper_bound);
            uint32_t right_result = Query(2 * v + 1, query_left, query_right, lower_bound, upper_bound);
            return left_result + right_result;
        }
    }

    uint32_t GetPrev(uint32_t v, uint32_t value) {
        uint32_t left_bound = 0, right_bound = tree_[v].array.size();
        while (left_bound < right_bound - 1) {
            uint32_t middle_bound = (left_bound + right_bound) >> 1;
            if (value >= tree_[v].array[middle_bound].first) {
                left_bound = middle_bound;
            } else {
                right_bound = middle_bound;
            }
        }
        if (value < tree_[v].array[left_bound].first) {
            left_bound = tree_[v].array.size();
        }
        return left_bound;
    }

    uint32_t GetNext(uint32_t v, uint32_t value) {
        uint32_t left_bound = 0, right_bound = tree_[v].array.size();
        while (left_bound + 1 < right_bound) { // right_bound м.б. равен нулю, а типы unsigned
            uint32_t middle_bound = (left_bound + right_bound) >> 1;
            if (value <= tree_[v].array[middle_bound].first) {
                right_bound = middle_bound;
            } else {
                left_bound = middle_bound;
            }
        }

        if (value <= tree_[v].array[0].first) {
            right_bound = 0;
        }
        return right_bound;
    }

    void Update(uint32_t v, uint32_t index, uint32_t prev_value, uint32_t new_value) {
        if (tree_[v].left == tree_[v].right - 1) {
            uint32_t prev_index = GetPrev(v, prev_value);
            uint32_t new_index = GetPrev(v, new_value);
            tree_[v].fenwick.Update(prev_index, new_index);
        } else {
            uint32_t middle = (tree_[v].left + tree_[v].right) >> 1;
            if (index < middle) {
                Update(2 * v, index, prev_value, new_value);
            }
            else {
                Update(2 * v + 1, index, prev_value, new_value);
            }
            uint32_t prev_index = GetPrev(v, prev_value);
            uint32_t new_index = GetPrev(v, new_value);
            tree_[v].fenwick.Update(prev_index, new_index);
        }
    }

    void show(uint32_t v) {
        if (tree_[v].left == tree_[v].right - 1) {
            std::cout << "[" << tree_[v].left << ", " << tree_[v].right << ") : ";
            for (uint32_t i = 0; i < tree_[v].array.size(); ++i) {
                cout << tree_[v].array[i].first << ' ';
            }
            cout << '\n';
        } else {
            std::cout << "[" << tree_[v].left << ", " << tree_[v].right << ") : ";
            for (uint32_t i = 0; i < tree_[v].array.size(); ++i) {
                cout << tree_[v].array[i].first << ' ';
            }
            cout << '\n';
            show(2 * v);
            show(2 * v + 1);
        }
    }

public:
    explicit SegmentTree(const vector<vector<pair<uint32_t, bool>>>& array) : tree_(4 * array.size()) {
        Build(root_index_, 0, array.size(), array);
    }

    uint32_t Query(uint32_t left, uint32_t right, uint32_t lower_bound, uint32_t upper_bound) {
        return Query(root_index_, left, right, lower_bound, upper_bound);
    }
    
    void Update(uint32_t index, uint32_t prev_value, uint32_t new_value) {
        Update(root_index_, index, prev_value, new_value);
    }

    void show() {
        show(root_index_);
    }
};

struct Query {
    string type_query;
    vector<uint32_t> values;
};

int main(int argc, char* argv[]) {
    uint32_t N = 0, M = 0;
    cin >> N >> M;

    vector<uint32_t> current_array(N);
    vector<vector<pair<uint32_t, bool>>> array(N);

    for (uint32_t i = 0; i < N; ++i) {
        uint32_t value = 0;
        cin >> value;
        array[i].push_back(std::make_pair(value, false));
        current_array[i] = value;
    }

    vector<Query> query_list; 

    while (M-- > 0) {
        string type_query;
        vector<uint32_t> values;
        cin >> type_query;
        if (type_query == "GET") {
            values.resize(4);
            for (uint32_t i = 0; i < 4; ++i) {
                cin >> values[i];
            }
        } else if (type_query == "SET") {
            values.resize(2);
            for (uint32_t i = 0; i < 2; ++i) {
                cin >> values[i];
            }
            array[values[0] - 1].push_back(std::make_pair(values[1], false));
        }
        query_list.push_back({type_query, values});
    }

    for (uint32_t i = 0; i < N; ++i) {
        std::sort(array[i].begin(), array[i].end());
        array[i].resize(unique(array[i].begin(), array[i].end()) - array[i].begin());
    }

    for (uint32_t i = 0; i < N; ++i) {
        for (uint32_t j = 0; j < N; ++j) {
            if (array[i][j].first == current_array[i]) {
                array[i][j].second = true;
                break;
            }
        }
    }
   
    SegmentTree tree(array);

    for (uint32_t i = 0; i < query_list.size(); ++i) {
        Query query = query_list[i];
        if (query.type_query == "GET") {
            uint32_t x = query.values[0], y = query.values[1], k = query.values[2], l = query.values[3];
            --x;
            cout << tree.Query(x, y, k, l) << '\n';
        } else if (query.type_query == "SET") {
            uint32_t a = query.values[0], b = query.values[1];
            --a;
            tree.Update(a, current_array[a], b);
            current_array[a] = b;
        }
    }

    return 0;
}
