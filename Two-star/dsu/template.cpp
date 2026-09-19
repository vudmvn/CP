/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Cấu trúc Dữ liệu Các Tập hợp Rời nhau (Disjoint Set Union - DSU) (C++20)
 * @author Duc-Minh Vu (SLSCM Lab - FDA - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>

using namespace std;

// ==============================================================================
// 1. DSU CHUẨN MỰC: NÉN ĐƯỜNG ĐI (PATH COMPRESSION) & GỘP THEO KÍCH THƯỚC (UNION BY SIZE)
// ==============================================================================
namespace StandardDSU {

    class DSU {
    private:
        int n;
        vector<int> parent;
        vector<int> sz;
        int num_components;

    public:
        DSU(int n = 0) : n(n), parent(n + 1), sz(n + 1, 1), num_components(n) {
            iota(parent.begin(), parent.end(), 0);
        }

        int find(int u) {
            if (u == parent[u]) return u;
            return parent[u] = find(parent[u]); // Nén đường đi O(alpha(N))
        }

        bool same(int u, int v) {
            return find(u) == find(v);
        }

        bool unite(int u, int v) {
            int root_u = find(u);
            int root_v = find(v);
            if (root_u == root_v) return false;

            // Gộp theo kích thước (treo cây nhỏ vào cây lớn)
            if (sz[root_u] < sz[root_v]) swap(root_u, root_v);
            parent[root_v] = root_u;
            sz[root_u] += sz[root_v];
            num_components--;
            return true;
        }

        int get_size(int u) {
            return sz[find(u)];
        }

        int count_components() const {
            return num_components;
        }
    };
}

// ==============================================================================
// 2. DSU THẾ VỊ / TRỌNG SỐ (POTENTIALIZED DSU - WEIGHED DSU)
// ==============================================================================
namespace PotentializedDSU {

    template<typename T = long long>
    class WeightedDSU {
    private:
        int n;
        vector<int> parent;
        vector<T> pot; // pot[u]: hiệu giá trị từ u đến parent[u] (val[u] - val[parent[u]])

    public:
        WeightedDSU(int n = 0) : n(n), parent(n + 1), pot(n + 1, 0) {
            iota(parent.begin(), parent.end(), 0);
        }

        int find(int u) {
            if (u == parent[u]) return u;
            int p = parent[u];
            parent[u] = find(p);
            pot[u] += pot[p]; // Cộng dồn thế vị dọc theo đường nén lên gốc
            return parent[u];
        }

        bool same(int u, int v) {
            return find(u) == find(v);
        }

        /**
         * @brief Thêm ràng buộc val[u] - val[v] = w
         * @return true nếu ràng buộc hợp lệ, false nếu phát hiện mâu thuẫn
         */
        bool unite(int u, int v, T w) {
            int root_u = find(u);
            int root_v = find(v);

            if (root_u == root_v) {
                // Đã cùng tập hợp: kiểm tra tính tương thích
                return (pot[u] - pot[v] == w);
            }

            // Gộp root_u vào root_v:
            // val[u] - val[v] = (pot[u] + val[root_u]) - (pot[v] + val[root_v]) = w
            // => val[root_u] - val[root_v] = w + pot[v] - pot[u]
            parent[root_u] = root_v;
            pot[root_u] = w + pot[v] - pot[u];
            return true;
        }

        /**
         * @brief Lấy hiệu val[u] - val[v] khi đã biết cùng tập hợp
         */
        T get_diff(int u, int v) {
            assert(same(u, v));
            return pot[u] - pot[v];
        }
    };
}

// ==============================================================================
// 3. DSU CÓ THỂ HOÀN TÁC (ROLLBACK DSU / UNDO DSU)
// ==============================================================================
namespace RollbackDSU {

    struct RollbackEvent {
        int u, v;  // v được gộp vào u
        int sz_u;  // kích thước cũ của u trước khi gộp
    };

    class UndoDSU {
    private:
        int n;
        vector<int> parent;
        vector<int> sz;
        vector<RollbackEvent> history;
        int num_components;

    public:
        UndoDSU(int n = 0) : n(n), parent(n + 1), sz(n + 1, 1), num_components(n) {
            iota(parent.begin(), parent.end(), 0);
        }

        // CẤM NÉN ĐƯỜNG ĐI! Chỉ đi lên gốc O(log N)
        int find(int u) const {
            while (u != parent[u]) u = parent[u];
            return u;
        }

        bool same(int u, int v) const {
            return find(u) == find(v);
        }

        bool unite(int u, int v) {
            int root_u = find(u);
            int root_v = find(v);

            if (root_u == root_v) {
                history.push_back({-1, -1, 0}); // Không thay đổi cấu trúc
                return false;
            }

            if (sz[root_u] < sz[root_v]) swap(root_u, root_v);

            history.push_back({root_u, root_v, sz[root_u]});
            parent[root_v] = root_u;
            sz[root_u] += sz[root_v];
            num_components--;
            return true;
        }

        void rollback() {
            assert(!history.empty());
            auto [u, v, old_sz] = history.back();
            history.pop_back();

            if (u == -1) return;

            parent[v] = v;
            sz[u] = old_sz;
            num_components++;
        }

        int snapshot() const {
            return (int)history.size();
        }

        void rollback_to(int snap) {
            while ((int)history.size() > snap) {
                rollback();
            }
        }

        int count_components() const {
            return num_components;
        }
    };
}

// ==============================================================================
// 4. CÂY TÁI CẤU TRÚC KRUSKAL (KRUSKAL RECONSTRUCTION TREE)
// ==============================================================================
namespace KruskalTree {

    struct Edge {
        int u, v;
        long long w;
        bool operator<(const Edge& other) const {
            return w < other.w;
        }
    };

    class KruskalReconstructionTree {
    private:
        int n;
        int total_nodes;
        vector<long long> node_val; // Trọng số cạnh ứng với đỉnh ảo
        vector<vector<int>> adj;
        vector<vector<int>> up;
        vector<int> depth;
        int LOGN;

        int find_root(int u, vector<int>& p) {
            if (u == p[u]) return u;
            return p[u] = find_root(p[u], p);
        }

        void dfs(int u, int p, int d) {
            up[0][u] = p;
            depth[u] = d;
            for (int i = 1; i < LOGN; ++i) {
                up[i][u] = up[i - 1][up[i - 1][u]];
            }

            for (int v : adj[u]) {
                if (v != p) {
                    dfs(v, u, d + 1);
                }
            }
        }

    public:
        KruskalReconstructionTree(int n) : n(n), total_nodes(n) {}

        void build(vector<Edge> edges) {
            sort(edges.begin(), edges.end());
            int max_nodes = 2 * n;
            total_nodes = n;

            vector<int> p(max_nodes + 1);
            iota(p.begin(), p.end(), 0);

            node_val.assign(max_nodes + 1, 0);
            adj.assign(max_nodes + 1, vector<int>());

            for (const auto& edge : edges) {
                int ru = find_root(edge.u, p);
                int rv = find_root(edge.v, p);
                if (ru != rv) {
                    total_nodes++;
                    node_val[total_nodes] = edge.w;
                    adj[total_nodes].push_back(ru);
                    adj[total_nodes].push_back(rv);
                    p[ru] = total_nodes;
                    p[rv] = total_nodes;
                }
            }

            LOGN = 32 - __builtin_clz(total_nodes);
            up.assign(LOGN, vector<int>(total_nodes + 1, 0));
            depth.assign(total_nodes + 1, 0);

            // Duyệt từ các gốc của rừng Kruskal
            for (int i = 1; i <= total_nodes; ++i) {
                if (p[i] == i) {
                    dfs(i, i, 0);
                }
            }
        }

        int get_lca(int u, int v) const {
            if (depth[u] < depth[v]) swap(u, v);
            for (int i = LOGN - 1; i >= 0; --i) {
                if (depth[u] - (1 << i) >= depth[v]) {
                    u = up[i][u];
                }
            }
            if (u == v) return u;
            for (int i = LOGN - 1; i >= 0; --i) {
                if (up[i][u] != up[i][v]) {
                    u = up[i][u];
                    v = up[i][v];
                }
            }
            return up[0][u];
        }

        /// Tìm trọng số cạnh lớn nhất trên đường đi Minimax giữa u và v
        long long query_minimax(int u, int v) const {
            int lca = get_lca(u, v);
            return node_val[lca];
        }
    };
}

// ==============================================================================
// VÍ DỤ MINH HỌA KIỂM THỬ (TEST HARNESS)
// ==============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1. Kiểm thử Standard DSU
    StandardDSU::DSU dsu(5);
    dsu.unite(1, 2);
    dsu.unite(3, 4);
    cout << "=== 1. Standard DSU Test ===" << "\n";
    cout << "same(1, 2): " << boolalpha << dsu.same(1, 2) << "\n"; // true
    cout << "same(1, 3): " << boolalpha << dsu.same(1, 3) << "\n"; // false
    cout << "size of comp(1): " << dsu.get_size(1) << "\n";        // 2
    cout << "total components: " << dsu.count_components() << "\n"; // 3

    // 2. Kiểm thử Potentialized DSU
    PotentializedDSU::WeightedDSU<long long> w_dsu(4);
    // val[1] - val[2] = 5, val[2] - val[3] = 3
    w_dsu.unite(1, 2, 5);
    w_dsu.unite(2, 3, 3);
    cout << "\n=== 2. Potentialized DSU Test ===" << "\n";
    cout << "val[1] - val[3] = " << w_dsu.get_diff(1, 3) << "\n"; // 8
    // Thêm ràng buộc mâu thuẫn: val[1] - val[3] = 10 -> false
    cout << "Add valid constraint (1 - 3 = 8): " << boolalpha << w_dsu.unite(1, 3, 8) << "\n";  // true
    cout << "Add conflict constraint (1 - 3 = 10): " << boolalpha << w_dsu.unite(1, 3, 10) << "\n"; // false

    // 3. Kiểm thử Rollback DSU
    RollbackDSU::UndoDSU undo_dsu(5);
    undo_dsu.unite(1, 2);
    int snap = undo_dsu.snapshot();
    undo_dsu.unite(2, 3);
    undo_dsu.unite(4, 5);
    cout << "\n=== 3. Rollback DSU Test ===" << "\n";
    cout << "Components before rollback: " << undo_dsu.count_components() << "\n"; // 2 ( {1,2,3}, {4,5} )
    undo_dsu.rollback_to(snap);
    cout << "Components after rollback to snap: " << undo_dsu.count_components() << "\n"; // 4 ( {1,2}, {3}, {4}, {5} )
    cout << "same(2, 3) after rollback: " << boolalpha << undo_dsu.same(2, 3) << "\n"; // false

    // 4. Kiểm thử Kruskal Reconstruction Tree
    // Đồ thị: 1-2 (w=10), 2-3 (w=20), 1-3 (w=30), 3-4 (w=15)
    vector<KruskalTree::Edge> edges = {
        {1, 2, 10}, {2, 3, 20}, {1, 3, 30}, {3, 4, 15}
    };
    KruskalTree::KruskalReconstructionTree kt(4);
    kt.build(edges);
    cout << "\n=== 4. Kruskal Tree Minimax Query ===" << "\n";
    cout << "Minimax edge between 1 and 4: " << kt.query_minimax(1, 4) << "\n"; // 20

    return 0;
}
