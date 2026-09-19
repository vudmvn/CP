/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Kỹ thuật Euler Tour trên Cây (Euler Tour Technique on Trees - ETT) (C++20)
 * @author Duc-Minh Vu (SLSCM Lab - FDA - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cassert>

using namespace std;

// ==============================================================================
// 1. EULER TOUR DẠNG 1: SUBTREE FLATTENING (N PHẦN TỬ) & FENWICK TREE
// ==============================================================================
namespace SubtreeEulerTour {

    struct FenwickTree {
        int n;
        vector<long long> bit;
        FenwickTree(int n) : n(n), bit(n + 1, 0) {}

        void update(int idx, long long val) {
            for (; idx <= n; idx += idx & -idx) bit[idx] += val;
        }

        long long query(int idx) const {
            long long sum = 0;
            for (; idx > 0; idx -= idx & -idx) sum += bit[idx];
            return sum;
        }

        long long query_range(int l, int r) const {
            if (l > r) return 0;
            return query(r) - query(l - 1);
        }
    };

    class TreeSubtreeManager {
    public:
        int n;
        int timer;
        vector<int> tin, tout, flat_node, depth;
        vector<vector<int>> adj;
        FenwickTree ft;

        TreeSubtreeManager(int n) 
            : n(n), timer(0), tin(n + 1), tout(n + 1), flat_node(n + 1), depth(n + 1), adj(n + 1), ft(n) {}

        void add_edge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        void dfs(int u, int p, int d = 0) {
            tin[u] = ++timer;
            flat_node[timer] = u;
            depth[u] = d;

            for (int v : adj[u]) {
                if (v != p) {
                    dfs(v, u, d + 1);
                }
            }
            tout[u] = timer;
        }

        void build(int root = 1) {
            timer = 0;
            dfs(root, 0);
        }

        /// Kiểm tra u có phải là tổ tiên của v trong O(1)
        bool is_ancestor(int u, int v) const {
            return tin[u] <= tin[v] && tout[u] >= tout[v];
        }

        /// Cập nhật giá trị cộng thêm val vào đỉnh u
        void update_node(int u, long long val) {
            ft.update(tin[u], val);
        }

        /// Truy vấn tổng giá trị của toàn bộ cây con gốc u trong O(log N)
        long long query_subtree(int u) const {
            return ft.query_range(tin[u], tout[u]);
        }
    };
}

// ==============================================================================
// 2. EULER TOUR DẠNG 2: FULL EULER TOUR (2N - 1 PHẦN TỬ) & LCA O(1)
// ==============================================================================
namespace EulerTourLCA {

    class FastLCA {
    private:
        int n;
        vector<vector<int>> adj;
        vector<int> first_occ;
        vector<int> euler;
        vector<int> depth_arr;
        vector<int> node_depth;
        vector<vector<int>> st; // Sparse Table lưu index của độ sâu nhỏ nhất

        void dfs(int u, int p, int d) {
            first_occ[u] = euler.size();
            euler.push_back(u);
            depth_arr.push_back(d);
            node_depth[u] = d;

            for (int v : adj[u]) {
                if (v != p) {
                    dfs(v, u, d + 1);
                    euler.push_back(u);
                    depth_arr.push_back(d);
                }
            }
        }

        void build_sparse_table() {
            int m = euler.size();
            int log_m = 32 - __builtin_clz(m);
            st.assign(log_m, vector<int>(m));

            for (int i = 0; i < m; ++i) st[0][i] = i;

            for (int k = 1; (1 << k) <= m; ++k) {
                for (int i = 0; i + (1 << k) <= m; ++i) {
                    int idx1 = st[k - 1][i];
                    int idx2 = st[k - 1][i + (1 << (k - 1))];
                    st[k][i] = (depth_arr[idx1] < depth_arr[idx2]) ? idx1 : idx2;
                }
            }
        }

    public:
        FastLCA(int n) : n(n), adj(n + 1), first_occ(n + 1), node_depth(n + 1) {}

        void add_edge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        void build(int root = 1) {
            euler.clear();
            depth_arr.clear();
            dfs(root, 0, 0);
            build_sparse_table();
        }

        /// Truy vấn LCA của 2 đỉnh bất kỳ trong O(1) tuyệt đối
        int get_lca(int u, int v) const {
            int L = first_occ[u], R = first_occ[v];
            if (L > R) swap(L, R);
            int len = R - L + 1;
            int k = 31 - __builtin_clz(len);

            int idx1 = st[k][L];
            int idx2 = st[k][R - (1 << k) + 1];
            int best_idx = (depth_arr[idx1] < depth_arr[idx2]) ? idx1 : idx2;
            return euler[best_idx];
        }

        /// Tính khoảng cách số cạnh giữa u và v trong O(1)
        int get_dist(int u, int v) const {
            int lca = get_lca(u, v);
            return node_depth[u] + node_depth[v] - 2 * node_depth[lca];
        }

        int get_depth(int u) const {
            return node_depth[u];
        }
    };
}

// ==============================================================================
// 3. EULER TOUR DẠNG 3: PATH FLATTENING (2N PHẦN TỬ) CHO MO'S ALGORITHM TRÊN CÂY
// ==============================================================================
namespace TreeMoAlgorithm {

    struct Query {
        int id;
        int l, r;
        int lca; // Đỉnh LCA phụ cần tính (nếu lca != u && lca != v)
        int block_id;

        bool operator<(const Query& other) const {
            if (block_id != other.block_id) return block_id < other.block_id;
            return (block_id & 1) ? (r < other.r) : (r > other.r);
        }
    };

    class TreeMoHelper {
    public:
        int n;
        int timer;
        vector<vector<int>> adj;
        vector<int> tin, tout;
        vector<int> flat_order; // Kích thước 2N + 1
        EulerTourLCA::FastLCA lca_solver;

        TreeMoHelper(int n) 
            : n(n), timer(0), adj(n + 1), tin(n + 1), tout(n + 1), flat_order(2 * n + 1), lca_solver(n) {}

        void add_edge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
            lca_solver.add_edge(u, v);
        }

        void dfs(int u, int p) {
            tin[u] = ++timer;
            flat_order[timer] = u;

            for (int v : adj[u]) {
                if (v != p) {
                    dfs(v, u);
                }
            }

            tout[u] = ++timer;
            flat_order[timer] = u;
        }

        void build(int root = 1) {
            timer = 0;
            dfs(root, 0);
            lca_solver.build(root);
        }

        /// Chuyển truy vấn đường đi u -> v thành đoạn trên mảng 2N
        Query make_query(int id, int u, int v, int block_size) const {
            if (tin[u] > tin[v]) swap(u, v);
            int lca = lca_solver.get_lca(u, v);

            Query q;
            q.id = id;
            if (lca == u) {
                // u là tổ tiên của v: Đoạn [tin[u], tin[v]]
                q.l = tin[u];
                q.r = tin[v];
                q.lca = 0; // Đã bao gồm u
            } else {
                // u và v không phải tổ tiên của nhau: Đoạn [tout[u], tin[v]] + thêm LCA
                q.l = tout[u];
                q.r = tin[v];
                q.lca = lca;
            }
            q.block_id = q.l / block_size;
            return q;
        }
    };
}

