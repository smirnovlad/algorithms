/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/357142/problem/C
 */

#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <set>


class Node {
private:
    bool is_leaf_;
    uint32_t t_;
    size_t* keys_;
    uint32_t number_of_keys_;
    uint32_t sum_number_of_keys_;
    Node** children_;

public:
    Node(bool is_leaf, uint32_t t) : is_leaf_(is_leaf), t_(t), number_of_keys_(0), sum_number_of_keys_(0) {
        keys_ = new size_t[2 * t_];
        children_ = new Node*[2 * t_ + 1];
        for (uint32_t i = 1; i < 2 * t_ + 1; ++i) {
            children_[i] = nullptr;
        }
    }
    
    void traverse();
    
    Node* find(size_t key);
    
    uint32_t findKey(size_t key);

    void insertNonFull(size_t key);
    
    void splitChild(uint32_t index, Node* node);

    void remove(size_t key);

    void removeFromLeaf(uint32_t index);

    void removeFromNonLeaf(uint32_t index);

    size_t getPredecessor(uint32_t index);

    size_t getSuccessor(uint32_t index);

    void fill(uint32_t index);

    void borrowFromPrevious(uint32_t index);

    void borrowFromNext(uint32_t index);

    void merge(uint32_t index);

    size_t getKTH(uint32_t index);

friend class BTree;
};

class BTree {
private:
    uint32_t t_;
    Node* root_ = nullptr;

public:
    BTree(uint32_t t) : t_(t) {}
    
    void traverse() {
        if (root_ != nullptr) { root_->traverse(); }
    }

    Node* find(size_t key) {
        return (root_ == nullptr) ? nullptr : root_->find(key);
    }

    void insert(size_t key);

    void remove(size_t key);
    
    size_t getKTH(uint32_t index) { 
        return root_->getKTH(index); 
    };

};

void Node::traverse() {
    uint32_t current_pos = 1;
    while (current_pos <= number_of_keys_) {
        if (!is_leaf_) {
            children_[current_pos]->traverse();
        }
        std::cout << " " << keys_[current_pos];
        ++current_pos;
    }

    if (!is_leaf_) {
        children_[current_pos]->traverse();
    }
}

Node* Node::find(size_t key) {
    uint32_t current_pos = 1;
    while (current_pos <= number_of_keys_ && key > keys_[current_pos]) {
        ++current_pos;
    }
    if (keys_[current_pos] == key) {
        return this;
    }
    if (is_leaf_) {
        return nullptr;
    }
    return children_[current_pos]->find(key);
}

uint32_t Node::findKey(size_t key) {
    uint32_t index = 1;
    while (index <= number_of_keys_ && key > keys_[index]) {
        ++index;
    }
    return index;
}

void Node::remove(size_t key) {
    uint32_t index = findKey(key);
    // Ключ в текущей вершине
    if (index <= number_of_keys_ && keys_[index] == key) {
        if (is_leaf_) {
            removeFromLeaf(index);
        } else {
            removeFromNonLeaf(index);
        }
    } else {
        if (is_leaf_) {
            std::cout << "The key " << key << " is does not exist in the tree\n";
            return;
        }
        
        bool flag = (index == number_of_keys_ + 1);

        if (children_[index]->number_of_keys_ < t_) {
            fill(index);
        }
        // number_of_keys_ могло измениться после выполнения процедуры fill
        if (flag && index > number_of_keys_ + 1) {
            --sum_number_of_keys_;
            children_[index - 1]->remove(key);
        } else {
            --sum_number_of_keys_;
            children_[index]->remove(key);
        }
    }
    return;
}

void Node::removeFromLeaf(uint32_t index) {
    for (uint32_t i = index + 1; i <= number_of_keys_; ++i) {
        keys_[i - 1] = keys_[i];
    }
    --number_of_keys_;
    --sum_number_of_keys_;
    return;
}

void Node::removeFromNonLeaf(uint32_t index) {
    size_t key = keys_[index];

    if (children_[index]->number_of_keys_ >= t_) {
        size_t predecessor = getPredecessor(index);
        keys_[index] = predecessor;
        --sum_number_of_keys_;
        children_[index]->remove(predecessor);
    } else if (children_[index + 1]->number_of_keys_ >= t_) {
        size_t successor = getSuccessor(index);
        keys_[index] = successor;
        --sum_number_of_keys_;
        children_[index + 1]->remove(successor);
    } else {
        merge(index);
        --sum_number_of_keys_;
        children_[index]->remove(key);
    }
    return;
}

