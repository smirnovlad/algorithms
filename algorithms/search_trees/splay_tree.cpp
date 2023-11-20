/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/357142/problem/B
 */

#include <iostream>
#include <string>

const uint32_t Mod = 1e9;

class SplayTree {
private:
    struct Node {
        Node* left = nullptr;
        Node* right = nullptr;
        size_t key;
        long long sum;

        explicit Node(size_t key, size_t sum) : key(key), sum(sum) {}
    };

    Node* RotateRight(Node* previous_node) {
        Node* new_node = previous_node->left;
        if (new_node != nullptr) {
            previous_node->sum -= new_node->sum;
            previous_node->left = new_node->right;
            if (new_node->right != nullptr) {
                previous_node->sum += new_node->right->sum;
                new_node->sum -= new_node->right->sum;
            }
            new_node->right = previous_node;
            new_node->sum += previous_node->sum;
            return new_node;
        } else {
            return previous_node;
        }
    }

    Node* RotateLeft(Node* previous_node) {
        Node* new_node = previous_node->right;
        if (new_node != nullptr) {
            previous_node->sum -= new_node->sum;
            previous_node->right = new_node->left;
            if (new_node->left != nullptr) {
                previous_node->sum += new_node->left->sum;
                new_node->sum -= new_node->left->sum;
            }
            new_node->left = previous_node;
            new_node->sum += previous_node->sum;
            return new_node;
        } else {
            return previous_node;
        }
    }

    Node* Splay(Node* node, size_t key) {
        if (node == nullptr || node->key == key) {
            return node;
        } else {
            if (node->key > key) {
                if (node->left == nullptr) {
                    return node;
                } else if (node->left->key > key) {
                    node->left->left = Splay(node->left->left, key);
                    node = RotateRight(node);
                } else if (node->left->key < key) {
                    node->left->right = Splay(node->left->right, key);
                    if (node->left->right != nullptr) {
                        node->left = RotateLeft(node->left);
                    }
                }
                return (node->left == nullptr) ? node : RotateRight(node);
            } else if (node->key < key) {
                if (node->right == nullptr) {
                    return node;
                } else if (node->right->key < key) {
                    node->right->right = Splay(node->right->right, key);
                    node = RotateLeft(node);
                } else if (node->right->key > key) {
                    node->right->left = Splay(node->right->left, key);
                    if (node->right->left != nullptr) {
                        node->right = RotateRight(node->right);
                    }
                }
                return (node->right == nullptr) ? node : RotateLeft(node);
            }
        }
    }

    Node* Insert(Node* node, size_t key) {
        if (node == nullptr) {
            node = new Node(key, key);
        } else {
            node->sum += key;
            if (key < node->key) {
                node->left = Insert(node->left, key);
            } else if (key > node->key) {
                node->right = Insert(node->right, key);
            }
        }
        return node;
    }

    Node* Find(Node* node, size_t key) {
        Node* result = nullptr;
        if (node != nullptr) {
            if (key == node->key) {
                result = node;
            } else if (key < node->key) {
                result = Find(node->left, key);
            } else if (key > node->key) {
                result = Find(node->right, key);
            }
        }
        return result;
    }

    Node* Prev(Node* node, size_t key) {
        Node* result = nullptr;
        if (node != nullptr) {
            if (key >= node->key) {
                result = node;
                Node* candidate = Prev(node->right, key);
                result = (candidate && result->key < candidate->key) ? candidate : result;
            } else if (key < node->key) {
                if (node->left != nullptr) {
                    result = Prev(node->left, key);
                }
            }
        }
        return result;
    }

    Node* Next(Node* node, size_t key) {
        Node* result = nullptr;
        if (node != nullptr) {
            if (key <= node->key) {
                result = node;
                Node* candidate = Next(node->left, key);
                result = (candidate && result->key > candidate->key) ? candidate : result;
        } else if (key > node->key) {
                if (node->right != nullptr) {
                    result = Next(node->right, key);
                }
            }
        }
        return result;
    } 
    
    static Node* GetMax(Node* node) {
        return node->right ? GetMax(node->right) : node;
    }
    
    static Node* GetMin(Node* node) {
        return node->left ? GetMin(node->left) : node;
    }

    static Node* EraseMin(Node* node) {
        if (node->left) {
            node->left = EraseMin(node->left);
        } else {
            return node->right;
        }
        return node;
    }