// ==============================================================================
// 4. EULER TOUR TRÊN CẠNH (EDGE MAPPING)
// ==============================================================================
namespace EdgeEulerTour {

    class EdgeTreeManager {
    public:
        int n;
        int timer;
        vector<int> tin, tout;
        vector<int> parent_node;
        vector<int> edge_node; // edge_node[edge_idx]: đỉnh con tương ứng với cạnh đó
        vector<vector<pair<int, int>>> adj; // {v, edge_idx}

        EdgeTreeManager(int n) 
            : n(n), timer(0), tin(n + 1), tout(n + 1), parent_node(n + 1, 0), adj(n + 1) {}

        void add_edge(int edge_idx, int u, int v) {
            adj[u].push_back({v, edge_idx});
            adj[v].push_back({u, edge_idx});
        }

        void dfs(int u, int p) {
            tin[u] = ++timer;
            parent_node[u] = p;

            for (const auto& [v, e_id] : adj[u]) {
                if (v != p) {
                    if ((int)edge_node.size() <= e_id) edge_node.resize(e_id + 1);
                    edge_node[e_id] = v; // Cạnh e_id nối lên cha u của v, gán cho v
                    dfs(v, u);
                }
            }
            tout[u] = timer;
        }

        void build(int root = 1) {
            timer = 0;
            dfs(root, 0);
        }