size_t Node::getPredecessor(uint32_t index) {
    Node* current_node = children_[index];
    while (!current_node->is_leaf_) {
        current_node = current_node->children_[current_node->number_of_keys_ + 1];
    }
    return current_node->keys_[current_node->number_of_keys_];
}

size_t Node::getSuccessor(uint32_t index) {
    Node* current_node = children_[index + 1];
    while(!current_node->is_leaf_) {
        current_node = current_node->children_[1];
    }
    return current_node->keys_[1];
}

void Node::fill(uint32_t index) {
    if (index != 1 && children_[index - 1]->number_of_keys_ >= t_) {
        borrowFromPrevious(index);
    } else if (index != number_of_keys_ + 1 && children_[index + 1]->number_of_keys_ >= t_) {
        borrowFromNext(index);
    } else {
        if (index != number_of_keys_ + 1) {
            merge(index);
        } else {
            merge(index - 1);
        } 
    }
    return;
}

void Node::borrowFromPrevious(uint32_t index) {
    Node* child = children_[index];
    Node* child_brother = children_[index - 1];

    for (uint32_t i = child->number_of_keys_; i >= 1; --i) {
        child->keys_[i + 1] = child->keys_[i];
    }

    if (!child->is_leaf_) {
        for (uint32_t i = child->number_of_keys_ + 1; i >= 1; --i) {
            child->children_[i + 1] = child->children_[i];
        }
    }

    if (!child->is_leaf_) {
        child->children_[1] = child_brother->children_[child_brother->number_of_keys_ + 1];
        child->sum_number_of_keys_ += child_brother->children_[child_brother->number_of_keys_ + 1]->sum_number_of_keys_;
        child_brother->sum_number_of_keys_ -= child_brother->children_[child_brother->number_of_keys_ + 1]->sum_number_of_keys_;
    }


    child->keys_[1] = keys_[index - 1];

    keys_[index - 1] = child_brother->keys_[child_brother->number_of_keys_];

    ++child->number_of_keys_;
    --child_brother->number_of_keys_;

    ++child->sum_number_of_keys_;
    --child_brother->sum_number_of_keys_;

    return;
}

void Node::borrowFromNext(uint32_t index) {
    Node* child = children_[index];
    Node* child_brother = children_[index + 1];

    child->keys_[child->number_of_keys_ + 1] = keys_[index];

    if (!child->is_leaf_) {
        child->children_[child->number_of_keys_ + 2] = child_brother->children_[1];
        child->sum_number_of_keys_ += child_brother->children_[1]->sum_number_of_keys_;
        child_brother->sum_number_of_keys_ -= child_brother->children_[1]->sum_number_of_keys_;
    }

    keys_[index] = child_brother->keys_[1];

    for (uint32_t i = 2; i <= child_brother->number_of_keys_; ++i) {
        child_brother->keys_[i - 1] = child_brother->keys_[i];
    }

    if (!child_brother->is_leaf_) {
        for (uint32_t i = 2; i <= child_brother->number_of_keys_ + 1; ++i) {
            child_brother->children_[i - 1] = child_brother->children_[i];
        }
    }

    ++child->number_of_keys_;
    --child_brother->number_of_keys_;

    ++child->sum_number_of_keys_;
    --child_brother->sum_number_of_keys_;

    return;
}

void Node::merge(uint32_t index) {
    Node* child = children_[index];
    Node* child_brother = children_[index + 1];

    child->keys_[t_] = keys_[index];
    
    ++child->number_of_keys_;
    ++child->sum_number_of_keys_;

    for (uint32_t i = 1; i <= child_brother->number_of_keys_; ++i) {
        child->keys_[t_ + i] = child_brother->keys_[i];
    }

    if (!child->is_leaf_) {
        for (uint32_t i = 1; i <= child_brother->number_of_keys_ + 1; ++ i) {
            child->children_[t_ + i] = child_brother->children_[i];
        }
    }

    for (uint32_t i = index + 1; i <= number_of_keys_; ++i) {
        keys_[i - 1] = keys_[i];
    }

    for (uint32_t i = index + 2; i <= number_of_keys_ + 1; ++i) {
        children_[i - 1] = children_[i];
    }

    child->number_of_keys_ += child_brother->number_of_keys_;
    child->sum_number_of_keys_ += child_brother->sum_number_of_keys_;
    
    --number_of_keys_;

    delete child_brother;
    return;
}

