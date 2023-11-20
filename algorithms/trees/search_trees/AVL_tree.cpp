/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/357142/problem/A
 */

#include <iostream>
#include <string>

const int INF = 1e9 + 1;

class AVLTree {
private:
    typedef int key_t; 

    struct Node {
        Node* left = nullptr;
        Node* right = nullptr;
        key_t key;
        size_t height = 1;

        explicit Node(const key_t& key) : key(key) {}
    };
    
    static void FixHeight(Node* node) {
        if (node != nullptr) {
            node->height = std::max(GetHeight(node->right), GetHeight(node->left)) + 1;
        }
    }

    static size_t GetHeight(Node* node) {
        return node ? node->height : 0;
    }

    static int GetBalance(Node* node) {
        return GetHeight(node->left) - GetHeight(node->right);
    }
    
    static Node* RotateRight(Node* previous_node) {
        Node* new_node = previous_node->left;
        if (new_node != nullptr) {
            previous_node->left = new_node->right;
            new_node->right = previous_node;
            FixHeight(previous_node);
            FixHeight(new_node);
            return new_node;
        } else {
            return previous_node;
        }
    }

    static Node* RotateLeft(Node* previous_node) {
        Node* new_node = previous_node->right;
        if (new_node != nullptr) {
            previous_node->right = new_node->left;
            new_node->left = previous_node;
            FixHeight(previous_node);
            FixHeight(new_node);
            return new_node;
        } else {
            return previous_node;
        }
    }

    static Node* Balance(Node* node) {
        if (node != nullptr) {
            FixHeight(node);
            if (GetBalance(node) == -2) {
                if (GetBalance(node->right) == 1) {
                    node->right = RotateRight(node->right);
                }
                return RotateLeft(node);
            } else if (GetBalance(node) == 2) {
                if (GetBalance(node->left) == 1) {
                    node->left = RotateLeft(node->left);
                }
                return RotateRight(node);
            }
        }
        return node;
    }

    static Node* Insert(Node* node, const key_t& key) {
        if (node == nullptr) {
            node = new Node(key);
        } else {
            if (key < node->key) {
                node->left = Insert(node->left, key);
            } else if (key > node->key) {
                node->right = Insert(node->right, key);
            }
        }
        return Balance(node);
    }

    static bool Find(Node* node, const key_t& key) {
        bool result = false;
        if (node != nullptr) {
            if (key == node->key) {
                result = true;
            } else if (key < node->key) {
                result = Find(node->left, key);
            } else if (key > node->key) {
                result = Find(node->right, key);
            }
        }
        return result;
    }

    static key_t Prev(Node* node, const key_t& key) {
        key_t result = -INF;
        if (node != nullptr) {
            if (key > node->key) {
                result = node->key;
                result = std::max(result, Prev(node->right, key));
            } else if (key <= node->key) {
                if (node->left != nullptr) {
                    result = Prev(node->left, key);
                }
            }
        }
        return result;
    }

    static key_t Next(Node* node, const key_t& key) {
        key_t result = INF;
        if (node != nullptr) {
            if (key < node->key) {
                result = node->key;
                result = std::min(result, Next(node->left, key));
            } else if (key >= node->key) {
                if (node->right != nullptr) {
                    result = Next(node->right, key);
                }
            }
        }
        return result;
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
        return Balance(node);
    }

    static Node* Erase(Node* node, const key_t& key) {
        if (node == nullptr) {
            return nullptr;
        } else {
            if (key < node->key) {
                node->left = Erase(node->left, key);
            } else if (key > node->key) {
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
                    return Balance(min_value_node);
                }
            }
            return Balance(node);
        }
    } 
 
    Node* root_ = nullptr; 

public: 
    void Insert(const key_t& key) {
        root_ = Insert(root_, key);
    }

    void Remove(const key_t& key) {
        root_ = Erase(root_, key);
    }

    bool Find(const key_t& key)const {
        return Find(root_, key);
    }

    key_t Prev(const key_t& key)const {
        return Prev(root_, key);
    }

    key_t Next(const key_t& key)const {
        return Next(root_, key);
    }
};

int main(int argc, char* argv[]) { 
    AVLTree tree;

    std::string query;

    while (std::cin >> query) {
        int value = 0;
        std::cin >> value;
        if (query == "insert") { 
            tree.Insert(value);
        } else if (query == "exists") {
            std::cout << (tree.Find(value) ? "true" : "false") << '\n';
        } else if (query == "delete") {
            tree.Remove(value);
        } else if (query == "next") {
            int result = tree.Next(value);
            std::cout << (result == INF ? "none" : std::to_string(result)) << '\n';
        } else if (query == "prev") {
            int result = tree.Prev(value);
            std::cout << (result == -INF ? "none" : std::to_string(result)) << '\n';
        }
    }

    return 0;
}

