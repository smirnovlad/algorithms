/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/377095/problem/I
 */

#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>

const uint32_t INF = 1e9;

std::unordered_map<std::string, uint32_t> word_to_id;
std::vector<std::string> dict;

std::string t = "";

uint32_t h(uint32_t v) {
    uint32_t ans = 0;
    std::string word = dict[v];
    for (uint32_t k = 0; k < word.size(); ++k) {
        if (word[k] != t[k]) {
            ++ans;
        }
    }
    return ans;
}

void solve() {
    uint32_t n = 0;
    std::cin >> n;

    for (uint32_t i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        dict.resize(n);
        dict[i] = word;
        word_to_id[word] = i;
    }
    std::vector<std::vector<uint32_t>> gr(n);
    for (const auto& [word, id]: word_to_id) {
        std::string to = word;
        for (uint32_t k = 0; k < word.size(); ++k) {
            to = word;
            for (uint32_t i = 0; i < 26; ++i) {
                to[k] = char(int('a') + i);
                if (to != word && word_to_id.find(to) != word_to_id.end()) {
                    gr[id].push_back(word_to_id[to]);
                }
            }
        }
    }
    uint32_t m = 0;
    std::cin >> m;
    while (m-- > 0) {
        std::string s;
        std::cin >> s >> t;
        uint32_t s_id = word_to_id[s], t_id = word_to_id[t];
        //std::cout << "s_id = " << s_id << ", t_id = " << t_id << '\n';
        std::vector<uint32_t> g(n, INF), f(n);
        std::set<std::pair<uint32_t, uint32_t>> q;
        g[s_id] = 0;
        f[s_id] = g[s_id] + h(s_id);
        q.insert({f[s_id], s_id});
        while (!q.empty()) {
            uint32_t v = q.begin()->second;
            q.erase(q.begin());
            if (v == t_id) {
                break;
            } else {
                for (uint32_t to: gr[v]) {
                    uint32_t x = g[v] + 1;
                    if (g[to] > x) {
                        auto it = q.find({g[to], to});
                        if (it != q.end()) {
                            q.erase(it);
                        }
                        g[to] = x;
                        f[to] = g[to] + h(to);
                        q.insert({f[to], to});
                        //std::cout << "to = " << to << ", g = " << g[to] << ", f = " << f[to] << '\n';
                    }
                }
            }
        }
        std::cout << f[t_id] << '\n';
    }
}

int main() {
    solve();

    return 0;
}