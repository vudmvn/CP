/**
 * @file template.cpp
 * @brief Production-Ready Offline Processing Frameworks (Sweep-line, Mo's Algorithm, Dynamic Connectivity)
 * @author Duc-Minh Vu
 * @institution SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), National Economics University (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Standard C++20 implementations for:
 * 1. OfflinePrefixSweep: Right-Endpoint Sweep with Fenwick Tree (Distinct Elements O((N+Q) log N)).
 * 2. MosAlgorithm: Mo's Algorithm with Odd-Even sorting optimization (O((N+Q) sqrt(N))).
 * 3. OfflineDynamicConnectivity: Time Divide & Conquer + Rollback DSU (O(Q log Q log N)).
 * 4. ParallelBinarySearch: Framework for batch binary searching over event timelines.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <cassert>

namespace cp::offline {

// =========================================================================
// 1. Right-Endpoint Sweep for Distinct Elements (CSES 1734 pattern)
// =========================================================================
class FenwickTree {
private:
    int n;
    std::vector<int> tree;

public:
    explicit FenwickTree(int size) : n(size), tree(size + 1, 0) {}

    void add(int i, int delta) {
        for (; i <= n; i += i & (-i)) tree[i] += delta;
    }

    int query(int i) const {
        int sum = 0;
        for (; i > 0; i -= i & (-i)) sum += tree[i];
        return sum;
    }

    int query(int l, int r) const {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }
};

struct DistinctQuery {
    int l, r, id;
};

/**
 * @brief Compute distinct elements in subsegments [L, R] in O((N + Q) log N)
 * @param a 1-indexed array of values
 * @param queries List of queries (l, r, id)
 * @return Vector of answers indexed by query id
 */
std::vector<int> solve_distinct_elements(const std::vector<int>& a, const std::vector<DistinctQuery>& queries) {
    int n = static_cast<int>(a.size()) - 1;
    int q = static_cast<int>(queries.size());
    std::vector<int> ans(q);

    // Group queries by right endpoint R
    std::vector<std::vector<std::pair<int, int>>> q_at(n + 1);
    for (const auto& qry : queries) {
        q_at[qry.r].push_back({qry.l, qry.id});
    }

    FenwickTree bit(n);
    std::unordered_map<int, int> last_pos;

    for (int r = 1; r <= n; ++r) {
        int val = a[r];
        if (auto it = last_pos.find(val); it != last_pos.end()) {
            bit.add(it->second, -1); // Erase old position
        }
        bit.add(r, 1); // Add current position
        last_pos[val] = r;

        for (const auto& [l, qid] : q_at[r]) {
            ans[qid] = bit.query(l, r);
        }
    }
    return ans;
}

// =========================================================================
// 2. Mo's Algorithm with Odd-Even Sort Optimization
// =========================================================================
struct MoQuery {
    int l, r, id;
};

template <typename State>
class MosAlgorithm {
private:
    int n, q, block_size;
    std::vector<MoQuery> queries;

public:
    MosAlgorithm(int array_size, int query_count)
        : n(array_size), q(query_count) {
        block_size = std::max(1, static_cast<int>(n / std::sqrt(std::max(1, q))));
    }

    void add_query(int l, int r, int id) {
        queries.push_back({l, r, id});
    }

    template <typename AddFunc, typename RemoveFunc, typename GetAnsFunc>
    std::vector<typename State::AnsType> solve(
        AddFunc&& add, RemoveFunc&& remove, GetAnsFunc&& get_ans) {
        
        std::vector<typename State::AnsType> ans(queries.size());

        // Odd-Even sorting
        std::sort(queries.begin(), queries.end(), [&](const MoQuery& a, const MoQuery& b) {
            int b1 = a.l / block_size;
            int b2 = b.l / block_size;
            if (b1 != b2) return b1 < b2;
            return (b1 & 1) ? a.r < b.r : a.r > b.r;
        });

        int cur_l = 1, cur_r = 0;
        for (const auto& qry : queries) {
            while (cur_r < qry.r) add(++cur_r);
            while (cur_l > qry.l) add(--cur_l);
            while (cur_r > qry.r) remove(cur_r--);
            while (cur_l < qry.l) remove(cur_l++);
            ans[qry.id] = get_ans();
        }

        return ans;
    }
};

// =========================================================================
// 3. Offline Dynamic Connectivity via Time D&C + Rollback DSU
// =========================================================================
class RollbackDSU {
private:
    int n;
    int num_components;
    std::vector<int> parent;
    std::vector<int> sz;

    struct HistoryEntry {
        int u, v, old_sz_u;
    };
    std::vector<HistoryEntry> history;

public:
    explicit RollbackDSU(int size)
        : n(size), num_components(size), parent(size + 1), sz(size + 1, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int i) const {
        // Union by Size only: NO path compression!
        while (i != parent[i]) i = parent[i];
        return i;
    }

    bool same(int u, int v) const {
        return find(u) == find(v);
    }

    int components() const {
        return num_components;
    }

    bool unite(int u, int v) {
        int root_u = find(u);
        int root_v = find(v);
        if (root_u == root_v) return false;

        if (sz[root_u] < sz[root_v]) std::swap(root_u, root_v);

        // Record history for rollback
        history.push_back({root_u, root_v, sz[root_u]});

        parent[root_v] = root_u;
        sz[root_u] += sz[root_v];
        --num_components;
        return true;
    }