    static Node* Erase(Node* node, const size_t& key) {
        if (node == nullptr) {
            return nullptr;
        } else {
            if (key < node->key) {
                node->sum -= key;
                node->left = Erase(node->left, key);
            } else if (key > node->key) {
                node->sum -= key;
                node->right = Erase(node->right, key);
            } else if (key == node->key) {
                Node *left_son = node->left, *right_son = node->right;
                delete node;
                if (right_son == nullptr) {
                    return left_son;
                } else {
                    Node* min_value_node = GetMin(right_son);
                    min_value_node->right = EraseMin(right_son);
                    min_value_node->left = left_son;
                    return min_value_node;
                }
            }
            return node;
        }
    } 

    Node* root_ = nullptr; 

public: 
    SplayTree() = default;
    SplayTree(Node* node) : root_(node) {}
    SplayTree(const SplayTree& tree) : root_(tree.root_) {}

    friend std::pair<SplayTree, SplayTree> Split(SplayTree, size_t);
    friend SplayTree Merge(SplayTree, SplayTree);

    size_t GetSum(size_t query_left, size_t query_right) {
        size_t result = 0;
        
        Node* next_left = Next(root_, query_left);
        Node* prev_right = Prev(root_, query_right);

        if (next_left != nullptr && next_left->key <= query_right) {
            SplayTree tree_1, tree_2, tree_3;
            std::pair<SplayTree, SplayTree> pair_1 = Split(*this, next_left->key);
            tree_1 = pair_1.first;
            tree_2 = pair_1.second;
            std::pair<SplayTree, SplayTree> pair_2 = Split(tree_2, prev_right->key);
            tree_2 = pair_2.first;
            tree_3 = pair_2.second;
            if (tree_2.root_ != nullptr) {
                result = tree_2.root_->sum;
            }
            result += prev_right->key;
            tree_2 = Merge(tree_2, tree_3);
            *this = Merge(tree_1, tree_2);
        }
        return result;
    }

    void Insert(size_t key) {
        if (Find(root_, key) == nullptr) {
            root_ = Insert(root_, key);
            Splay(key);
        }
    }

    void Splay(size_t key) {
        root_ = Splay(root_, key);
    } 
};

std::pair<SplayTree, SplayTree> Split(SplayTree tree, size_t key) {
    if (tree.root_ != nullptr) {
        tree.Splay(key);
        auto root_1 = tree.root_->left;
        if (tree.root_->left != nullptr) {
            tree.root_->sum -= tree.root_->left->sum;
        }
        tree.root_->left = nullptr;
        auto root_2 = tree.root_;
        return std::make_pair(SplayTree(root_1), SplayTree(root_2));
    } else {
        return std::make_pair(SplayTree(nullptr), SplayTree(nullptr));
    }
}

// tree_1_keys < tree_2_keys
SplayTree Merge(SplayTree tree_1, SplayTree tree_2) {
    SplayTree merged;
    if (tree_1.root_ != nullptr) {
        merged.Insert(tree_1.GetMax(tree_1.root_)->key);
        merged.root_->left = tree_1.Erase(tree_1.root_, merged.root_->key);
        if (merged.root_->left != nullptr) {
            merged.root_->sum += merged.root_->left->sum;
        }
        merged.root_->right = tree_2.root_;
        if (merged.root_->right != nullptr) {
            merged.root_->sum += merged.root_->right->sum;
        }
    } else {
        merged.root_ = tree_2.root_;
    }
    return merged;
}

int main(int argc, char* argv[]) {
    uint32_t n = 0;
    std::cin >> n;

    SplayTree tree;
    bool b = true;
    size_t last_answer = 0;

    while(n-- > 0) {
        char query = '#';
        std::cin >> query;
        if (query == '+') {
            size_t value = 0;
            std::cin >> value;
            if (b) {
                tree.Insert(value);
            } else {
                tree.Insert((value + last_answer) % Mod);
                b = true;
            }
        } else if (query == '?') {
            size_t left = 0, right = 0;
            std::cin >> left >> right;
            last_answer = tree.GetSum(left, right);
            b = false; // НЕ ЗАБЫТЬ ПОМЕНЯТЬ НА FALSE!!!!
            std::cout << last_answer << '\n';
        }
    }

    return 0;
}
