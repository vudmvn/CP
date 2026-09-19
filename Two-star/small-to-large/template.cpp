/**
 * @file template.cpp
 * @brief Production-Ready Small-to-Large Merging & DSU on Tree (Sack) Frameworks in C++20
 * @author Duc-Minh Vu
 * @institution SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), National Economics University (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Standard C++20 implementations:
 * 1. SetMergingSmallToLarge: Pointer/swap-based Small-to-Large merging using std::set/std::map (O(N log^2 N)).
 * 2. DSUOnTree (Sack): Cache-friendly Heavy-Light DSU on Tree with single flat array cnt[] (O(N log N)).
 *    Supports dominant color sum (CF 600E pattern) and distinct color count.
 * 3. SegmentTreeMerge: Dynamic Segment Tree merging on subtrees (O(N log V)).
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>
#include <memory>

namespace cp::small_to_large {

// =========================================================================
// 1. Pointer / Swap-based Small-to-Large Merging with std::set
// =========================================================================
class SetMergingSmallToLarge {
private:
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> color;
    std::vector<int> distinct_colors;
    std::vector<std::unique_ptr<std::set<int>>> sets;

    void dfs(int u, int p) {
        sets[u] = std::make_unique<std::set<int>>();
        sets[u]->insert(color[u]);

        for (int v : adj[u]) {
            if (v == p) continue;
            dfs(v, u);

            // Gộp tập nhỏ vào tập lớn
            if (sets[u]->size() < sets[v]->size()) {
                std::swap(sets[u], sets[v]);
            }

            // Chuyển toàn bộ phần tử từ sets[v] sang sets[u]
            for (int x : *sets[v]) {
                sets[u]->insert(x);
            }
            sets[v]->clear(); // Giải phóng phần tử
        }

        distinct_colors[u] = (int)sets[u]->size();
    }

public:
    explicit SetMergingSmallToLarge(int size, const std::vector<int>& col)
        : n(size), adj(size + 1), color(size + 1), distinct_colors(size + 1, 0), sets(size + 1) {
        for (int i = 1; i <= size; ++i) {
            color[i] = col[i - 1];
        }
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<int> solve(int root = 1) {
        dfs(root, 0);
        return distinct_colors;
    }
};

// =========================================================================
// 2. DSU on Tree / Sack (O(N log N) time, O(N) flat space, Euler tour linear scan)
// =========================================================================
class DSUOnTree {
private:
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> color;
    std::vector<int> sz;
    std::vector<int> heavy;
    std::vector<int> tin, tout;
    std::vector<int> euler_node;
    int timer = 0;

    // Dữ liệu duy trì trong mảng tần số toàn cục
    std::vector<int> cnt;
    int max_freq = 0;
    long long sum_dominant_colors = 0;

    // Kết quả truy vấn cho từng đỉnh
    std::vector<int> ans_distinct;
    std::vector<long long> ans_dominant_sum;
    int distinct_counter = 0;

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
        if (cnt[c] == 0) ++distinct_counter;
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
        if (cnt[c] == 0) --distinct_counter;
    }

    void dfs_sack(int u, int p, bool keep) {
        // 1. Duyệt các con nhẹ với keep = false (tính xong thì dọn sạch)
        for (int v : adj[u]) {
            if (v == p || v == heavy[u]) continue;
            dfs_sack(v, u, false);
        }

        // 2. Duyệt con nặng với keep = true (giữ lại thông tin trong cnt)
        if (heavy[u] != -1) {
            dfs_sack(heavy[u], u, true);
        }

        // 3. Thêm lại các con nhẹ và chính đỉnh u vào mảng tần số
        for (int v : adj[u]) {
            if (v == p || v == heavy[u]) continue;
            for (int t = tin[v]; t <= tout[v]; ++t) {
                add(euler_node[t]);
            }
        }
        add(u);

        // 4. Trả lời truy vấn cho cây con gốc u
        ans_distinct[u] = distinct_counter;
        ans_dominant_sum[u] = sum_dominant_colors;

        // 5. Nếu keep == false, xóa sạch cây con u để trả lại trạng thái trống cho cha
        if (!keep) {
            for (int t = tin[u]; t <= tout[u]; ++t) {
                remove(euler_node[t]);
            }
            max_freq = 0;
            sum_dominant_colors = 0;
        }
    }

public:
    explicit DSUOnTree(int size, const std::vector<int>& col)
        : n(size), adj(size + 1), color(size + 1),
          sz(size + 1, 0), heavy(size + 1, -1),
          tin(size + 1, 0), tout(size + 1, 0), euler_node(size + 1, 0),
          ans_distinct(size + 1, 0), ans_dominant_sum(size + 1, 0) {
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

    const std::vector<int>& get_distinct_colors() const {
        return ans_distinct;
    }

    const std::vector<long long>& get_dominant_sums() const {
        return ans_dominant_sum;
    }
};

// =========================================================================
// 3. Dynamic Segment Tree Merging on Subtrees (O(N log V))
// =========================================================================
class SegmentTreeMerge {
public:
    struct Node {
        int lc = 0, rc = 0;
        int count = 0;
    };

private:
    int n, max_val;
    std::vector<Node> tree;
    std::vector<int> roots;
    int node_count = 0;

    int new_node() {
        tree.push_back(Node{});
        return ++node_count;
    }

    void update(int& u, int l, int r, int pos, int val) {
        if (!u) u = new_node();
        tree[u].count += val;
        if (l == r) return;
        int mid = l + (r - l) / 2;
        if (pos <= mid) {
            int lc = tree[u].lc;
            update(lc, l, mid, pos, val);
            tree[u].lc = lc;
        } else {
            int rc = tree[u].rc;
            update(rc, mid + 1, r, pos, val);
            tree[u].rc = rc;
        }
    }

    int merge(int u, int v, int l, int r) {
        if (!u || !v) return u ? u : v;
        if (l == r) {
            tree[u].count += tree[v].count;
            return u;
        }
        int mid = l + (r - l) / 2;
        tree[u].lc = merge(tree[u].lc, tree[v].lc, l, mid);
        tree[u].rc = merge(tree[u].rc, tree[v].rc, mid + 1, r);
        tree[u].count = (tree[u].lc ? tree[tree[u].lc].count : 0) +
                        (tree[u].rc ? tree[tree[u].rc].count : 0);
        return u;
    }

    int query_kth(int u, int l, int r, int k) const {
        if (l == r) return l;
        int mid = l + (r - l) / 2;
        int left_cnt = tree[u].lc ? tree[tree[u].lc].count : 0;
        if (k <= left_cnt) return query_kth(tree[u].lc, l, mid, k);
        return query_kth(tree[u].rc, mid + 1, r, k - left_cnt);
    }

public:
    explicit SegmentTreeMerge(int num_nodes, int max_v)
        : n(num_nodes), max_val(max_v), roots(num_nodes + 1, 0) {
        tree.reserve(100000);
        tree.push_back(Node{}); // 1-indexed node 0 is dummy
    }

    void insert_value(int node_id, int val) {
        int root = roots[node_id];
        update(root, 1, max_val, val, 1);
        roots[node_id] = root;
    }

    void merge_trees(int dest_node, int src_node) {
        roots[dest_node] = merge(roots[dest_node], roots[src_node], 1, max_val);
    }

    int get_kth(int node_id, int k) const {
        if (!roots[node_id] || tree[roots[node_id]].count < k) return -1;
        return query_kth(roots[node_id], 1, max_val, k);
    }

    int get_total(int node_id) const {
        if (!roots[node_id]) return 0;
        return tree[roots[node_id]].count;
    }
};

} // namespace cp::small_to_large

// =========================================================================
// Verification & Unit Tests
// =========================================================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Test 1: SetMergingSmallToLarge on a tree
    // Tree:
    //       1 (color 1)
    //      / \_
    //     2   3 (color 2)
    //    /
    //   4 (color 1)
    // Color array: [1, 2, 2, 1] for nodes 1..4
    {
        std::vector<int> col = {1, 2, 2, 1};
        cp::small_to_large::SetMergingSmallToLarge solver(4, col);
        solver.add_edge(1, 2);
        solver.add_edge(1, 3);
        solver.add_edge(2, 4);

        auto distinct = solver.solve(1);
        assert(distinct[4] == 1); // only color 1
        assert(distinct[2] == 2); // colors {1, 2}
        assert(distinct[3] == 1); // only color 2
        assert(distinct[1] == 2); // colors {1, 2}
    }

    // Test 2: DSU on Tree (Sack) - CF 600E pattern
    // Tree:
    //       1 (c=1)
    //      / \_
    //     2   3 (c=3)
    //    / \_
    //   4   5 (c=2, c=2)
    // Colors: [1, 2, 3, 2, 2]
    {
        std::vector<int> col = {1, 2, 3, 2, 2};
        cp::small_to_large::DSUOnTree sack(5, col);
        sack.add_edge(1, 2);
        sack.add_edge(1, 3);
        sack.add_edge(2, 4);
        sack.add_edge(2, 5);
        sack.run(1);

        const auto& distinct = sack.get_distinct_colors();
        assert(distinct[4] == 1); // {2}
        assert(distinct[5] == 1); // {2}
        assert(distinct[2] == 1); // {2} has 3 nodes with color 2
        assert(distinct[3] == 1); // {3}
        assert(distinct[1] == 3); // {1, 2, 3}

        const auto& dominant = sack.get_dominant_sums();
        // At node 2: color 2 appears 3 times (nodes 2, 4, 5) -> dominant sum is 2
        assert(dominant[2] == 2);
        // At node 1: color 2 appears 3 times, color 1 appears 1 time, color 3 appears 1 time
        // Dominant frequency is 3 (color 2), dominant sum is 2
        assert(dominant[1] == 2);
        // At node 3: dominant is 3
        assert(dominant[3] == 3);
    }

    // Test 3: SegmentTreeMerge
    {
        cp::small_to_large::SegmentTreeMerge stm(3, 100);
        stm.insert_value(1, 10);
        stm.insert_value(1, 30);
        stm.insert_value(2, 20);
        stm.insert_value(2, 40);

        assert(stm.get_total(1) == 2);
        assert(stm.get_total(2) == 2);

        // Merge tree 2 into tree 1 -> tree 1 has values {10, 20, 30, 40}
        stm.merge_trees(1, 2);
        assert(stm.get_total(1) == 4);
        assert(stm.get_kth(1, 1) == 10);
        assert(stm.get_kth(1, 2) == 20);
        assert(stm.get_kth(1, 3) == 30);
        assert(stm.get_kth(1, 4) == 40);
        assert(stm.get_kth(1, 5) == -1); // out of bounds
    }

    std::cout << "Small-to-Large & DSU on Tree Templates: 100% assertions PASSED!\n";
    return 0;
}
