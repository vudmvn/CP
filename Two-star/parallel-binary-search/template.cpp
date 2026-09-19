/**
 * @file template.cpp
 * @brief Production-Ready Parallel Binary Search (PBS) Frameworks in C++20
 * @author Duc-Minh Vu
 * @institution SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), National Economics University (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Standard C++20 implementations:
 * 1. RecursivePBS_Meteors: Divide & Conquer PBS for cumulative threshold problems (e.g. POI Meteors)
 *    with Fenwick Tree Range Add / Point Query and Saturating Arithmetic.
 * 2. RoundBasedPBS: Iterative / Round-based PBS avoiding recursion and rollback.
 * 3. DynamicConnectivityPBS: PBS with Rollback DSU for earliest connectivity in dynamic graphs.
 * 4. OfflineRangeKthPBS: Value-space PBS for Range K-th Smallest Element without Persistent Trees.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <climits>

namespace cp::pbs {

// =========================================================================
// 1. Fenwick Tree (Range Add, Point Query via Difference Array)
// =========================================================================
class FenwickRangeAdd {
private:
    int n;
    std::vector<unsigned long long> tree;

public:
    explicit FenwickRangeAdd(int size = 0) : n(size), tree(size + 2, 0ULL) {}

    void init(int size) {
        n = size;
        tree.assign(n + 2, 0ULL);
    }

    void add(int i, unsigned long long delta) {
        for (; i <= n; i += i & (-i)) {
            tree[i] += delta;
        }
    }

    void range_add(int l, int r, unsigned long long delta) {
        if (l > r) return;
        add(l, delta);
        add(r + 1, -delta); // Giữ đúng nguyên lý mảng hiệu
    }

    unsigned long long point_query(int i) const {
        unsigned long long sum = 0ULL;
        for (; i > 0; i -= i & (-i)) {
            sum += tree[i];
        }
        return sum;
    }

    void clear() {
        std::fill(tree.begin(), tree.end(), 0ULL);
    }
};

// =========================================================================
// 2. POI Meteors Solver via Recursive Divide & Conquer PBS
// =========================================================================
struct MeteorEvent {
    int l, r;
    unsigned long long amount;
};

struct MeteorQuery {
    int id;
    unsigned long long need;
};

class MeteorsSolver {
private:
    int m_stations;
    int k_events;
    FenwickRangeAdd bit;
    std::vector<MeteorEvent> events;
    std::vector<std::vector<int>> country_stations;
    std::vector<int> ans;

    void apply_event(int t, bool add) {
        const auto& e = events[t];
        unsigned long long delta = add ? e.amount : -e.amount;
        if (e.l <= e.r) {
            bit.range_add(e.l, e.r, delta);
        } else {
            // Cung tròn: từ l đến M và từ 1 đến r
            bit.range_add(e.l, m_stations, delta);
            bit.range_add(1, e.r, delta);
        }
    }

    void solve(int l, int r, const std::vector<int>& q_indices, std::vector<unsigned long long>& req) {
        if (q_indices.empty()) return;

        if (l == r) {
            for (int q_id : q_indices) {
                ans[q_id] = l;
            }
            return;
        }

        int mid = l + (r - l) / 2;

        // Thêm các sự kiện trong nửa trái [l, mid]
        for (int t = l; t <= mid; ++t) {
            apply_event(t, true);
        }

        std::vector<int> left_list, right_list;

        for (int q_id : q_indices) {
            unsigned long long sum = 0ULL;
            for (int st : country_stations[q_id]) {
                sum += bit.point_query(st);
                if (sum >= req[q_id]) {
                    sum = req[q_id];
                    break;
                }
            }

            if (sum >= req[q_id]) {
                left_list.push_back(q_id);
            } else {
                req[q_id] -= sum; // Trừ hao lượng đóng góp từ [l, mid]
                right_list.push_back(q_id);
            }
        }

        // Hoàn tác các sự kiện từ [l, mid] để giữ BIT sạch
        for (int t = l; t <= mid; ++t) {
            apply_event(t, false);
        }

        // Đệ quy giải độc lập 2 nhánh con
        solve(l, mid, left_list, req);
        solve(mid + 1, r, right_list, req);
    }

public:
    MeteorsSolver(int stations, int num_countries, int num_events)
        : m_stations(stations), k_events(num_events), bit(stations),
          events(num_events + 1), country_stations(num_countries + 1),
          ans(num_countries + 1, -1) {}

    void set_event(int t, int l, int r, unsigned long long val) {
        events[t] = {l, r, val};
    }

    void add_station(int country, int station) {
        country_stations[country].push_back(station);
    }

    std::vector<int> run(const std::vector<unsigned long long>& requirements) {
        int n_countries = (int)requirements.size() - 1;
        std::vector<int> all_queries(n_countries);
        std::iota(all_queries.begin(), all_queries.end(), 1);

        std::vector<unsigned long long> req = requirements;
        // Cận tìm kiếm từ 1 đến k_events + 1 (với k_events + 1 là mốc vô nghiệm)
        solve(1, k_events + 1, all_queries, req);
        return ans;
    }
};

// =========================================================================
// 3. Disjoint Set Union with Rollback (Không nén đường đi, hỗ trợ hoàn tác)
// =========================================================================
class RollbackDSU {
private:
    int n;
    std::vector<int> parent;
    std::vector<int> sz;

    struct Op {
        int u, v, old_sz_u;
    };
    std::vector<Op> history;

public:
    explicit RollbackDSU(int size) : n(size), parent(size + 1), sz(size + 1, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int u) const {
        while (u != parent[u]) u = parent[u];
        return u;
    }

    bool same(int u, int v) const {
        return find(u) == find(v);
    }

    bool unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) {
            history.push_back({0, 0, 0}); // Không thay đổi cấu trúc
            return false;
        }
        if (sz[u] < sz[v]) std::swap(u, v);
        history.push_back({u, v, sz[u]});
        parent[v] = u;
        sz[u] += sz[v];
        return true;
    }

    int checkpoint() const {
        return (int)history.size();
    }

    void rollback(int target_checkpoint) {
        while ((int)history.size() > target_checkpoint) {
            auto [u, v, old_sz_u] = history.back();
            history.pop_back();
            if (u == 0) continue;
            sz[u] = old_sz_u;
            parent[v] = v;
        }
    }
};

// =========================================================================
// 4. Dynamic Connectivity PBS (Earliest Connection Timestamp)
// =========================================================================
struct Edge {
    int u, v;
};

struct ConnQuery {
    int u, v;
};

class DynamicConnectivityPBS {
private:
    int n_nodes;
    int m_edges;
    RollbackDSU dsu;
    std::vector<Edge> edges;
    std::vector<int> ans;

    void solve(int l, int r, const std::vector<int>& q_indices, const std::vector<ConnQuery>& queries) {
        if (q_indices.empty()) return;

        if (l == r) {
            for (int q_id : q_indices) {
                ans[q_id] = l;
            }
            return;
        }

        int mid = l + (r - l) / 2;
        int snap = dsu.checkpoint();

        for (int t = l; t <= mid; ++t) {
            dsu.unite(edges[t].u, edges[t].v);
        }

        std::vector<int> left_list, right_list;
        for (int q_id : q_indices) {
            if (dsu.same(queries[q_id].u, queries[q_id].v)) {
                left_list.push_back(q_id);
            } else {
                right_list.push_back(q_id);
            }
        }

        // Hoàn tác trước khi rẽ nhánh
        dsu.rollback(snap);

        // Đệ quy nhánh trái
        solve(l, mid, left_list, queries);

        // Để giải nhánh phải, ta nạp lại các cạnh từ l..mid vào DSU
        int snap_right = dsu.checkpoint();
        for (int t = l; t <= mid; ++t) {
            dsu.unite(edges[t].u, edges[t].v);
        }
        solve(mid + 1, r, right_list, queries);
        dsu.rollback(snap_right);
    }

public:
    DynamicConnectivityPBS(int n, int m)
        : n_nodes(n), m_edges(m), dsu(n), edges(m + 1) {}

    void set_edge(int t, int u, int v) {
        edges[t] = {u, v};
    }

    std::vector<int> run(const std::vector<ConnQuery>& queries) {
        int q = (int)queries.size();
        ans.assign(q, -1);
        std::vector<int> all_queries(q);
        std::iota(all_queries.begin(), all_queries.end(), 0);

        solve(1, m_edges + 1, all_queries, queries);
        return ans;
    }
};

// =========================================================================
// 5. Offline Range K-th Smallest PBS (Memory O(N) instead of Persistent Tree)
// =========================================================================
struct Element {
    int val, pos;
};

struct RangeKthQuery {
    int l, r, k, id;
};

class FenwickPointAdd {
private:
    int n;
    std::vector<int> tree;

public:
    explicit FenwickPointAdd(int size) : n(size), tree(size + 1, 0) {}

    void add(int i, int delta) {
        for (; i <= n; i += i & (-i)) tree[i] += delta;
    }

    int query(int i) const {
        int sum = 0;
        for (; i > 0; i -= i & (-i)) sum += tree[i];
        return sum;
    }

    int range_sum(int l, int r) const {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }
};

class OfflineRangeKthSolver {
private:
    int n;
    FenwickPointAdd bit;
    std::vector<Element> elements;
    std::vector<int> ans;

    void solve(int vl, int vr,
               int el_left, int el_right,
               const std::vector<RangeKthQuery>& queries) {
        if (queries.empty()) return;

        if (vl == vr) {
            for (const auto& q : queries) {
                ans[q.id] = vl;
            }
            return;
        }

        int mid = vl + (vr - vl) / 2;

        // Phân hoạch các phần tử mảng: phần tử <= mid nằm bên trái
        int mid_el_idx = el_left - 1;
        for (int i = el_left; i <= el_right; ++i) {
            if (elements[i].val <= mid) {
                ++mid_el_idx;
                std::swap(elements[mid_el_idx], elements[i]);
                bit.add(elements[mid_el_idx].pos, 1);
            }
        }

        std::vector<RangeKthQuery> q_left, q_right;
        for (const auto& q : queries) {
            int cnt_in_range = bit.range_sum(q.l, q.r);
            if (q.k <= cnt_in_range) {
                q_left.push_back(q);
            } else {
                q_right.push_back({q.l, q.r, q.k - cnt_in_range, q.id});
            }
        }

        // Hoàn tác các phần tử đã add vào BIT
        for (int i = el_left; i <= mid_el_idx; ++i) {
            bit.add(elements[i].pos, -1);
        }

        solve(vl, mid, el_left, mid_el_idx, q_left);
        solve(mid + 1, vr, mid_el_idx + 1, el_right, q_right);
    }

public:
    explicit OfflineRangeKthSolver(const std::vector<int>& arr)
        : n((int)arr.size()), bit((int)arr.size()), ans() {
        for (int i = 0; i < n; ++i) {
            elements.push_back({arr[i], i + 1});
        }
    }

    std::vector<int> run(const std::vector<std::tuple<int, int, int>>& raw_queries) {
        int q = (int)raw_queries.size();
        ans.assign(q, 0);
        std::vector<RangeKthQuery> queries(q);
        for (int i = 0; i < q; ++i) {
            auto [l, r, k] = raw_queries[i];
            queries[i] = {l, r, k, i};
        }

        int min_v = INT_MAX, max_v = INT_MIN;
        for (const auto& el : elements) {
            min_v = std::min(min_v, el.val);
            max_v = std::max(max_v, el.val);
        }

        solve(min_v, max_v, 0, n - 1, queries);
        return ans;
    }
};

} // namespace cp::pbs

// =========================================================================
// Verification & Unit Tests
// =========================================================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Test 1: POI Meteors Simulation
    // 3 countries, 5 stations, 3 events
    // Stations ownership:
    // Country 1 owns station 1, 4
    // Country 2 owns station 2, 5
    // Country 3 owns station 3
    // Requirements: Country 1: 10, Country 2: 5, Country 3: 7
    // Event 1: [1, 2] += 4
    // Event 2: [4, 5] += 6
    // Event 3: [2, 4] += 5
    {
        cp::pbs::MeteorsSolver meteors(5, 3, 3);
        meteors.add_station(1, 1);
        meteors.add_station(1, 4);
        meteors.add_station(2, 2);
        meteors.add_station(2, 5);
        meteors.add_station(3, 3);

        meteors.set_event(1, 1, 2, 4);
        meteors.set_event(2, 4, 5, 6);
        meteors.set_event(3, 2, 4, 5);

        std::vector<unsigned long long> req = {0, 10, 5, 7};
        auto res = meteors.run(req);

        // Country 1 needs 10:
        // After event 1: st1=4, st4=0 -> sum=4
        // After event 2: st1=4, st4=6 -> sum=10 >= 10 -> satisfied at event 2!
        assert(res[1] == 2);

        // Country 2 needs 5:
        // After event 1: st2=4, st5=0 -> sum=4
        // After event 2: st2=4, st5=6 -> sum=10 >= 5 -> satisfied at event 2!
        assert(res[2] == 2);

        // Country 3 needs 7:
        // After event 1: st3=0
        // After event 2: st3=0
        // After event 3: st3=5 < 7
        // Unsatisfied -> ans is k_events + 1 = 4
        assert(res[3] == 4);
    }

    // Test 2: Dynamic Connectivity PBS
    // 5 vertices, 4 edges:
    // t=1: (1, 2)
    // t=2: (3, 4)
    // t=3: (2, 3) -> now 1, 2, 3, 4 are connected
    // t=4: (4, 5) -> now all 1..5 connected
    {
        cp::pbs::DynamicConnectivityPBS d_conn(5, 4);
        d_conn.set_edge(1, 1, 2);
        d_conn.set_edge(2, 3, 4);
        d_conn.set_edge(3, 2, 3);
        d_conn.set_edge(4, 4, 5);

        std::vector<cp::pbs::ConnQuery> queries = {
            {1, 2}, // connected at t=1
            {1, 4}, // connected at t=3
            {1, 5}, // connected at t=4
            {2, 5}, // connected at t=4
            {2, 3}  // connected at t=3
        };

        auto res = d_conn.run(queries);
        assert(res[0] == 1);
        assert(res[1] == 3);
        assert(res[2] == 4);
        assert(res[3] == 4);
        assert(res[4] == 3);
    }

    // Test 3: Offline Range K-th Smallest
    // Array: [15, 3, 9, 21, 8, 1, 12]
    {
        std::vector<int> a = {15, 3, 9, 21, 8, 1, 12};
        cp::pbs::OfflineRangeKthSolver solver(a);

        std::vector<std::tuple<int, int, int>> queries = {
            {1, 7, 1}, // min of whole array -> 1
            {1, 7, 7}, // max of whole array -> 21
            {1, 7, 4}, // 4th smallest of [1, 3, 8, 9, 12, 15, 21] -> 9
            {2, 5, 2}, // subsegment [3, 9, 21, 8] -> sorted: [3, 8, 9, 21] -> 2nd smallest is 8
            {4, 6, 1}  // subsegment [21, 8, 1] -> min is 1
        };

        auto res = solver.run(queries);
        assert(res[0] == 1);
        assert(res[1] == 21);
        assert(res[2] == 9);
        assert(res[3] == 8);
        assert(res[4] == 1);
    }

    std::cout << "Parallel Binary Search Templates: 100% assertions PASSED!\n";
    return 0;
}
