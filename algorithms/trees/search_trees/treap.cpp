/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/357142/problem/D
 */

#include <chrono>
#include <iostream>
#include <random>
#include <fstream>

class Treap {
  private:
    struct Node {
      using Priority = uint64_t;
      
      Node* left = nullptr;
      Node* right = nullptr;

      size_t subtree_size = 1;
      uint64_t length;
      uint64_t sq_sum;
      Priority priority;

      Node(uint64_t length, const Priority& priority) 
        : length(length), priority(priority), sq_sum(length * length) {}
    };
    
    static uint64_t GetSqSum(Node* node) {
      return node == nullptr ? 0 : node->sq_sum;
    }

    static size_t GetSize(Node* node) {
      return node == nullptr ? 0 : node->subtree_size;
    }

    static void Update(Node* node) {
      if (node != nullptr) {
        node->subtree_size = GetSize(node->left) + GetSize(node->right) + 1;
        node->sq_sum = GetSqSum(node->left) + GetSqSum(node->right) + node->length * node->length;
      }
    }

    static void Add(Node* node, uint32_t pos, uint64_t val) {
      if (GetSize(node->left) + 1 == pos) {
        node->length += val;    
        Update(node);
      } else if (GetSize(node->left) >= pos) {
        Add(node->left, pos, val);
        Update(node);
      } else {
        Add(node->right, pos - GetSize(node->left) - 1, val);
        Update(node);
      }
    }

    uint64_t GetLen(Node* node, uint32_t pos) {
      if (GetSize(node->left) + 1 == pos) {
        return node->length;
      } else if (GetSize(node->left) >= pos) {
        return GetLen(node->left, pos);
      } else {
        return GetLen(node->right, pos - GetSize(node->left) - 1);
      }
    }

    static std::pair<Node*, Node*> Split(Node* node, uint32_t pos) {
      if (node == nullptr) {
        return {nullptr, nullptr};
      } else if (GetSize(node->left) + 1 >= pos) {
        auto split_result = Split(node->left, pos);
        node->left = split_result.second;
        Update(node);
        return {split_result.first, node};
      } else {
        auto split_result = Split(node->right, pos - GetSize(node->left) - 1);
        node->right = split_result.first;
        Update(node);
        return {node, split_result.second};
      }
    }

    static Node* Merge(Node* left, Node* right) {
      if (left == nullptr) {
        return right;
      } else if (right == nullptr) {
        return left;
      } else if (left->priority < right->priority) {
        left->right = Merge(left->right, right);
        Update(left);
        return left;
      } else {
        right->left = Merge(left, right->left);
        Update(right);
        return right;
      }
    }

    static void Clear(Node* node) {
      if (node != nullptr) {
        Clear(node->left);
        Clear(node->right);
        delete node;
      }
    }

    static Node* Copy(Node* node) {
      if (node == nullptr) {
        return nullptr;
      } else {
        auto new_node = new Node(node->length, node->priority);
        new_node->left = Copy(node->left);
        new_node->right = Copy(node->right);
        return new_node;
      }
    }

    mutable Node* root_ = nullptr;
    std::mt19937_64 gen_;
  public:
    Treap() : gen_(std::chrono::high_resolution_clock::now().time_since_epoch().count()) {}

    Treap(const Treap& treap) : Treap() {
      root_ = Copy(treap.root_); 
    }

    Treap& operator=(const Treap& treap) {
      if (this != &treap) {
        Clear();
        root_ = Copy(treap.root_);
      }

      return *this;
    }

    ~Treap() {
      Clear();
    }

    void Insert(uint64_t length, uint32_t pos) {
      auto split_result = Split(root_, pos);
      auto new_node = new Node(length, gen_());
      root_ = Merge(split_result.first, Merge(new_node, split_result.second));
    }

    void Erase(uint32_t pos) {
      auto split_result_1 = Split(root_, pos + 1);
      auto split_result_2 = Split(split_result_1.first, pos);

      root_ = Merge(split_result_2.first, split_result_1.second);
    }

    void Add(uint32_t pos, uint64_t val)const {
      Add(root_, pos, val);
    }

    uint64_t GetLen(uint32_t pos) {
      return GetLen(root_, pos);
    }

    uint64_t GetAns()const {
      return root_->sq_sum;
    }

    void Clear() {
      Clear(root_);
    }

    size_t Size() {
      return GetSize(root_);
    }
};

int main(int argc, char* argv[]) {
  uint32_t n = 0, p = 0, k = 0;
  std::ifstream Fin;
  Fin.open("river.in");
  Fin >> n >> p;
 
  Treap treap;
  
  for (uint32_t pos = 1; pos <= n; ++pos) {
    uint64_t value = 0;
    Fin >> value;
    treap.Insert(value, pos);
  }
  
  std::ofstream Fout;
  Fout.open("river.out");
  Fout << treap.GetAns() << '\n';

  Fin >> k;
    
  while (k-- > 0) {
    uint32_t query = 0, pos = 0;
    Fin >> query >> pos;
    uint32_t len = treap.GetLen(pos);
    uint32_t l1 = len / 2, l2 = len - l1; 
    treap.Erase(pos);
    if (query == 1) {
      if (pos > 1 && pos < n) {
        treap.Add(pos - 1, l1);
        treap.Add(pos, l2);
      } else if (pos == 1) {
        treap.Add(pos, len);
      } else if (pos == n) {
        treap.Add(pos - 1, len);
      } 
      --n;
    } else if (query == 2) {
      treap.Insert(l1, pos);
      treap.Insert(l2, pos + 1); 
      ++n;
    }
    Fout << treap.GetAns() << '\n';
  }
  Fin.close();
  Fout.close();

  return 0;
}
