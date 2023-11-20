/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/400596/problem/L
 */

#include <iostream>
#include <vector>
#include <map>

uint32_t sigma = 256;

class SuffixTree {
private:
    std::string text;
    uint32_t root = 1;
    std::vector<uint32_t> suff_array, lcp;

    struct Node {
        uint32_t p = 0;
        uint32_t l = 0, r = 0; // [l, r)
        std::map<char, uint32_t> to;
    };

    std::vector<Node> tree;

    void get_suff_array();

    void get_lcp();

    void add(uint32_t i, uint32_t node_id, uint32_t to_up);

    void build();

    void dfs(uint32_t v, std::vector<bool> &used,
             std::vector<uint32_t> &id, uint32_t& cnt);

public:
    SuffixTree(const std::string &s) : text(s) {
        build();
    }

    void show();
};

void SuffixTree::get_suff_array() {
    uint32_t n = text.size();
    std::vector<uint32_t> p(n, n);
    std::vector<uint32_t> cnt(sigma, 0);
    std::vector<uint32_t> c(n, 0);
    for (uint32_t i = 0; i < n; ++i) {
        ++cnt[text[i]];
    }
    for (uint32_t i = 1; i < sigma; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int32_t i = n - 1; i >= 0; --i) {
        p[--cnt[text[i]]] = i;
    }
    c[p[0]] = 0;
    for (uint32_t i = 1; i < n; ++i) {
        c[p[i]] = c[p[i - 1]];
        if (text[p[i]] != text[p[i - 1]]) {
            ++c[p[i]];
        }
    }
    std::vector<int32_t> pn(n, n);
    std::vector<uint32_t> cn(n, n);
    cnt.resize(n);
    for (uint32_t exp_two = 1; exp_two <= n; exp_two *= 2) {
        for (uint32_t i = 0; i < n; ++i) {
            pn[i] = p[i] - exp_two;
            if (pn[i] < 0) pn[i] += n;
        }
        cnt.assign(cnt.size(), 0);
        for (uint32_t i = 0; i < n; ++i) {
            ++cnt[c[pn[i]]];
        }
        for (uint32_t i = 1; i < n; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int32_t i = n - 1; i >= 0; --i) {
            p[--cnt[c[pn[i]]]] = pn[i];
        }
        cn[p[0]] = 0;
        for (uint32_t i = 1; i < n; ++i) {
            cn[p[i]] = cn[p[i - 1]];
            if (c[p[i]] != c[p[i - 1]]) {
                ++cn[p[i]];
            } else if (c[(p[i] + exp_two) % n] != c[(p[i - 1] + exp_two) % n]) {
                ++cn[p[i]];
            }
        }
        c = cn;
    }
    suff_array = p;
}

void SuffixTree::get_lcp() {
    uint32_t n = suff_array.size();
    std::vector<uint32_t> pos(n, n);
    for (uint32_t i = 0; i < n; ++i) {
        pos[suff_array[i]] = i;
    }
    lcp.assign(n - 1, n + 1);
    uint32_t k = 0;
    for (uint32_t i = 0; i < n; ++i) {
        if (k > 0) {
            k = k - 1;
        }
        if (pos[i] == n - 1) {
            continue;
        } else {
            uint32_t j = suff_array[pos[i] + 1];
            while (text[i + k] == text[j + k]) {
                ++k;
            }
            lcp[pos[i]] = k;
        }
    }
}

void SuffixTree::add(uint32_t i, uint32_t node_id, uint32_t to_up) {
    if (to_up == 0) {
        uint32_t l = (i == 0 ? suff_array[i] : suff_array[i] + lcp[i - 1]);
        uint32_t r = text.size();
        tree.push_back({node_id, l, r});
        uint32_t added_id = tree.size() - 1;
        tree[node_id].to[text[l]] = added_id;
    } else {
        uint32_t length = tree[node_id].r - tree[node_id].l;
        if (length <= to_up) {
            add(i, tree[node_id].p, to_up - length);
        } else {
            uint32_t l1 = tree[node_id].l,
                    r2 = tree[node_id].r;
            uint32_t r1 = r2 - to_up;
            uint32_t l2 = r1;

            tree.push_back({tree[node_id].p, l1, r1});
            uint32_t added_id = tree.size() - 1;
            tree[tree[node_id].p].to[text[l1]] = added_id;
            tree[added_id].to[text[l2]] = node_id;
            tree[node_id].p = added_id;
            tree[node_id].l = l2;
            tree[node_id].r = r2;
            add(i, added_id, 0);
        }
    }
}

void SuffixTree::build() {
    get_suff_array();
    get_lcp();
    uint32_t n = text.size();
    /* for (uint32_t i = 0; i < n; ++i) {
         std::cout << lcp[i] << ": ";
         for (uint32_t j = suff_array[i]; j < n; ++j) {
             std::cout << text[j];
         }
         std::cout << '\n';
     }*/
    tree.resize(2);
    root = 1;
    tree[root] = {0, 0, 0};
    std::vector<uint32_t> lcp_node_id(n, 0);
    for (uint32_t i = 0; i < n; ++i) {
        uint32_t to_up = (i == 0 ? 0 : (n - suff_array[i - 1]) - lcp[i - 1]);
        add(i, tree.size() - 1, to_up); // (tree.size() - 1) is the last node we have added
    }
}

void SuffixTree::dfs(uint32_t v, std::vector<bool> &used,
                     std::vector<uint32_t> &id, uint32_t& cnt) {
    id[v] = cnt++;
    if (v != root) {
        std::cout << id[tree[v].p] << ' ' << tree[v].l << ' ' << tree[v].r << '\n';
    }
    used[v] = true;
    for (auto el: tree[v].to) {
        if (!used[el.second]) {
            dfs(el.second, used, id, cnt);
        }
    }
}

void SuffixTree::show() {
    std::vector<bool> used(tree.size(), false);
    std::cout << tree.size() - 1 << '\n';
    std::vector<uint32_t> id(tree.size(), 0);
    uint32_t cnt = 0;
    dfs(1, used, id, cnt);
}

void solve() {
    std::string s;
    std::cin >> s;
    SuffixTree t(s);
    t.show();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(0);
    std::cin.tie(0);
    solve();

    return 0;
}