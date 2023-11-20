/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/400596/problem/D
 */

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

class SetOfOccurrences {
private:
    static const uint32_t ALPHABET_SIZE;

    std::string& text;
    std::vector<std::string>& dict;

    std::vector<std::vector<uint32_t>> res;

    class Trie;

    Trie* trie_ptr;

public:
    SetOfOccurrences(std::string& text,
                     std::vector<std::string>& dict);
    void get_res();
    void show();
};

class SetOfOccurrences::Trie {
public:
    struct Node {
        std::vector<uint32_t> to = std::vector<uint32_t>(ALPHABET_SIZE, 0);
        bool is_term = false;
        uint32_t link = 0;
        uint32_t compressedLink = 0;

        Node() = default;
    };

    std::vector<Node> trie = std::vector<Node>();

    std::unordered_map<uint32_t, std::vector<uint32_t>> initial_id;

    void get_link_and_go_arrays();
    void build(const std::vector<std::string>& dict);

    Trie(const std::vector<std::string>& dict) {
        build(dict);
        get_link_and_go_arrays();
    }
};

SetOfOccurrences::SetOfOccurrences(std::string& text, std::vector<std::string>& dict) :
        text(text), dict(dict), res(dict.size()), trie_ptr(new Trie(dict)) {
    get_res();
}

const uint32_t SetOfOccurrences::ALPHABET_SIZE = 26;

void SetOfOccurrences::Trie::build(const std::vector<std::string>& dict) {
    for (uint32_t id = 0; id < dict.size(); ++id) {
        const auto& str = dict[id];
        if (trie.empty()) {
            trie.resize(2);
        }
        uint32_t v = 1;
        for (uint32_t i = 0; i < str.size(); ++i) {
            if (!trie[v].to[str[i] - 'a']) {
                trie.push_back(Node());
                trie[v].to[str[i] - 'a'] = trie.size() - 1;
            }
            v = trie[v].to[str[i] - 'a'];
            /* std::cout << "v = " << v << ", str = ";
            for (uint32_t j = 0; j <= i; ++j) {
              std::cout << str[j];
            }
            std::cout << '\n'; */
        }
        trie[v].is_term = true;
        initial_id[v].push_back(id);
    }
}

void SetOfOccurrences::Trie::get_link_and_go_arrays() {
    trie[1].link = 1;
    trie[1].compressedLink = 1;
    for (uint32_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (!trie[1].to[i]) {
            trie[1].to[i] = 1; // go-array
        }
    }
    std::queue<uint32_t> q;
    q.push(1);
    while (!q.empty()) {
        uint32_t v = q.front();
        q.pop();
        for (uint32_t i = 0; i < ALPHABET_SIZE; ++i) {
            uint32_t u = trie[v].to[i];
            if (trie[u].link != 0) {
                continue;
            }
            if (v == 1) {
                trie[u].link = 1;
            } else {
                trie[u].link = trie[trie[v].link].to[i];
            }
            for (uint32_t j = 0; j < ALPHABET_SIZE; ++j) {
                if (!trie[u].to[j]) {
                    trie[u].to[j] = trie[trie[u].link].to[j];
                }
            }
            if (trie[trie[u].link].is_term) {
                trie[u].compressedLink = trie[u].link;
            } else {
                trie[u].compressedLink = trie[trie[u].link].compressedLink;
            }
            q.push(u);
        }
    }
}

void SetOfOccurrences::get_res() {
    uint32_t v = 1;
    for (uint32_t i = 0; i < text.size(); ++i) {
        uint32_t j = text[i] - 'a';
        v = trie_ptr->trie[v].to[j];
        uint32_t u = v;
        while (u > 1) {
            if (trie_ptr->trie[u].is_term) {
                for (const auto& el: trie_ptr->initial_id[u]) {
                    res[el].push_back(i - dict[el].size() + 2);
                }
            }
            u = trie_ptr->trie[u].compressedLink;
        }
    }
}

void SetOfOccurrences::show() {
    for (const auto& vec: res) {
        std::cout << vec.size() << ' ';
        for (const auto& el: vec) {
            std::cout << el << ' ';
        }
        std::cout << '\n';
    }
}

void solve() {
    std::string text;
    std::cin >> text;
    uint32_t N;
    std::cin >> N;
    std::vector<std::string> dict(N);
    for (auto& el: dict) {
        std::cin >> el;
    }
    SetOfOccurrences res(text, dict);
    res.show();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    solve();

    return 0;
}