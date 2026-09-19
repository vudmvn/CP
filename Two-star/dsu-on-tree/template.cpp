/**
 * @file template.cpp
 * @brief Production-Ready DSU on Tree (Sack / Arpa's Trick) Frameworks in C++20
 * @author Duc-Minh Vu
 * @institution SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), National Economics University (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Standard C++20 implementations:
 * 1. DSUOnTree_SubtreeMode: Standard Sack engine for Subtree Frequency & Mode queries (CF 600E pattern).
 * 2. DSUOnTree_PathPairing: Query-Before-Add pattern for counting pairs at distance K in subtrees (CF 161D pattern).
 * 3. DSUOnTree_PalindromeXOR: Arpa's trick for longest palindrome-forming path in subtrees (CF 741D pattern).
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <climits>

namespace cp::dsu_on_tree {

// =========================================================================
// 1. DSU on Tree for Subtree Queries (Euler Tour Flat Scan, Mode & Sum)
// =========================================================================
class SubtreeModeSack {
private:
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> color;
    std::vector<int> sz, heavy;
    std::vector<int> tin, tout, euler_node;
    int timer = 0;

    // Dữ liệu duy trì trong mảng tần số toàn cục
    std::vector<int> cnt;
    int max_freq = 0;
    long long sum_dominant_colors = 0;
    int distinct_colors_cnt = 0;

    // Kết quả lưu cho từng đỉnh
    std::vector<long long> ans_dominant_sum;
    std::vector<int> ans_distinct;

    void dfs_size(int u, int p) {
        sz[u] = 1;
        heavy[u] = -1;
        tin[u] = ++timer;
        euler_node[timer] = u;

        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_size(v, u);
            sz[u] += sz[v];
            if (heavy[u] == -1 || sz[v] > sz[heavy[u]]) {
                heavy[u] = v;
            }
        }
        tout[u] = timer;
    }

    void add(int u) {
        int c = color[u];
        if (cnt[c] == 0) ++distinct_colors_cnt;
        ++cnt[c];

        if (cnt[c] > max_freq) {
            max_freq = cnt[c];
            sum_dominant_colors = c;
        } else if (cnt[c] == max_freq) {
            sum_dominant_colors += c;
        }
    }

    void remove(int u) {
        int c = color[u];
        --cnt[c];
        if (cnt[c] == 0) --distinct_colors_cnt;
    }

    void dfs_sack(int u, int p, bool keep) {
        // 1. Duyệt các con nhẹ với keep = false (dọn sạch sau khi duyệt)
        for (int v : adj[u]) {
            if (v == p || v == heavy[u]) continue;
            dfs_sack(v, u, false);
        }

        // 2. Duyệt con nặng với keep = true (giữ nguyên dữ liệu trong cnt)
        if (heavy[u] != -1) {
            dfs_sack(heavy[u], u, true);
        }

        // 3. Nạp lại các con nhẹ và chính đỉnh u vào mảng tần số
        for (int v : adj[u]) {
            if (v == p || v == heavy[u]) continue;
            for (int t = tin[v]; t <= tout[v]; ++t) {
                add(euler_node[t]);
            }
        }
        add(u);

        // 4. Trả lời truy vấn cây con đỉnh u
        ans_dominant_sum[u] = sum_dominant_colors;
        ans_distinct[u] = distinct_colors_cnt;

        // 5. Nếu keep = false, xóa sạch cây con u khỏi mảng tần số
        if (!keep) {
            for (int t = tin[u]; t <= tout[u]; ++t) {
                remove(euler_node[t]);
            }
            max_freq = 0;
            sum_dominant_colors = 0;
        }
    }

public:
    explicit SubtreeModeSack(int size, const std::vector<int>& col)
        : n(size), adj(size + 1), color(size + 1),
          sz(size + 1, 0), heavy(size + 1, -1),
          tin(size + 1, 0), tout(size + 1, 0), euler_node(size + 1, 0),
          ans_dominant_sum(size + 1, 0), ans_distinct(size + 1, 0) {
        int max_c = 0;
        for (int i = 1; i <= size; ++i) {
            color[i] = col[i - 1];
            max_c = std::max(max_c, color[i]);
        }
        cnt.assign(max_c + 2, 0);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void run(int root = 1) {
        timer = 0;
        dfs_size(root, 0);
        dfs_sack(root, 0, true);
    }

    const std::vector<long long>& get_dominant_sums() const {
        return ans_dominant_sum;
    }

    const std::vector<int>& get_distinct_counts() const {
        return ans_distinct;
    }
};

// =========================================================================
// 2. DSU on Tree for Path Queries (Query-Before-Add Pattern, Distance K)
// =========================================================================
class PathDistancePairingSack {
private:
    int n, target_dist;
    std::vector<std::vector<int>> adj;
    std::vector<int> sz, heavy, depth;
    std::vector<int> tin, tout, euler_node;
    int timer = 0;

    std::vector<int> depth_cnt;
    long long total_pairs = 0;

    void dfs_size(int u, int p, int d) {
        sz[u] = 1;
        heavy[u] = -1;
        depth[u] = d;
        tin[u] = ++timer;
        euler_node[timer] = u;

        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_size(v, u, d + 1);
            sz[u] += sz[v];
            if (heavy[u] == -1 || sz[v] > sz[heavy[u]]) {
                heavy[u] = v;
            }
        }
        tout[u] = timer;
    }

    void dfs_sack(int u, int p, bool keep) {
        // 1. Duyệt các con nhẹ
        for (int v : adj[u]) {
            if (v == p || v == heavy[u]) continue;
            dfs_sack(v, u, false);
        }

        // 2. Duyệt con nặng
        if (heavy[u] != -1) {
            dfs_sack(heavy[u], u, true);
        }

        // 3. Kỹ thuật Query-Before-Add:
        // Đếm các đường đi kết thúc tại đỉnh u
        int target_d = depth[u] + target_dist;
        if (target_d < (int)depth_cnt.size()) {
            total_pairs += depth_cnt[target_d];
        }

        // Với mỗi con nhẹ: HỎI trước, THÊM sau!
        for (int v : adj[u]) {
            if (v == p || v == heavy[u]) continue;
            // Bước hỏi: ghép x in Subtree(v) với các đỉnh y đã có
            for (int t = tin[v]; t <= tout[v]; ++t) {
                int x = euler_node[t];
                int needed_depth = 2 * depth[u] + target_dist - depth[x];
                if (needed_depth >= 0 && needed_depth < (int)depth_cnt.size()) {
                    total_pairs += depth_cnt[needed_depth];
                }
            }
            // Bước thêm: nạp Subtree(v) vào depth_cnt
            for (int t = tin[v]; t <= tout[v]; ++t) {
                int x = euler_node[t];
                ++depth_cnt[depth[x]];
            }
        }
        // Thêm chính đỉnh u
        ++depth_cnt[depth[u]];

        // 4. Nếu keep = false, xóa Subtree(u)
        if (!keep) {
            for (int t = tin[u]; t <= tout[u]; ++t) {
                int x = euler_node[t];
                --depth_cnt[depth[x]];
            }
        }
    }

public:
    PathDistancePairingSack(int size, int k)
        : n(size), target_dist(k), adj(size + 1),
          sz(size + 1, 0), heavy(size + 1, -1), depth(size + 1, 0),
          tin(size + 1, 0), tout(size + 1, 0), euler_node(size + 1, 0),
          depth_cnt(size + 2, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    long long count_pairs(int root = 1) {
        timer = 0;
        total_pairs = 0;
        dfs_size(root, 0, 0);
        dfs_sack(root, 0, true);
        return total_pairs;
    }
};

// =========================================================================
// 3. Arpa's Trick: Palindrome XOR Path in Subtrees (CF 741D Pattern)
// =========================================================================
struct Edge {
    int to;
    char ch;
};

class PalindromePathSack {
private:
    static constexpr int ALPHABET_SIZE = 22; // ký tự 'a' đến 'v'
    static constexpr int MAX_MASK = 1 << ALPHABET_SIZE;
    static constexpr int NEG_INF = -1e9;

    int n;
    std::vector<std::vector<Edge>> adj;
    std::vector<int> sz, heavy, depth, prefix_mask;
    std::vector<int> tin, tout, euler_node;
    int timer = 0;

    std::vector<int> max_depth; // max_depth[mask]
    std::vector<int> ans;

    void dfs_size(int u, int p, int d, int mask) {
        sz[u] = 1;
        heavy[u] = -1;
        depth[u] = d;
        prefix_mask[u] = mask;
        tin[u] = ++timer;
        euler_node[timer] = u;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            if (v == p) continue;
            int bit = edge.ch - 'a';
            dfs_size(v, u, d + 1, mask ^ (1 << bit));
            sz[u] += sz[v];
            if (heavy[u] == -1 || sz[v] > sz[heavy[u]]) {
                heavy[u] = v;
            }
        }
        tout[u] = timer;
    }

    void dfs_sack(int u, int p, bool keep) {
        // 1. Duyệt con nhẹ
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            if (v == p || v == heavy[u]) continue;
            dfs_sack(v, u, false);
            ans[u] = std::max(ans[u], ans[v]);
        }

        // 2. Duyệt con nặng
        if (heavy[u] != -1) {
            dfs_sack(heavy[u], u, true);
            ans[u] = std::max(ans[u], ans[heavy[u]]);
        }

        // 3. Kiểm tra đường đi qua đỉnh u
        // Cặp giữa đỉnh u và các đỉnh x trong con nặng/nhẹ
        if (max_depth[prefix_mask[u]] != NEG_INF) {
            ans[u] = std::max(ans[u], max_depth[prefix_mask[u]] - depth[u]);
        }
        for (int b = 0; b < ALPHABET_SIZE; ++b) {
            int target = prefix_mask[u] ^ (1 << b);
            if (max_depth[target] != NEG_INF) {
                ans[u] = std::max(ans[u], max_depth[target] - depth[u]);
            }
        }
        max_depth[prefix_mask[u]] = std::max(max_depth[prefix_mask[u]], depth[u]);

        // Ghép cặp giữa các con nhẹ với con nặng & con nhẹ trước đó
        for (const auto& edge : adj[u]) {
            int v = edge.to;
            if (v == p || v == heavy[u]) continue;

            for (int t = tin[v]; t <= tout[v]; ++t) {
                int x = euler_node[t];
                // Thử mask giống hệt
                if (max_depth[prefix_mask[x]] != NEG_INF) {
                    ans[u] = std::max(ans[u], depth[x] + max_depth[prefix_mask[x]] - 2 * depth[u]);
                }
                // Thử 22 mask lệch 1 bit
                for (int b = 0; b < ALPHABET_SIZE; ++b) {
                    int target = prefix_mask[x] ^ (1 << b);
                    if (max_depth[target] != NEG_INF) {
                        ans[u] = std::max(ans[u], depth[x] + max_depth[target] - 2 * depth[u]);
                    }
                }
            }

            // Nạp con nhẹ vào max_depth
            for (int t = tin[v]; t <= tout[v]; ++t) {
                int x = euler_node[t];
                max_depth[prefix_mask[x]] = std::max(max_depth[prefix_mask[x]], depth[x]);
            }
        }

        // 4. Nếu keep = false, xóa Subtree(u)
        if (!keep) {
            for (int t = tin[u]; t <= tout[u]; ++t) {
                int x = euler_node[t];
                max_depth[prefix_mask[x]] = NEG_INF;
            }
        }
    }

public:
    explicit PalindromePathSack(int size)
        : n(size), adj(size + 1),
          sz(size + 1, 0), heavy(size + 1, -1), depth(size + 1, 0), prefix_mask(size + 1, 0),
          tin(size + 1, 0), tout(size + 1, 0), euler_node(size + 1, 0),
          max_depth(MAX_MASK, NEG_INF), ans(size + 1, 0) {}

    void add_edge(int u, int v, char ch) {
        adj[u].push_back({v, ch});
        adj[v].push_back({u, ch});
    }

    std::vector<int> solve(int root = 1) {
        timer = 0;
        dfs_size(root, 0, 0, 0);
        dfs_sack(root, 0, true);
        return ans;
    }
};

} // namespace cp::dsu_on_tree

// =========================================================================
// Verification & Unit Tests
// =========================================================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Test 1: Subtree Mode & Sum (CF 600E pattern)
    // Tree:
    //       1 (color 1)
    //      / \_
    //     2   3 (color 3)
    //    / \_
    //   4   5 (color 2, color 2)
    // Colors: [1, 2, 3, 2, 2]
    {
        std::vector<int> colors = {1, 2, 3, 2, 2};
        cp::dsu_on_tree::SubtreeModeSack sack(5, colors);
        sack.add_edge(1, 2);
        sack.add_edge(1, 3);
        sack.add_edge(2, 4);
        sack.add_edge(2, 5);
        sack.run(1);

        const auto& dominant = sack.get_dominant_sums();
        const auto& distinct = sack.get_distinct_counts();

        // Node 4: dominant sum 2, distinct 1
        assert(dominant[4] == 2);
        assert(distinct[4] == 1);

        // Node 2: color 2 appears 3 times -> dominant sum 2, distinct 1
        assert(dominant[2] == 2);
        assert(distinct[2] == 1);

        // Node 1: color 2 appears 3 times, others 1 time -> dominant sum 2, distinct 3
        assert(dominant[1] == 2);
        assert(distinct[1] == 3);

        // Node 3: dominant sum 3, distinct 1
        assert(dominant[3] == 3);
        assert(distinct[3] == 1);
    }

    // Test 2: Path Distance Pairing (Count pairs at distance K=2)
    // Tree: 1 - 2 - 3 - 4 (linear path)
    // Pairs at distance 2: (1, 3) and (2, 4) -> total 2 pairs
    {
        cp::dsu_on_tree::PathDistancePairingSack path_sack(4, 2);
        path_sack.add_edge(1, 2);
        path_sack.add_edge(2, 3);
        path_sack.add_edge(3, 4);
        long long pairs = path_sack.count_pairs(1);
        assert(pairs == 2);
    }

    // Star tree: Center 1 connected to 2, 3, 4, 5
    // Distance 2 pairs: any pair of leaves: C(4, 2) = 6 pairs!
    {
        cp::dsu_on_tree::PathDistancePairingSack star_sack(5, 2);
        star_sack.add_edge(1, 2);
        star_sack.add_edge(1, 3);
        star_sack.add_edge(1, 4);
        star_sack.add_edge(1, 5);
        long long pairs = star_sack.count_pairs(1);
        assert(pairs == 6);
    }

    // Test 3: Palindrome XOR Path (CF 741D pattern)
    // Tree: 1 -('a')-> 2 -('a')-> 3
    // Path 1-3 has 'a' twice -> palindrome -> length 2!
    {
        cp::dsu_on_tree::PalindromePathSack pal_sack(3);
        pal_sack.add_edge(1, 2, 'a');
        pal_sack.add_edge(2, 3, 'a');
        auto res = pal_sack.solve(1);
        assert(res[1] == 2);
    }

    std::cout << "DSU on Tree (Sack) Templates: 100% assertions PASSED!\n";
    return 0;
}
