/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/400596/problem/I
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#define uint32_t uint64_t

class SuffixAutomaton {
private:
    uint32_t root = 0;
    uint32_t last_state = 0;
    struct State {
        uint32_t link = 1;
        uint32_t longest_length = 0;
        uint32_t nearest_term_dist = 1e9, further_term_dist = 0;
        bool is_term = false;
        std::map<char, uint32_t> to;
    };
    std::vector<State> automaton;

    void build();

    uint32_t new_state();

    uint32_t clone(uint32_t q);

    void add_char(char c);

    void update_terminal();

    void dfs(uint32_t v, uint64_t &ans, std::vector<bool> &used);

public:
    SuffixAutomaton() {
        build();
    }

    void add(const std::string &word);

    uint64_t get_ans();
};

void SuffixAutomaton::build() {
    automaton.resize(2);
    root = 1;
    last_state = 1;
    automaton[root].link = 0;
}

uint32_t SuffixAutomaton::clone(uint32_t q) {
    uint32_t new_state_id = new_state();
    automaton[new_state_id] = automaton[q];
    return new_state_id;
}

uint32_t SuffixAutomaton::new_state() {
    automaton.push_back(State());
    return automaton.size() - 1;
}

void SuffixAutomaton::add_char(char c) {
    uint32_t curr = new_state();
    uint32_t p = last_state;
    automaton[curr].longest_length = automaton[p].longest_length + 1;
    while (p >= root &&
           automaton[p].to.find(c) == automaton[p].to.end()) {
        automaton[p].to[c] = curr;
        p = automaton[p].link;
    }
    if (p != 0) {
        uint32_t q = automaton[p].to[c];
        if (automaton[p].longest_length + 1 ==
            automaton[q].longest_length) {
            automaton[curr].link = q;
        } else {
            uint32_t new_state = clone(q);
            automaton[new_state].longest_length =
                    automaton[p].longest_length + 1;
            automaton[q].link = automaton[curr].link = new_state;
            while (p >= root && automaton[p].to[c] == q) {
                automaton[p].to[c] = new_state;
                p = automaton[p].link;
            }
        }
    }
    last_state = curr;
}

void SuffixAutomaton::add(const std::string &word) {
    for (char c: word) {
        add_char(c);
    }
    update_terminal();
}

void SuffixAutomaton::update_terminal() {
    uint32_t p = last_state;
    while (p >= root) {
        automaton[p].is_term = true;
        p = automaton[p].link;
    }
}

void SuffixAutomaton::dfs(uint32_t v, uint64_t &ans, std::vector<bool> &used) {
    if (automaton[v].is_term) {
        automaton[v].nearest_term_dist = 0;
        if (automaton[v].to.empty()) {
            automaton[v].further_term_dist = 0;
        }
    }
    used[v] = true;
    for (auto el: automaton[v].to) {
        uint32_t u = el.second;
        if (!used[u]) {
            dfs(u, ans, used);
        }
        if (automaton[u].further_term_dist + 1 >
            automaton[v].further_term_dist) {
            automaton[v].further_term_dist = automaton[u].further_term_dist + 1;
        }
        if (automaton[u].nearest_term_dist + 1 <
            automaton[v].nearest_term_dist) {
            automaton[v].nearest_term_dist = automaton[u].nearest_term_dist + 1;
        }
    }
    uint32_t length = automaton[v].further_term_dist -
                      automaton[v].nearest_term_dist + automaton[v].longest_length;
    uint64_t value = length + uint64_t(automaton[v].longest_length) * automaton[v].longest_length;
    if (automaton[v].further_term_dist != automaton[v].nearest_term_dist) {
        /* std::cout << "further_dist = " << automaton[v].further_term_dist
                   << ", nearest_dist = " << automaton[v].nearest_term_dist
                   << ", cur_length = " << automaton[v].longest_length
                   << ", length = " << length
                   << ", value = " << value << '\n';*/
        if (ans < value) {
            ans = value;
        }
    }
}

uint64_t SuffixAutomaton::get_ans() {
    uint64_t value = 0;
    std::vector<bool> used(automaton.size(), false);
    dfs(root, value, used);
    return value;
}

void solve() {
    SuffixAutomaton suf_auto;
    std::string s;
    std::cin >> s;
    suf_auto.add(s);
    std::cout << suf_auto.get_ans() << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    solve();

    return 0;
}