        /// Lấy vị trí 1D trên mảng ETT tương ứng với cạnh edge_idx
        int get_edge_pos(int edge_idx) const {
            int v = edge_node[edge_idx];
            return tin[v];
        }

        /// Lấy đoạn [L, R] chứa toàn bộ các cạnh trong subtree u (loại trừ cạnh đi vào u)
        pair<int, int> get_subtree_edges_range(int u) const {
            return {tin[u] + 1, tout[u]};
        }
    };
}

// ==============================================================================
// VÍ DỤ MINH HỌA KIỂM THỬ (TEST HARNESS)
// ==============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Cây mẫu: 5 đỉnh
    // 1 - 2, 1 - 3, 2 - 4, 2 - 5
    int n = 5;

    // 1. Kiểm thử Subtree Euler Tour & Fenwick Tree
    SubtreeEulerTour::TreeSubtreeManager tree(n);
    tree.add_edge(1, 2); tree.add_edge(1, 3);
    tree.add_edge(2, 4); tree.add_edge(2, 5);
    tree.build(1);

    cout << "=== 1. Subtree Euler Tour Test ===" << "\n";
    cout << "is_ancestor(1, 4): " << boolalpha << tree.is_ancestor(1, 4) << "\n"; // true
    cout << "is_ancestor(3, 5): " << boolalpha << tree.is_ancestor(3, 5) << "\n"; // false

    // Cập nhật giá trị đỉnh
    tree.update_node(4, 10);
    tree.update_node(5, 20);
    tree.update_node(2, 5);
    cout << "Subtree Sum of Node 2: " << tree.query_subtree(2) << "\n"; // 35 (2 + 4 + 5)
    cout << "Subtree Sum of Node 1: " << tree.query_subtree(1) << "\n"; // 35

    // 2. Kiểm thử Fast LCA O(1) bằng Full Euler Tour (2N - 1)
    EulerTourLCA::FastLCA lca_solver(n);
    lca_solver.add_edge(1, 2); lca_solver.add_edge(1, 3);
    lca_solver.add_edge(2, 4); lca_solver.add_edge(2, 5);
    lca_solver.build(1);

    cout << "\n=== 2. Fast LCA O(1) Test ===" << "\n";
    cout << "LCA(4, 5) = " << lca_solver.get_lca(4, 5) << " (Dist = " << lca_solver.get_dist(4, 5) << ")\n";
    cout << "LCA(4, 3) = " << lca_solver.get_lca(4, 3) << " (Dist = " << lca_solver.get_dist(4, 3) << ")\n";

    // 3. Kiểm thử Tree Mo Path Mapping (2N)
    TreeMoAlgorithm::TreeMoHelper mo_helper(n);
    mo_helper.add_edge(1, 2); mo_helper.add_edge(1, 3);
    mo_helper.add_edge(2, 4); mo_helper.add_edge(2, 5);
    mo_helper.build(1);

    cout << "\n=== 3. Tree Mo Path Mapping Test ===" << "\n";
    int block_size = max(1, (int)(sqrt(2 * n)));
    auto q1 = mo_helper.make_query(1, 1, 4, block_size); // 1 là tổ tiên của 4
    cout << "Path (1 -> 4): range [" << q1.l << ", " << q1.r << "], extra LCA = " << q1.lca << "\n";

    auto q2 = mo_helper.make_query(2, 4, 3, block_size); // 4 và 3 không phải tổ tiên của nhau
    cout << "Path (4 -> 3): range [" << q2.l << ", " << q2.r << "], extra LCA = " << q2.lca << "\n";

    return 0;
}