void BTree::insert(size_t key) {
    if (root_ == nullptr) {
        root_ = new Node(true, t_);
        root_->keys_[1] = key;
        root_->number_of_keys_ = 1;
        root_->sum_number_of_keys_ = 1;
    } else {
        if (root_->number_of_keys_ == 2 * t_ - 1) {
            Node* new_root = new Node(false, t_);
            new_root->children_[1] = root_;
            new_root->sum_number_of_keys_ = root_->sum_number_of_keys_;
            new_root->splitChild(1, root_);
            uint32_t current_pos = 1;
            if (new_root->keys_[1] < key) {
                ++current_pos;
            }
            ++new_root->sum_number_of_keys_;
            new_root->children_[current_pos]->insertNonFull(key);
            root_ = new_root;
        } else {
            root_->insertNonFull(key);
        }
    }
}

void Node::insertNonFull(size_t key) {
    uint32_t current_pos = number_of_keys_;
    ++sum_number_of_keys_;
    if (is_leaf_) {
        while (current_pos >= 1 && key < keys_[current_pos]) {
            keys_[current_pos + 1] = keys_[current_pos];
            --current_pos;
        }
        keys_[current_pos + 1] = key;
        ++number_of_keys_;
    } else { 
        while (current_pos >= 1 && key < keys_[current_pos]) {
            --current_pos;
        }
        ++current_pos;
        if (children_[current_pos]->number_of_keys_ == 2 * t_ - 1) {
            splitChild(current_pos, children_[current_pos]);
            if (keys_[current_pos] < key) {
                ++current_pos;
            }
        }
        children_[current_pos]->insertNonFull(key);
    }
}

void Node::splitChild(uint32_t index, Node* node) {
   Node* right_son = new Node(node->is_leaf_, node->t_);
   right_son->number_of_keys_ = t_ - 1;
   
   for (uint32_t i = 1; i <= t_ - 1; ++i) {
       right_son->keys_[i] = node->keys_[t_ + i];
       ++right_son->sum_number_of_keys_;
       --node->sum_number_of_keys_;
   }

   if (!node->is_leaf_) {
       for (uint32_t i = 1; i <= t_; ++i) {
           right_son->children_[i] = node->children_[t_ + i];
           right_son->sum_number_of_keys_ += right_son->children_[i]->sum_number_of_keys_;
           node->sum_number_of_keys_ -= node->children_[t_ + i]->sum_number_of_keys_;
       }
   }
    
   node->number_of_keys_ = t_ - 1;

   for (uint32_t i = number_of_keys_ + 1; i >= index + 1; --i) {
       children_[i + 1] = children_[i];
   }
   children_[index + 1] = right_son;

   for (uint32_t i = number_of_keys_; i >= index; --i) {
       keys_[i + 1] = keys_[i];
   }
   keys_[index] = node->keys_[t_];
   --node->sum_number_of_keys_;

   ++number_of_keys_;
}

void BTree::remove(size_t key) {
    if (!root_) {
        std::cout << "The tree is empty\n";
        return;
    } else {
        root_->remove(key);
        if (root_->number_of_keys_ == 0) {
            Node* temp_node = root_;
            if (root_->is_leaf_) {
                root_ = nullptr;
            } else {
                root_ = root_->children_[1];
            }
            delete temp_node;
        }
        return;
    }
}

size_t Node::getKTH(uint32_t index) {
    uint32_t current_pos = 1;
    children_[1] = children_[1];
    while (index >= 1 && (children_[current_pos] == nullptr || index >= children_[current_pos]->sum_number_of_keys_ + 1)) {
        --index;
        if (children_[current_pos] != nullptr) {
            index -= children_[current_pos]->sum_number_of_keys_;
        }
        ++current_pos;
    }
    if (index == 0) {
        return keys_[current_pos - 1];
    } else {
        return children_[current_pos]->getKTH(index);
    }
}

int main() {
    BTree Tree(3);
    
    uint32_t n = 0;
    std::cin >> n;
    uint32_t count = 0; 
    while(n-- > 0) {
        char ch = '#';
        std::cin >> ch;
        if (ch == '+' || ch == '1') {
            if (ch == '+') {
                std::cin >> ch;
            }
            size_t value = 0;
            std::cin >> value;
            Tree.insert(value);
            ++count;
        } else if (ch == '0') {
            uint32_t index = 0;
            std::cin >> index;
            std::cout << Tree.getKTH(count - index + 1) << '\n';
        } else if (ch == '-') {
            std::cin >> ch;
            size_t value = 0;
            std::cin >> value;
            Tree.remove(value);
            --count;
        }
    } 
  
    return 0;
}