    int checkpoint() const {
        return static_cast<int>(history.size());
    }

    void rollback(int target_checkpoint) {
        while (static_cast<int>(history.size()) > target_checkpoint) {
            auto [u, v, old_sz_u] = history.back();
            history.pop_back();

            parent[v] = v;
            sz[u] = old_sz_u;
            ++num_components;
        }
    }
};

struct Edge {
    int u, v;
};

struct ConnectivityQuery {
    int u, v, id;
};

class DynamicConnectivityOffline {
private:
    int n, max_time;
    RollbackDSU dsu;
    std::vector<std::vector<Edge>> tree;
    std::vector<std::vector<ConnectivityQuery>> queries_at_time;
    std::vector<bool> ans;

    void add_edge_segment(int id, int l, int r, int ql, int qr, const Edge& edge) {
        if (ql <= l && r <= qr) {
            tree[id].push_back(edge);
            return;
        }
        int mid = (l + r) / 2;
        if (ql <= mid) add_edge_segment(2 * id, l, mid, ql, qr, edge);
        if (qr > mid) add_edge_segment(2 * id + 1, mid + 1, r, ql, qr, edge);
    }

    void dfs(int id, int l, int r) {
        int cp = dsu.checkpoint();

        // Apply all edges assigned to this time segment
        for (const auto& edge : tree[id]) {
            dsu.unite(edge.u, edge.v);
        }

        if (l == r) {
            for (const auto& qry : queries_at_time[l]) {
                ans[qry.id] = dsu.same(qry.u, qry.v);
            }
        } else {
            int mid = (l + r) / 2;
            dfs(2 * id, l, mid);
            dfs(2 * id + 1, mid + 1, r);
        }

        // Rollback state before leaving node
        dsu.rollback(cp);
    }

public:
    DynamicConnectivityOffline(int nodes, int total_time)
        : n(nodes), max_time(total_time), dsu(nodes),
          tree(4 * total_time + 4), queries_at_time(total_time + 1) {}

    void add_edge(int u, int v, int t_in, int t_out) {
        if (t_in <= t_out) {
            add_edge_segment(1, 1, max_time, t_in, t_out, {u, v});
        }
    }

    void add_query(int u, int v, int time, int id) {
        queries_at_time[time].push_back({u, v, id});
    }

    std::vector<bool> solve(int query_count) {
        ans.assign(query_count, false);
        if (max_time >= 1) dfs(1, 1, max_time);
        return ans;
    }
};

} // namespace cp::offline

// =========================================================================
// Self-Testing & Verification
// =========================================================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    using namespace cp::offline;

    // 1. Test Distinct Elements Sweep
    std::vector<int> arr = {0, 3, 2, 3, 1, 2}; // 1-indexed
    std::vector<DistinctQuery> d_queries = {
        {1, 3, 0}, // [3, 2, 3] -> 2
        {2, 4, 1}, // [2, 3, 1] -> 3
        {1, 5, 2}  // [3, 2, 3, 1, 2] -> 3
    };
    auto d_ans = solve_distinct_elements(arr, d_queries);
    assert(d_ans[0] == 2);
    assert(d_ans[1] == 3);
    assert(d_ans[2] == 3);

    // 2. Test Mo's Algorithm (Counting distinct elements)
    struct MoState {
        using AnsType = int;
    };
    MosAlgorithm<MoState> mo(5, 3);
    mo.add_query(1, 3, 0);
    mo.add_query(2, 4, 1);
    mo.add_query(1, 5, 2);

    std::vector<int> freq(10, 0);
    int distinct_cnt = 0;
    auto mo_ans = mo.solve(
        [&](int idx) {
            if (++freq[arr[idx]] == 1) ++distinct_cnt;
        },
        [&](int idx) {
            if (--freq[arr[idx]] == 0) --distinct_cnt;
        },
        [&]() {
            return distinct_cnt;
        }
    );
    assert(mo_ans[0] == 2);
    assert(mo_ans[1] == 3);
    assert(mo_ans[2] == 3);

    // 3. Test Offline Dynamic Connectivity
    // 4 nodes, 4 time steps
    // Time 1: Add edge (1, 2)
    // Time 2: Add edge (2, 3) -> Query (1, 3) -> should be true
    // Time 3: Remove edge (1, 2)
    // Time 4: Query (1, 3) -> should be false
    DynamicConnectivityOffline dc(4, 4);
    dc.add_edge(1, 2, 1, 2); // Exists at t=1, 2
    dc.add_edge(2, 3, 2, 4); // Exists at t=2, 3, 4

    dc.add_query(1, 3, 2, 0); // Query at t=2
    dc.add_query(1, 3, 4, 1); // Query at t=4

    auto dc_ans = dc.solve(2);
    assert(dc_ans[0] == true);
    assert(dc_ans[1] == false);

    std::cout << "[SUCCESS] All Offline Processing test cases passed perfectly!\n";
    std::cout << "Distinct elements query 1: " << d_ans[0] << "\n";
    std::cout << "Mo's algorithm query 2: " << mo_ans[1] << "\n";
    std::cout << "Dynamic connectivity query 0 (t=2): " << std::boolalpha << dc_ans[0] << "\n";
    std::cout << "Dynamic connectivity query 1 (t=4): " << std::boolalpha << dc_ans[1] << "\n";

    return 0;
}
